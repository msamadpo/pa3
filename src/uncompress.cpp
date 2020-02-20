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

/* TODO: Pseudo decompression with ascii encoding and naive header (checkpoint)
 */
void pseudoDecompression(string inFileName, string outFileName) {
    vector<unsigned int> ofChars(256);
    HCTree* ofAllChars = new HCTree();
    ifstream ofInput;
    unsigned char nextChar;
    int nextByte;
    ofInput.open(inFileName, ios::binary);
    for (int i = 0; i < 256; i++) {
        ofInput >> ofChars.at(i);
    }
    ofAllChars->build(ofChars);
    ofstream ofOutput;
    ofOutput.open(outFileName, ios::trunc);
    ofInput.get();
    while ((nextByte = ofInput.peek()) != EOF) {
        ofOutput << ofAllChars->decode(ofInput);
    }
    ofInput.close();
    ofOutput.close();
    delete ofAllChars;
}

/* TODO: True decompression with bitwise i/o and small header (final) */
void trueDecompression(string inFileName, string outFileName) {
    vector<unsigned int> ofChars(256);
    HCTree* ofAllChars = new HCTree();
    ifstream ofInput;
    unsigned char nextChar;
    int nextByte;
    unsigned int totalChars = 0;
    ofInput.open(inFileName, ios::binary);
    for (int i = 0; i < 256; i++) {
        ofInput >> ofChars.at(i);
        totalChars += ofChars.at(i);
    }
    ofAllChars->build(ofChars);
    ofstream ofOutput;
    ofOutput.open(outFileName);
    ofInput.get();
    BitInputStream bs(ofInput, 4000);
    while (totalChars > 0) {
        ofOutput << ofAllChars->decode(bs);
        totalChars--;
        // ofOutput << ofAllChars->decode(ofInput);
    }
    ofInput.close();
    ofOutput.close();
    delete ofAllChars;
}

/* TODO: Main program that runs the uncompress */
int main(int argc, char* argv[]) {
    /*
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

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        return 0;
    }
    string ofInputFile = argv[2];
    string ofOutputFile = argv[3];
    if (FileUtils::isEmptyFile(ofInputFile)) {
        ofstream ofOutput;
        ofOutput.open(ofOutputFile, ios::trunc);
        ofOutput.close();
        return 0;
    }
    */
    trueDecompression(argv[1], argv[2]);
    return 0;
}
