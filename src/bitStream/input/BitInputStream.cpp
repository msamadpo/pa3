/**
 * TODO: file header
 *
 * Author:
 */
#include "BitInputStream.hpp"

/* TODO */
void BitInputStream::fill() {
    if (eof()) {
        return;
    }
    this->in.read(this->buf, this->bufSize);
    if (atEndOfFile()) {
        this->eofBit = true;
    }
}

/* TODO */
bool BitInputStream::atEndOfFile() {
    if (eof()) {
        return false;
    }
    if (this->nbits == this->bufSize * 8 && (!this->in)) {
        return true;
    } else if (this->in.gcount() == 0 && (!this->in)) {
        return true;
    } else {
        return false;
    }
}

/* Returns true when trying to read past the end of the file, false otherwise */
bool BitInputStream::eof() { return eofBit; }

/* TODO */
unsigned int BitInputStream::readBit() {
    int indexOfArray = this->nbits / 8;
    int indexOfChar = (this->nbits + 1) % 8;
    if (this->nbits == (this->bufSize * 8) - 1) {
        char atIndex = this->buf[indexOfArray];
        unsigned int ofIndex = atIndex && 1;
        this->nbits++;
        this->fill();
        return ofIndex;
    } else {
        char atIndex = this->buf[indexOfArray];
        atIndex = atIndex >> (8 - indexOfChar);
        unsigned int ofIndex = atIndex && 1;
        this->nbits++;
        return ofIndex;
    }
}