/**
 * Authors: Matthew Lund and Mohammad Javid
 * Contact info: mtlund@ucsd.edu (Matthew),
 *               Msamadpo@ucsd.edu (Mohammad)
 *
 * Sources used:
 *  1) Stepik: Introduction to Data Structures (Fall 2016)
 *     by Moshiri and Izhikevich (available at stepik.org)
 *
 * Description of File:
 *  This file defines the neccessary methods to create a
 *  build a huffman tree. It also contains
 *  methods to encode and decode characters based on
 *  the tree structure and recursively outline
 *  the tree structure.
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

/**
 * The HCTree class allows the user to create a
 * huffman tree in order to store characters and their encoding.
 * Each character is considered to be a leaf node and the tree
 * does not allow one character to be multiple leaf nodes.
 *
 * Instance variables:
 *  1) root - The node representing the root
 *            of the  huffman tree.
 *  2) leaves - A vector storing all the leafs of the tree.
 *  3) ofEncoding - A vector containing encodings of characters.
 */
class HCTree {
  private:
    HCNode* root;            // the root of HCTree
    vector<HCNode*> leaves;  // a vector storing pointers to all leaf HCNodes
    vector<string>* ofEncoding;  // a vector storing the encodings of characters

    /*
     * This method deletes all of the nodes in the tree
     * recursively. It deallocates all of the memory
     * used by the tree.
     *
     * Parameters:
     *  1) root - The node we are currently at.
     *
     * Edge Case:
     *  1) The tree is empty -> return;
     *
     */
    void deleteAll(HCNode* root);

  public:
    /*
     * This is the constructor method for the HCTree.
     * It initializes all of the instance variables
     * and allocates space for the vectors.
     *
     * Edge Case:
     *  1) The frequency vector is all 0 -> return
     *
     */
    HCTree();

    /*
     * This is the deconstructor method for the HCTree.
     * It frees all of the memory used by the instance
     * variables and deletes all of the tree's nodes.
     *
     * Edge Case:
     *  1) The tree is empty -> don't try to delete nodes.
     *
     */
    ~HCTree();

    /*
     * This method creates a huffman tree based on the frequency
     * of each character. It utilizes a priority queue with its
     * own comparator. It first build by lowest frequency and
     * then by ascii value.
     *
     * Source: CSE 100 Lecture Slides
     *
     * Parameters:
     *  1) freqs - The vector containing the frequencies of each
     *             char.
     *
     * Edge Case:
     *  1) The frequency vector is empty -> return;
     *  2) The tree should only have one node ->
     *     don't use heap.
     *
     */
    void build(const vector<unsigned int>& freqs);

    /*
     * This method traverses through the tree from
     * bottom to top. While doing so it ouputs
     * '0' or '1' based on the location of each
     * node to represent its encoding. All of the
     * encodings are stored in a bit buffer.
     *
     * Source: CSE 100 Lecture Slides and PA outline.
     *
     * Parameters:
     *  1) symbol - The character that we want to encode.
     *  2) out - The bit buffer which we are writting to.
     *
     * Edge Case:
     *  1) The symbol does not exist -> return
     *
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /*
     * This method traverses through the tree from
     * bottom to top. While doing so it ouputs
     * '0' or '1' based on the location of each
     * node to represent its encoding. All of the
     * encodings are written to a output stream.
     *
     * Source: CSE 100 Lecture Slides and PA outline.
     *
     * Parameters:
     *  1) symbol - The character that we want to encode.
     *  2) out - The output stream which we are writting to.
     *
     * Edge Case:
     *  1) The symbol does not exist -> return
     *
     */
    void encode(byte symbol, ostream& out) const;

    /*
     * This method traverses through the tree from
     * top to bottom. It reads a series of bits
     * which tell the algorithm to go left or
     * right. Once it gits a leaf node it
     * outputs a character.
     *
     * Source: CSE 100 Lecture Slides and PA outline.
     *
     * Parameters:
     *  1) in - The bit buffer where the encoded
     *          bits are being read from.
     *
     * Edge Case:
     *  1) We hit a leaf node -> return the symbol of leaf.
     *  2) Tree is empty -> return.
     *
     * Returns:
     *  1) byte - The character of the leaf node.
     *
     */
    byte decode(BitInputStream& in) const;

    /*
     * This method traverses through the tree from
     * top to bottom. It reads a series of characters
     * which tell the algorithm to go left or
     * right. Once it gits a leaf node it
     * outputs a character.
     *
     * Source: CSE 100 Lecture Slides and PA outline.
     *
     * Parameters:
     *  1) in - The input stream where the encoded
     *          characters are being read from.
     *
     * Edge Case:
     *  1) We hit a leaf node -> return the symbol of leaf.
     *  2) Tree is empty -> return.
     *
     * Returns:
     *  1) byte - The character of the leaf node.
     *
     */
    byte decode(istream& in) const;

    /*
     * This creates a sequence of characters based on the structure
     * of the tree. It does so by preforming a pre-order iteration
     * in reverse order.
     *
     * Source: TA Office hours
     *
     * Parameters:
     *  1) ofInfo - The vector containing all of the characters.
     *  2) ofCurrent - The node where the method is currently at.
     *
     * Edge Case:
     *  1) The tree is empty -> don't modify the vector.
     *  2) We hit null nodes -> return, don't modify the vector.
     *
     */
    void recursiveIteration(vector<char>* ofInfo, HCNode* ofCurrent);

    /*
     * This method builds the huffman tree recursively
     * based on a sequency of characters. It uses
     * a set of rules to recurse throughout the tree.
     *
     * Source: TA Office hours
     *
     * Parameters:
     *  1) ofInfo - The vector containing all of the characters.
     *  2) ofCurrent - The node where the method is currently at.
     *
     * Edge Case:
     *  1) The vector containing all of the characters is empty ->
     *     return.
     *  2) If we hit null nodes -> return.
     *
     */
    void recursiveBuild(vector<char>* ofHeader, HCNode* ofCurrent);
};

#endif  // HCTREE_HPP
