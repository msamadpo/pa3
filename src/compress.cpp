/**
 * TODO: file header
 *
 * Author:
 */
#include <fstream>
#include <iostream>

#include <algorithm>
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

/* TODO: add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
void pseudoCompression(string inFileName, string outFileName) {
    vector<unsigned int> ofChars(256);
    HCTree* ofAllChars = new HCTree();
    ifstream ofInput;
    unsigned char nextChar;
    int nextByte;
    ofInput.open(inFileName, ios::binary);
    while ((nextByte = ofInput.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        ofChars.at(nextChar)++;
    }
    ofInput.close();
    ofAllChars->build(ofChars);
    ofstream ofOutput;
    ofOutput.open(outFileName);
    for (int i = 0; i < ofChars.size(); i++) {
        unsigned int ofIndex = ofChars.at(i);
        ofOutput << ofIndex << endl;
    }
    ofInput.open(inFileName, ios::binary);
    while ((nextByte = ofInput.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        ofAllChars->encode(nextChar, ofOutput);
    }
    ofInput.close();
    ofOutput.close();
    delete ofAllChars;
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(string inFileName, string outFileName) {
    vector<unsigned int> ofChars(256);
    HCTree* ofAllChars = new HCTree();
    ifstream ofInput;
    unsigned char nextChar;
    int nextByte;
    unsigned int totalChars = 0;
    unsigned char newLineRules = 0;
    ofInput.open(inFileName, ios::binary);
    while ((nextByte = ofInput.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        ofChars.at(nextChar)++;
        totalChars++;
    }
    ofInput.close();
    ofstream ofOutput;
    ofOutput.open(outFileName);
    ofOutput << totalChars << endl;
    ofAllChars->build(ofChars);
    auto ofHeader = new vector<char>();
    ofAllChars->recursiveIteration(ofHeader, nullptr);
    std::reverse(ofHeader->begin(), ofHeader->end());
    for (int i = 0; i < ofHeader->size(); i++) {
        if (ofHeader->at(i) == '\n') {
            if (i == ofHeader->size() - 1) {
                newLineRules = newLineRules + 0x01;
            } else {
                newLineRules = newLineRules + 0x02;
            }
        }
    }
    ofOutput.put(newLineRules);
    ofOutput.put('\n');
    for (int i = 0; i < ofHeader->size(); i++) {
        ofOutput.put(ofHeader->at(i));
    }
    ofOutput.put('\n');
    ofInput.open(inFileName, ios::binary);
    BitOutputStream bos(ofOutput, 4000);
    while ((nextByte = ofInput.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        ofAllChars->encode(nextChar, bos);
    }
    bos.flush();
    ofInput.close();
    ofOutput.close();
    delete ofAllChars;
    delete ofHeader;
}

/* TODO: Main program that runs the compress */
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

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        return 0;
    }

    if (argv[3] == NULL) {
        if (FileUtils::isEmptyFile(argv[1])) {
            ofstream ofOutput;
            ofOutput.open(argv[2], ios::trunc);
            ofOutput.close();
            return 0;
        }
        trueCompression(argv[1], argv[2]);
    } else {
        string ofInputName = argv[2];
        string ofOutputName = argv[3];
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