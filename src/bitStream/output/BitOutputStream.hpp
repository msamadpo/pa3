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
#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <algorithm>
#include <iostream>

typedef unsigned char byte;

using namespace std;

/**
 * The BitOutputStream class allows the user to create a
 * buffer in order to store data. Once the buffer has been
 * filled, the buffer outputs all of its stored data into
 * the output stream. The class also provides a method
 * for the user to manually flush the buffer.
 *
 * Instance variables:
 *  1) out - The outstream where we are writting the data
 *  2) buf - The array that stores the data
 *  3) bufSize - The size of the buffer which stores the data
 *  4) nbits - The location of the next empty bit position.
 */
class BitOutputStream {
  private:
    ostream& out;          // reference to the output stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits have been writen to buf

  public:
    /*
     * This method creates an the BitOutputStream.
     * It initializes all of the instance variables,
     * allocates space for the buffer and once the buffer
     * has been allocated, it zeroes out all of the data.
     *
     * Parameters:
     *  1) os - The outstream to which to write the data
     *  2) bufSize - The desired size of our buffer.
     */
    explicit BitOutputStream(ostream& os, unsigned int bufSize) : out(os) {
        this->bufSize = bufSize;
        this->buf = new char[bufSize];
        fill(this->buf, this->buf + this->bufSize, 0);
        this->nbits = 0;
    };

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
    void flush();

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
    void writeBit(unsigned int i);

    /*
     * This method is a deconstructor. It frees the memory
     * used by the buffer by deleting the character array.
     */
    ~BitOutputStream();
};

#endif
