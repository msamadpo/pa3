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
    ofOutput.open(outFileName);
    while ((nextByte = ofInput.peek()) != EOF) {
        ofOutput << ofAllChars->decode(ofInput);
    }
    ofInput.close();
    ofOutput.close();
    delete ofAllChars;
}

/* TODO: True decompression with bitwise i/o and small header (final) */
void trueDecompression(string inFileName, string outFileName) {}

/* TODO: Main program that runs the uncompress */
int main(int argc, char* argv[]) {
    string ofInputFile = argv[2];
    string ofOutputFile = argv[3];
    pseudoDecompression(ofInputFile, ofOutputFile);
    return 0;
}
