/**
 * TODO: file header
 *
 * Author:
 */
#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"

using namespace std;

/** TODO: class header */
class HCTree {
  private:
    HCNode* root;            // the root of HCTree
    vector<HCNode*> leaves;  // a vector storing pointers to all leaf HCNodes
    vector<string>* ofEncoding;  // a vector storing the encodings of characters

    void deleteAll(HCNode* root);

    void recursiveBuild();

  public:
    /* TODO: add function header and implement */
    HCTree();

    /* TODO: add function header */
    ~HCTree();

    /* TODO: add function header */
    void build(const vector<unsigned int>& freqs);

    /* TODO: add function header */
    void encode(byte symbol, BitOutputStream& out) const;

    /* TODO: add function header */
    void encode(byte symbol, ostream& out) const;

    /* TODO: add function header */
    byte decode(BitInputStream& in) const;

    /* TODO: add function header */
    byte decode(istream& in) const;

    void recursiveIteration(vector<char>* ofInfo, HCNode* ofCurrent);

    void recursiveBuild(vector<char>* ofHeader, HCNode* ofCurrent);
};

#endif  // HCTREE_HPP
