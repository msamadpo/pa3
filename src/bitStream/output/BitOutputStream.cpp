/**
 * TODO: file header
 *
 * Author:
 */
#include "BitOutputStream.hpp"

/* TODO */
void BitOutputStream::flush() {
    int lastIndexOfCell = this->nbits / 8;
    this->out.write(this->buf, lastIndexOfCell + 1);
    fill(this->buf, this->buf + this->bufSize, 0);
}

/* TODO */
void BitOutputStream::writeBit(unsigned int i) {
    if (i == 0) {
        this->nbits++;
        return;
    }
    if (this->nbits == (this->bufSize * 8)) {
        this->flush();
    }
    int filledCells = this->nbits / 8;
    int indexInCell = (this->nbits + 1) % 8;
    char ofCell = this->buf[filledCells];
    if (indexInCell == 0) {
        ofCell = ofCell + 1;
    } else {
        unsigned int ofInput = i;
        ofInput << 8 - indexInCell;
        ofCell = ofCell + ofInput;
    }
    this->nbits++;
}

BitOutputStream::~BitOutputStream() { delete[] this->buf; }
