/**
 * TODO: file header
 *
 * Author:
 */
#include "BitInputStream.hpp"

/* TODO */
void BitInputStream::fill() { this->in.read(this->buf, this->bufSize); }

/* TODO */
bool BitInputStream::atEndOfFile() {
    if (eof()) {
        return false;
    }
    if (this->nbits == in.gcount() * 8 && (!this->in)) {
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
    if (this->nbits == 0 || this->nbits == this->bufSize * 8) {
        this->fill();
    }
    if (atEndOfFile()) {
        this->eofBit = true;
        return 0;
    } else if (this->nbits == this->bufSize * 8) {
        this->nbits = 0;
    }
    int indexOfArray = this->nbits / 8;
    int indexOfChar = (this->nbits + 1) % 8;
    if (this->nbits == (this->bufSize * 8) - 1) {
        unsigned char atIndex = this->buf[indexOfArray];
        atIndex = atIndex << (8 - 1);
        atIndex = atIndex >> (8 - 1);
        unsigned int ofIndex = atIndex && 1;
        this->nbits++;
        return ofIndex;
    } else {
        unsigned char atIndex = this->buf[indexOfArray];
        if (indexOfChar == 0) {
            atIndex = atIndex << (8 - 1);
            atIndex = atIndex >> (8 - 1);
            unsigned int ofIndex = atIndex && 1;
            this->nbits++;
            return ofIndex;
        }
        if (indexOfChar > 1) {
            atIndex = atIndex << (indexOfChar - 1);
            atIndex = atIndex >> (indexOfChar - 1);
        }
        atIndex = atIndex >> (8 - indexOfChar);
        unsigned int ofIndex = atIndex && 1;
        this->nbits++;
        return ofIndex;
    }
}

BitInputStream::~BitInputStream() { delete[] this->buf; }