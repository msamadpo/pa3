/**
 * TODO: file header
 *
 * Author:
 */
#include <fstream>
#include <iostream>

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
    if (ofInput.peek() == EOF) {
        ofInput.close();
        ofstream ofOutput;
        ofOutput.open(outFileName);
        ofOutput.close();
        delete ofAllChars;
        return;
    }
    while ((nextByte = ofInput.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        ofChars.at(nextChar)++;
    }
    ofInput.close();
    ofAllChars->build(ofChars);
    ofstream ofOutput;
    ofOutput.open(outFileName, ios::trunc);
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
void trueCompression(string inFileName, string outFileName) {}

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

    string ofInputName = argv[2];
    string ofOutputName = argv[3];
    if (FileUtils::isEmptyFile(ofInputName)) {
        ofstream ofOutput;
        ofOutput.open(ofOutputName, ios::trunc);
        ofOutput.close();
        return 0;
    }
    pseudoCompression(ofInputName, ofOutputName);
    return 0;
}
