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
 * Sources used:
 *  1) Stepik: Introduction to Data Structures (Fall 2016)
 *     by Moshiri and Izhikevich (available at stepik.org)
 *
 * Description of File:
 *  This file defines the neccessary methods to create a
 *  bit buffer to which we can read data from. Once
 *  all of the data has been read, we refill the buffer
 *  it possible.
 *
 */
#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <algorithm>
#include <iostream>

typedef unsigned char byte;

using namespace std;

/**
 * The BitInputStream class allows the user to create a
 * bit buffer in order to store data in its encoded form.
 * Each element of the data is represented by a sequence
 * of bits all of which make up different characters in
 * the buffer.
 *
 * Instance variables:
 *  1) in - the input stream to used to read in data
 *  2) buf - The character array to store the data
 *  3) bufSize - The size of the character array
 *  4) nbits - The position of the next unread bit.
 *  5) eofbit - True if the input stream is empty
 */
class BitInputStream {
  private:
    istream& in;           // reference to the input stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits that have been read from buf
    bool eofBit;           // true when trying to read past the end of the file

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
    bool atEndOfFile();

  public:
    /*
     * This is the constructor. It initializes all of the
     * instance variables to operate the buffer.
     * Once the buffer has been created it also zeroes
     * out all of the bits.
     *
     * Parameters:
     *  1) is - The input stream from which to read data from
     *  2) bufSize - The size we want the buffer to have.
     *
     */
    explicit BitInputStream(istream& is, unsigned int bufSize) : in(is) {
        this->bufSize = bufSize;
        this->buf = new char[bufSize];
        std::fill(this->buf, this->buf + bufSize, 0);
        this->nbits = 0;
        this->eofBit = false;
    };

    /*
     * This method read in data from the input stream and
     * stores it into the buffer. It reads characters
     * until the buffer has been filled or input stream
     * is empty.
     *
     * Sources used:
     *  1) Stepik: Introduction to Data Structures (Fall 2016)
     *     by Moshiri and Izhikevich (available at stepik.org)
     *
     *     Section: 7.4 - Step 6
     *
     */
    void fill();

    /*
     * This method return a boolean representing
     * whether the user has tried to read past
     * the buffer when no more data is allowed
     * to be inputed.
     *
     * Returns:
     *  1) bool - True if user is trying to read empty data.
     */
    bool eof();

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
    unsigned int readBit();

    /*
     * This is the deconstructor. It makes sure
     * to free the memory used by the buffer.
     */
    ~BitInputStream();
};

#endif