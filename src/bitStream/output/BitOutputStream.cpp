/**
 * TODO: file header
 *
 * Author:
 */
#include "BitOutputStream.hpp"

/* TODO */
void BitOutputStream::flush() {
    if (this->nbits == 0) {
        return;
    }
    int lastIndexOfCell = this->nbits / 8;
    if (lastIndexOfCell == 0) {
        lastIndexOfCell++;
    }
    this->out.write(this->buf, lastIndexOfCell);
    fill(this->buf, this->buf + this->bufSize, 0);
    this->nbits = 0;
}

/* TODO */
void BitOutputStream::writeBit(unsigned int i) {
    if (i == 0) {
        this->nbits++;
        return;
    }
    if (this->nbits == (this->bufSize * 8)) {
        this->flush();
        this->nbits = 0;
    }
    int filledCells = this->nbits / 8;
    int indexInCell = (this->nbits + 1) % 8;
    unsigned char ofCell = this->buf[filledCells];
    if (indexInCell == 0) {
        ofCell = ofCell + 1;
    } else {
        unsigned int ofInput = i;
        ofInput = ofInput << 8 - indexInCell;
        ofCell = ofCell + ofInput;
    }
    this->buf[filledCells] = ofCell;
    this->nbits++;
}

BitOutputStream::~BitOutputStream() { delete[] this->buf; }
