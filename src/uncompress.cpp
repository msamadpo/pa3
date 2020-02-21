/**
 * Authors: Matthew Lund and Mohammad Javid
 * Contact info: mtlund@ucsd.edu (Matthew),
 *               Msamadpo@ucsd.edu (Mohammad)
 *
 * Sources used:
 *  2) CSE 100 Winter 2020, Lecture slides and Discussion slides.
 *
 * Description of File:
 * This file allows the user to take a compressed file
 * and output its decoded from. It assumes that the
 * user either compressed it with ascii or "true"
 * compression style.
 *
 */
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

const unsigned int FILE_OUTPUT_POSITION = 2;
const unsigned int FILE_INPUT_POSITION = 2;
const unsigned int FILE_OUTPUT_POSITION_ASCII = 3;
const unsigned int VALUE_SECOND_BIT = 2;
const unsigned int SIZE_OF_BUFFER = 4000;
const unsigned int NUMBER_OF_CHARS = 256;

/*
 * This method allows the user to decompress an encoded file
 * that used ascii level compression. It assumes that the
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
void pseudoDecompression(string inFileName, string outFileName) {
    // Stores the frequency of each character
    vector<unsigned int> ofChars(NUMBER_OF_CHARS);
    HCTree* ofAllChars = new HCTree();
    ifstream ofInput;
    unsigned char nextChar;
    int nextByte;
    /*
    / Opens the file we want to decode,
    / counts the frequency of each char.
    */
    ofInput.open(inFileName, ios::binary);
    for (int i = 0; i < NUMBER_OF_CHARS; i++) {
        ofInput >> ofChars.at(i);
    }
    // Build the huffman tree.
    ofAllChars->build(ofChars);
    // Creates or opens the file which to store the decode.
    ofstream ofOutput;
    ofOutput.open(outFileName, ios::trunc);
    // Skips the \n char marking the end of the header.
    ofInput.get();
    /*
    / Loops through each character in the input file,
    / finds the encoding belonging to the sequence of
    / characters, adds the decoded character to output file.
    */
    while ((nextByte = ofInput.peek()) != EOF) {
        ofOutput << ofAllChars->decode(ofInput);
    }
    // Closes the files, frees memory.
    ofInput.close();
    ofOutput.close();
    delete ofAllChars;
}

/*
 * This method allows the user to decompress an encoded file
 * that used "true" compression. It assumes that the
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
void trueDecompression(string inFileName, string outFileName) {
    // Initializes an empty tree.
    HCTree* ofAllChars = new HCTree();
    // Vector stores the characters that make up the tree.
    vector<char>* ofHeader = new vector<char>();
    ifstream ofInput;
    int nextByte;
    // Keeps track of the number of characters to decode.
    unsigned int totalChars = 0;
    // Opens the file containing the encoded data.
    ofInput.open(inFileName, ios::binary);
    // Gets the number of characters from the header.
    ofInput >> totalChars;
    // Moves to next line.
    ofInput.get();
    /*
    / Gets a character which stores information about
    / the possability that the huffman tree contains
    / a new line character.
    */
    unsigned int ofNewLineRules = ofInput.get();
    // Moves to next line.
    ofInput.get();
    /*
    / Used to keep track of the current character
    / we are at.
    */
    unsigned char nextChar = ' ';
    /*
    / Loops through the characters representing the huffman
    / tree until it hits the new line character.
    */
    while ((nextChar = ofInput.get()) != '\n') {
        ofHeader->push_back(nextChar);
    }
    // Checks if the tree contains a new line character.
    if (ofNewLineRules > 0) {
        /*
        / Checks to see if the tree continues past
        / the new line character.
        */
        if (ofNewLineRules == VALUE_SECOND_BIT) {
            /*
            / Push the new line character that is part of the tree,
            / continue to add characters that make up the tree.
            */
            ofHeader->push_back(nextChar);
            while ((nextChar = ofInput.get()) != '\n') {
                ofHeader->push_back(nextChar);
            }
        } else {
            // Adds the last character that makes up the tree.
            ofHeader->push_back(nextChar);
            ofInput.get();
        }
    }
    // Builds the tree recursively based on the header analyzed.
    ofAllChars->recursiveBuild(ofHeader, nullptr);
    ofstream ofOutput;
    ofOutput.open(outFileName);
    BitInputStream bs(ofInput, SIZE_OF_BUFFER);
    // Decodes all of the bits in the encoding part of the file.
    while (totalChars > 0) {
        ofOutput << ofAllChars->decode(bs);
        totalChars--;
    }
    // Closes files, frees the neccessary memory.
    ofInput.close();
    ofOutput.close();
    delete ofAllChars;
    delete ofHeader;
}

/*
 * This handels the function calling and the interaction that
 * the user has with the program. It provides the user with
 * the required info for him/she to achieve the desired outcome.
 * It accepts both ascii and true decompression.
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
    cxxopts::Options options("./uncompress",
                             "Uncompresses files using Huffman Encoding");
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

    // Checks if the input is valid or the user wants help.
    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        return 0;
    }

    // Checks to see if we want true decoding.
    if (argv[FILE_OUTPUT_POSITION_ASCII] == NULL) {
        // Checks to see if the input file is empty.
        if (FileUtils::isEmptyFile(argv[1])) {
            ofstream ofOutput;
            ofOutput.open(argv[FILE_OUTPUT_POSITION], ios::trunc);
            ofOutput.close();
            return 0;
        }
        // Decompresses the file.
        trueDecompression(argv[1], argv[FILE_OUTPUT_POSITION]);
    } else {
        // Assumes user wants ascii decoding.
        string ofInputName = argv[FILE_INPUT_POSITION];
        string ofOutputName = argv[FILE_OUTPUT_POSITION_ASCII];
        // Checks if the input file is empty.
        if (FileUtils::isEmptyFile(ofInputName)) {
            ofstream ofOutput;
            ofOutput.open(ofOutputName, ios::trunc);
            ofOutput.close();
            return 0;
        }
        // Decompresses the file.
        pseudoDecompression(ofInputName, ofOutputName);
    }
    return 0;
}
