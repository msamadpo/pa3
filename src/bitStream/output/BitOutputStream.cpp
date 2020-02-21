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
 *  bit buffer to which we can write data. Once
 *  the buffer has been completely filled with data,
 *  the program flushes said data to an output specified
 *  by the user.
 *
 */
#include "BitOutputStream.hpp"

const unsigned int SIZE_OF_CHAR = 8;
const unsigned int SET_FIRST_BIT = 0x01;

/*
 * This method writes all of the data that is found
 * in the buffer to the output stream. This method
 * is called when the buffer is full or when the
 * user want to manually write the data contained
 * in the buffer.
 *
 * Sources used:
 *  1) Stepik: Introduction to Data Structures (Fall 2016)
 *     by Moshiri and Izhikevich (available at stepik.org)
 *
 *     Section: 7.4 - Step 6
 *
 * Edge Case:
 *  1) The buffer is empty -> return;
 *
 */
void BitOutputStream::flush() {
    // If the buffer is empty, don't flush.
    if (this->nbits == 0) {
        return;
    }
    // If the buffer is full, flush all chars.
    if (this->nbits == this->bufSize * SIZE_OF_CHAR) {
        this->out.write(this->buf, this->bufSize);
    } else {
        // Get the char where nbit is.
        int lastIndexOfCell = this->nbits / SIZE_OF_CHAR;
        // Check if the whole char was filled, if so flush it.
        if (this->nbits % SIZE_OF_CHAR == 0) {
            this->out.write(this->buf, lastIndexOfCell);
        }
        // If the whole char was not filled, add padding.
        else {
            this->out.write(this->buf, lastIndexOfCell + 1);
        }
    }
    // Clear the buffer.
    fill(this->buf, this->buf + this->bufSize, 0);
    this->nbits = 0;
}

/*
 * This method writes an unsigned int into an unfilled bit
 * in the buffer. If the buffer is full, the method is first
 * going to flush the buffer and then write.
 *
 * Sources used:
 *  1) Stepik: Introduction to Data Structures (Fall 2016)
 *     by Moshiri and Izhikevich (available at stepik.org)
 *
 *     Section: 7.4 - Step 6
 *
 * Parameters:
 *  1) i - The data which we want the next empty bit to contain.
 *
 * Edge Case:
 *  1) The buffer is full -> flush before writting
 *
 */
void BitOutputStream::writeBit(unsigned int i) {
    // If we have read the whole buffer.
    if (this->nbits == (this->bufSize * SIZE_OF_CHAR)) {
        // Clear the buffer.
        this->flush();
        this->nbits = 0;
    }
    // If we want to add a zero.
    if ((unsigned int)i == 0) {
        this->nbits++;
        return;
    }
    // Pos of character where the nbit is located.
    int filledCells = this->nbits / SIZE_OF_CHAR;
    // Pos of the nbit inside the character.
    int indexInCell = (this->nbits + 1) % SIZE_OF_CHAR;
    // Get char where the nbit is.
    unsigned char ofCell = this->buf[filledCells];
    // If we are at the last bit in the char, add 1.
    if (indexInCell == 0) {
        ofCell = ofCell + SET_FIRST_BIT;
    } else {
        // Shift of input to postion of nbit.
        unsigned int ofInput = i;
        ofInput = ofInput << SIZE_OF_CHAR - indexInCell;
        // Add the desired bit at postion of nbit.
        ofCell = ofCell + ofInput;
    }
    // Add the new modified char at postion of nbit.
    this->buf[filledCells] = ofCell;
    this->nbits++;
}

/*
 * This method is a deconstructor. It frees the memory
 * used by the buffer by deleting the character array.
 */
BitOutputStream::~BitOutputStream() { delete[] this->buf; }
