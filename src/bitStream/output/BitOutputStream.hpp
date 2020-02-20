/**
 * TODO: file header
 *
 * Author:
 */
#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <algorithm>
#include <iostream>

typedef unsigned char byte;

using namespace std;

/** TODO: class header */
class BitOutputStream {
  private:
    ostream& out;          // reference to the output stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits have been writen to buf

  public:
    /* TODO: add function header and implement */
    explicit BitOutputStream(ostream& os, unsigned int bufSize) : out(os) {
        this->bufSize = bufSize;
        this->buf = new char[bufSize];
        fill(this->buf, this->buf + this->bufSize, 0);
        this->nbits = 0;
    };

    /* TODO: add function header */
    void flush();

    /* TODO: add function header */
    void writeBit(unsigned int i);

    ~BitOutputStream();
};

#endif
