/**
 * Authors: Matthew Lund and Mohammad Javid
 * Contact info: mtlund@ucsd.edu (Matthew),
 *               Msamadpo@ucsd.edu (Mohammad)
 *
 * Sources used:
 *  1) CSE 100 Winter 2020, Lecture slides and discussion slides.
 *
 * Description of File:
 * This file allows the user to take an input file and compress
 * it into its encoded form. It allows the user to use true or
 * ascii level encoding.
 *
 */
#include <fstream>
#include <iostream>

#include <algorithm>
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

const unsigned int FILE_OUTPUT_POSITION = 2;
const unsigned int FILE_INPUT_POSITION = 2;
const unsigned int FILE_OUTPUT_POSITION_ASCII = 3;
const unsigned int SET_SECOND_BIT = 0x02;
const unsigned int SET_FIRST_BIT = 0x01;
const unsigned int SIZE_OF_BUFFER = 4000;
const unsigned int NUMBER_OF_CHARS = 256;

/*
 * This method allows the user to compress a file
 * that using ascii level compression. It assumes that the
 * user is utilizing a valid input file and has provided
 * the name that the output file should have.
 *
 * Source: CSE 100 Lecture Slides and Discussion Slides.
 *
 * Parameters:
 *  1) inFileName - the name of the input file.
 *  2) outFileName - the name of the output file.
 *
 * Edge Cases:
 *  1) All possible edge cases where accounted for in the main
 *     method.
 */
void pseudoCompression(string inFileName, string outFileName) {
    // Creates a vector to store the frequency of each character.
    vector<unsigned int> ofChars(NUMBER_OF_CHARS);
    HCTree* ofAllChars = new HCTree();
    ifstream ofInput;
    unsigned char nextChar;
    int nextByte;
    ofInput.open(inFileName, ios::binary);
    // Loops through input file, counts the frequency of characters.
    while ((nextByte = ofInput.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        ofChars.at(nextChar)++;
    }
    ofInput.close();
    // Build a huffman tree based on the frequencies.
    ofAllChars->build(ofChars);
    ofstream ofOutput;
    ofOutput.open(outFileName);
    // Adds the frequency of each character to the header.
    for (int i = 0; i < ofChars.size(); i++) {
        unsigned int ofIndex = ofChars.at(i);
        ofOutput << ofIndex << endl;
    }
    ofInput.open(inFileName, ios::binary);
    // Encodes each character in the input file.
    while ((nextByte = ofInput.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        ofAllChars->encode(nextChar, ofOutput);
    }
    // Closes files used, frees memory used.
    ofInput.close();
    ofOutput.close();
    delete ofAllChars;
}

/*
 * This method allows the user to compress a file
 * that using "true" level compression. It assumes that the
 * user is utilizing a valid input file and has provided
 * the name that the output file should have.
 *
 * Source: CSE 100 Lecture Slides and Discussion Slides.
 *
 * Parameters:
 *  1) inFileName - the name of the input file.
 *  2) outFileName - the name of the output file.
 *
 * Edge Cases:
 *  1) All possible edge cases where accounted for in the main
 *     method.
 */
void trueCompression(string inFileName, string outFileName) {
    // Creates a vector to store the frequency of each character.
    vector<unsigned int> ofChars(NUMBER_OF_CHARS);
    HCTree* ofAllChars = new HCTree();
    ifstream ofInput;
    unsigned char nextChar;
    int nextByte;
    // Stores the number of characters in the input file.
    unsigned int totalChars = 0;
    /*
    / Character which outlines if the tree contains
    / a new line character.
    */
    unsigned char newLineRules = 0;
    ofInput.open(inFileName, ios::binary);
    /*
    / Loops through each character, counting its frequency
    / and the total number of characters.
    */
    while ((nextByte = ofInput.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        ofChars.at(nextChar)++;
        totalChars++;
    }
    ofInput.close();
    ofstream ofOutput;
    ofOutput.open(outFileName);
    // Adds the number of characters in input file to header.
    ofOutput << totalChars << endl;
    // Builds the huffman tree.
    ofAllChars->build(ofChars);
    auto ofHeader = new vector<char>();
    // Adds characters to the header to represent the tree.
    ofAllChars->recursiveIteration(ofHeader, nullptr);
    std::reverse(ofHeader->begin(), ofHeader->end());
    // Checks if the tree has a new line character.
    for (int i = 0; i < ofHeader->size(); i++) {
        if (ofHeader->at(i) == '\n') {
            /*
            / Checks to see if the new line character
            / is the last one.
            */
            if (i == ofHeader->size() - 1) {
                // Sets bit representing the last character.
                newLineRules = newLineRules + SET_FIRST_BIT;
            } else {
                // Sets bit representing not the last character.
                newLineRules = newLineRules + SET_SECOND_BIT;
            }
        }
    }
    // Adds new line info to the header.
    ofOutput.put(newLineRules);
    ofOutput.put('\n');
    // Adds the huffman tree representation to the header.
    for (int i = 0; i < ofHeader->size(); i++) {
        ofOutput.put(ofHeader->at(i));
    }
    ofOutput.put('\n');
    ofInput.open(inFileName, ios::binary);
    BitOutputStream bos(ofOutput, SIZE_OF_BUFFER);
    // Encodes each character in input file, adds it to encoded file.
    while ((nextByte = ofInput.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        ofAllChars->encode(nextChar, bos);
    }
    // Flushes buffer to make sure everything was written.
    bos.flush();
    // Closes files used, frees memory used.
    ofInput.close();
    ofOutput.close();
    delete ofAllChars;
    delete ofHeader;
}

/*
 * This handels the function calling and the interaction that
 * the user has with the program. It provides the user with
 * the required info for him/she to achieve the desired outcome.
 * It accepts both ascii and true compression specifications.
 *
 * Source: CSE 100 PA3 Outline.
 *
 * Parameters:
 *  1) argc - The number of inputs given by the user.
 *  2) argv - The input of the user as strings.
 *
 * Edge Cases:
 *  1) The files do not exists -> return
 *  2) The input file is empty -> output empty file.
 */
int main(int argc, char* argv[]) {
    cxxopts::Options options("./compress",
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    // Checks if the input is valid or if the user wants help
    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        return 0;
    }

    /*
    / Checks if we want true compression
    */
    if (argv[FILE_OUTPUT_POSITION_ASCII] == NULL) {
        // Checks if file is empty.
        if (FileUtils::isEmptyFile(argv[1])) {
            ofstream ofOutput;
            ofOutput.open(argv[FILE_OUTPUT_POSITION], ios::trunc);
            ofOutput.close();
            return 0;
        }
        trueCompression(argv[1], argv[FILE_OUTPUT_POSITION]);
    } else {
        string ofInputName = argv[FILE_INPUT_POSITION];
        string ofOutputName = argv[FILE_OUTPUT_POSITION_ASCII];
        // Checks if file is empty.
        if (FileUtils::isEmptyFile(ofInputName)) {
            ofstream ofOutput;
            ofOutput.open(ofOutputName, ios::trunc);
            ofOutput.close();
            return 0;
        }
        pseudoCompression(ofInputName, ofOutputName);
    }
    return 0;
}