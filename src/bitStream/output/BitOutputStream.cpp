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
    if (this->nbits == this->bufSize * 8) {
        this->out.write(this->buf, this->bufSize);
    } else {
        int lastIndexOfCell = this->nbits / 8;
        if (this->nbits % 8 == 0) {
            this->out.write(this->buf, lastIndexOfCell);
        } else {
            this->out.write(this->buf, lastIndexOfCell + 1);
        }
    }
    fill(this->buf, this->buf + this->bufSize, 0);
    this->nbits = 0;
}

/* TODO */
void BitOutputStream::writeBit(unsigned int i) {
    if (this->nbits == (this->bufSize * 8)) {
        this->flush();
        this->nbits = 0;
    }
    if ((unsigned int)i == 0) {
        this->nbits++;
        return;
    }
    int filledCells = this->nbits / 8;
    int indexInCell = (this->nbits + 1) % 8;
    unsigned char ofCell = this->buf[filledCells];
    if (indexInCell == 0) {
        ofCell = ofCell + 0x01;
    } else {
        unsigned int ofInput = i;
        ofInput = ofInput << 8 - indexInCell;
        ofCell = ofCell + ofInput;
    }
    this->buf[filledCells] = ofCell;
    this->nbits++;
}

BitOutputStream::~BitOutputStream() { delete[] this->buf; }
