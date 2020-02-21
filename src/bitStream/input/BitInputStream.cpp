/**
 * Authors: Matthew Lund and Mohammad Javid
 * Contact info: mtlund@ucsd.edu (Matthew),
 *               Msamadpo@ucsd.edu (Mohammad)
 *
 * Sources used:
 *  1) Stepik: Introduction to Data Structures (Fall 2016)
 *     by Moshiri and Izhikevich (available at stepik.org)
 *
 *     Section: 7.4 - Step 6
 *
 * Description of File:
 *  This file defines the neccessary methods to create a
 *  bit buffer to which we can read data from. Once
 *  all of the data has been read, we refill the buffer
 *  it possible.
 *
 */
#include "BitInputStream.hpp"

const unsigned int SIZE_OF_CHAR = 8;

/*
 * This method returns true whenever the buffer is full and
 * has read all the data or when the input file does not
 * have any more input.
 *
 * Returns:
 *  1) bool - True if no more input or all input
 *            has been read.
 *
 */
bool BitInputStream::atEndOfFile() {
    if (eof()) {
        return false;
    }

    if (this->nbits == in.gcount() * SIZE_OF_CHAR && (!this->in)) {
        return true;
    } else if (this->in.gcount() == 0 && (!this->in)) {
        return true;
    } else {
        if (this->nbits == this->bufSize * SIZE_OF_CHAR) {
            this->nbits = 0;
        }
        return false;
    }
}

/*
 * This method read in data from the input stream and
 * stores it into the buffer. It reads characters
 * until the buffer has been filled or input stream
 * is empty.
 *
 * Sources used:
 *  1) Stepik: Introduction to Data Structures (Fall 2016)
 *     by Moshiri and Izhikevich (available at stepik.org)
 *     Section: 7.4 - Step 6
 *
 */
void BitInputStream::fill() { this->in.read(this->buf, this->bufSize); }

/*
 * This method return a boolean representing
 * whether the user has tried to read past
 * the buffer when no more data is allowed
 * to be inputed.
 *
 * Returns:
 *  1) bool - True if user is trying to read empty data.
 */
bool BitInputStream::eof() { return eofBit; }

/*
 * This method returns an int representing
 * the bit that has not yet been read.
 * If the buffer has been completely read,
 * the method tries to refill the buffer.
 *
 * Sources used:
 *  1) Stepik: Introduction to Data Structures (Fall 2016)
 *     by Moshiri and Izhikevich (available at stepik.org)
 *
 *     Section: 7.4 - Step 6
 *
 * Edge Cases:
 *  1) The buffer has been read -> try to read in data.
 *  2) The buffer is empty -> try to read in data.
 *
 * Retruns:
 *  1) int - representing the bit at postion nbit.
 */
unsigned int BitInputStream::readBit() {
    if (this->nbits == 0 || this->nbits == this->bufSize * SIZE_OF_CHAR) {
        this->fill();
    }
    if (atEndOfFile()) {
        this->eofBit = true;
        return 0;
    }
    int indexOfArray = this->nbits / SIZE_OF_CHAR;
    int indexOfChar = (this->nbits + 1) % SIZE_OF_CHAR;
    if (this->nbits == (this->bufSize * SIZE_OF_CHAR) - 1) {
        unsigned char atIndex = this->buf[indexOfArray];
        atIndex = atIndex << (SIZE_OF_CHAR - 1);
        atIndex = atIndex >> (SIZE_OF_CHAR - 1);
        unsigned int ofIndex = atIndex && 1;
        this->nbits++;
        return ofIndex;
    } else {
        unsigned char atIndex = this->buf[indexOfArray];
        if (indexOfChar == 0) {
            atIndex = atIndex << (SIZE_OF_CHAR - 1);
            atIndex = atIndex >> (SIZE_OF_CHAR - 1);
            unsigned int ofIndex = atIndex && 1;
            this->nbits++;
            return ofIndex;
        }
        if (indexOfChar > 1) {
            atIndex = atIndex << (indexOfChar - 1);
            atIndex = atIndex >> (indexOfChar - 1);
        }
        atIndex = atIndex >> (SIZE_OF_CHAR - indexOfChar);
        unsigned int ofIndex = atIndex && 1;
        this->nbits++;
        return ofIndex;
    }
}

/*
 * This is the deconstructor. It makes sure
 * to free the memory used by the buffer.
 */
BitInputStream::~BitInputStream() { delete[] this->buf; }