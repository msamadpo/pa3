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
#include "HCTree.hpp"
#include <stack>

const unsigned int SET_FOURTH_BIT = 0x08;
const unsigned int SET_THIRD_BIT = 0x04;
const unsigned int SET_SECOND_BIT = 0x02;
const unsigned int SET_FIRST_BIT = 0x01;
const unsigned int VALUE_OF_FOURTH_BIT = 8;
const unsigned int VALUE_OF_THIRD_BIT = 4;
const unsigned int VALUE_OF_SECOND_BIT = 2;
const unsigned int VALUE_OF_FIRST_BIT = 1;
const unsigned int NUMBER_OF_CHARACTERS = 256;

/* Defines a priority queue with our custom constructor */
typedef std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp>
    node_heap;

/*
 * This is the constructor method for the HCTree.
 * It initializes all of the instance variables
 * and allocates space for the vectors.
 *
 * Edge Case:
 *  1) The frequency vector is all 0 -> return
 *
 */
HCTree::HCTree() {
    this->root = nullptr;
    this->leaves = vector<HCNode*>(NUMBER_OF_CHARACTERS);
    auto newVec = new vector<string>(NUMBER_OF_CHARACTERS);
    this->ofEncoding = newVec;
}

/*
 * This is the deconstructor method for the HCTree.
 * It frees all of the memory used by the instance
 * variables and deletes all of the tree's nodes.
 *
 * Edge Case:
 *  1) The tree is empty -> don't try to delete nodes.
 *
 */
HCTree::~HCTree() {
    auto currentNode = this->root;
    deleteAll(currentNode);
    this->leaves.clear();
    this->ofEncoding->clear();
    delete this->ofEncoding;
}

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
void HCTree::build(const vector<unsigned int>& freqs) {
    // Makes a heap.
    auto heapOfNodes = node_heap();
    // Loops through all characters to find their frequency.
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs.at(i) > 0) {
            // Adds character to heap if it appears more than once.
            unsigned char ofNewNode = i;
            auto nodeTree = new HCNode(freqs.at(i), ofNewNode);
            heapOfNodes.push(nodeTree);
        }
    }
    if (heapOfNodes.size() == 0) {
        return;
    }
    // If only one element is in the heap.
    if (heapOfNodes.size() == 1) {
        // Gets the element in the heap.
        auto ofNode = heapOfNodes.top();
        heapOfNodes.pop();
        /*
        / Sets the element to be a leaf node, and makes a tree
        / out of it.
        */
        auto parentNode = new HCNode(ofNode->count, ofNode->symbol);
        parentNode->c0 = ofNode;
        ofNode->p = parentNode;
        this->leaves.at(ofNode->symbol) = ofNode;
        this->root = parentNode;
        return;
    }
    // Loops till all of the subtrees have been built.
    while (heapOfNodes.size() > 1) {
        // Gets the left node and right node of parent.
        auto leftNode = heapOfNodes.top();
        heapOfNodes.pop();
        auto rightNode = heapOfNodes.top();
        heapOfNodes.pop();
        // Stores new frequency of parent.
        unsigned int newFreq = leftNode->count + rightNode->count;
        auto parentNode = new HCNode(newFreq, leftNode->symbol);
        // Adds the left and right node to the new parent node.
        parentNode->c0 = leftNode;
        parentNode->c1 = rightNode;
        leftNode->p = parentNode;
        rightNode->p = parentNode;
        // Pushes the subtree that was created.
        heapOfNodes.push(parentNode);
        // Checks if the left and right child are leafs to add them.
        if (leftNode->c0 == NULL && leftNode->c1 == NULL) {
            this->leaves.at(leftNode->symbol) = leftNode;
        }
        if (rightNode->c0 == NULL && rightNode->c1 == NULL) {
            this->leaves.at(rightNode->symbol) = rightNode;
        }
    }
    // Makes the root point to the start of the tree.
    this->root = heapOfNodes.top();
    // Empty the heap.
    heapOfNodes.pop();
}

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
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    // Starts at the leaf node.
    HCNode* ofLeaf = nullptr;
    ofLeaf = this->leaves.at((unsigned int)symbol);
    // Makes a vector holding the encoding of the char.
    auto ofEncoding = vector<unsigned int>();
    // Loops till it hits the root of the tree.
    while (ofLeaf->p != NULL) {
        // If the current node is a left child.
        if (ofLeaf->p->c0 == ofLeaf) {
            // Adds a 0.
            ofEncoding.push_back((unsigned int)0);
        }
        // Current node is a right child.
        else {
            // Adds a 1.
            ofEncoding.push_back((unsigned int)1);
        }
        // Moves one node up at a time.
        ofLeaf = ofLeaf->p;
    }
    // Reverses the bits to get true encoding.
    for (int i = ofEncoding.size() - 1; i >= 0; i--) {
        out.writeBit(ofEncoding.at(i));
    }
}

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
void HCTree::encode(byte symbol, ostream& out) const {
    // Uses a stack to represent encoding chars.
    std::stack<unsigned char> ofSymbols;
    // Starts at the leaf node.
    HCNode* ofLeaf = nullptr;
    ofLeaf = this->leaves.at(symbol);
    // Checks to see if the char has already been encoded.
    if (ofEncoding->at((unsigned int)symbol).empty()) {
        /*
        / Use loop to finds its encoding by moving
        / up the tree.
        */
        while (1) {
            auto parentOfLeaf = ofLeaf->p;
            // Checks to see if we are at the root.
            if (parentOfLeaf != NULL) {
                // Checks to see if the current node is a left child.
                if (ofLeaf == parentOfLeaf->c0) {
                    ofSymbols.push('0');
                } else {
                    ofSymbols.push('1');
                }
                // Moves up a node.
                ofLeaf = parentOfLeaf;
            } else {
                break;
            }
        }
        // String to store the encoding.
        string ofCurrent = "";
        ofCurrent.resize(ofSymbols.size());
        int i = 0;
        // Pops elements in stack to get encoding in order.
        while (ofSymbols.size() != 0) {
            ofCurrent.at(i) = ofSymbols.top();
            ofSymbols.pop();
            i++;
        }
        // Adds the encoding to a vector, works like a map.
        ofEncoding->at((unsigned int)symbol) = ofCurrent;
        // Outputs the encoding of the character.
        out << ofEncoding->at((unsigned int)symbol);
    } else {
        // If encoding already exists, it finds it to output.
        out << ofEncoding->at((unsigned int)symbol);
    }
}

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
byte HCTree::decode(BitInputStream& in) const {
    // Starts at the root of the tree.
    auto currentNode = this->root;
    // Iterates till it hits a leaf node.
    while (1) {
        // Hit leaf node.
        if (currentNode->c0 == NULL && currentNode->c1 == NULL) {
            break;
        }
        // Reads a bit
        auto currentBit = in.readBit();
        // Goes to left node is bit is 0, right node if bit is 1.
        if (currentBit == 0) {
            currentNode = currentNode->c0;
        } else {
            currentNode = currentNode->c1;
        }
    }
    // Returns the character of the leaf node.
    return currentNode->symbol;
}

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
byte HCTree::decode(istream& in) const {
    // Starts at the root.
    auto currNode = this->root;
    unsigned char ofCurrent;
    // Loops till it hits a leaf node.
    while (1) {
        // Check is a leaf node has been hit.
        if (currNode->c0 == NULL && currNode->c1 == NULL) {
            break;
        }
        ofCurrent = in.get();
        // If character is a 0 we move left, if 1 we move right.
        if (ofCurrent == '0') {
            currNode = currNode->c0;
        } else if (ofCurrent == '1') {
            currNode = currNode->c1;
        }
    }
    // Returns the character of the leaf node hit.
    return currNode->symbol;
}

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
void HCTree::deleteAll(HCNode* root) {
    // Checks if we are outside the bounds of the tree.
    if (root == NULL) {
        return;
    } else {
        // Recursively deletes the left and right.
        deleteAll(root->c0);
        deleteAll(root->c1);
    }
    // Deletes the current node.
    delete root;
}

/*
 * This creates a sequence of characters based on the structure
 * of the tree. It does so by preforming a pre-order iteration
 * in reverse order.
 *
 * Source: TA Office hours, Justin T helped me
 *         with my implementation (2/18/20)
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
void HCTree::recursiveIteration(vector<char>* ofInfo, HCNode* ofCurrent) {
    // Checks if we are at the begging of the iteration.
    if (ofCurrent == NULL) {
        ofCurrent = this->root;
    }
    // Uses char to represent how to recurse.
    unsigned char ofRules = 0;
    // True if left is leaf node.
    bool addLeft = false;
    // True if right is a leaf node.
    bool addRight = false;

    // Recurses to the right if possible.
    if (ofCurrent->c1 != NULL) {
        // Checks if right node is its own subtree.
        if (ofCurrent->c1->c0 != NULL) {
            // Sets bit that marks the right node as subtree.
            ofRules = ofRules + SET_THIRD_BIT;
            // Recurses to the right.
            recursiveIteration(ofInfo, ofCurrent->c1);
        } else {
            addRight = true;
            // Sets bit that marks the right node as a leaf.
            ofRules = ofRules + SET_FIRST_BIT;
        }
    }
    // Recurses to the left if possible.
    if (ofCurrent->c0 != NULL) {
        // Checks if left node is its own subtree.
        if (ofCurrent->c0->c0 != NULL) {
            // Sets bit that marks the left node as a subtree.
            ofRules = ofRules + SET_FOURTH_BIT;
            // Recurses left.
            recursiveIteration(ofInfo, ofCurrent->c0);
        } else {
            addLeft = true;
            // Sets bit that marks the left node as a leaf.
            ofRules = ofRules + SET_SECOND_BIT;
        }
    }

    // Checks if right node was a leaf.
    if (addRight) {
        // Adds leaf node to the tree representation.
        ofInfo->push_back(ofCurrent->c1->symbol);
    }
    // Checks if left node was a leaf.
    if (addLeft) {
        // Adds leaf node to the tree representation.
        ofInfo->push_back(ofCurrent->c0->symbol);
    }
    // Pushes the character containing the rules of recursion.
    ofInfo->push_back(ofRules);
}

/*
 * This method builds the huffman tree recursively
 * based on a sequency of characters. It uses
 * a set of rules to recurse throughout the tree.
 *
 * Source: TA Office hours, Justin T helped me
 *         with my implementation (2/18/20)
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
void HCTree::recursiveBuild(vector<char>* ofHeader, HCNode* ofCurrent) {
    // Check if the tree has been fully built.
    if (ofHeader->size() == 0) {
        return;
    }
    // Checks if recursion has just begun.
    if (ofCurrent == NULL) {
        // Makes the root a parent node.
        ofCurrent = new HCNode(0, 0);
        this->root = ofCurrent;
    }
    // Gets the character containing the rules of recursion.
    unsigned int ofRules = ofHeader->at(0);
    // True if left node exists.
    bool hasLeft;
    // True if tree has left subtree.
    bool recurseLeft = false;
    // True if tree has right subtree.
    bool recurseRight = false;
    // Removes rules character, we already pulled it.
    ofHeader->erase(ofHeader->begin());
    // Checks if 4th bit has been set. (Has left subtree)
    if (ofRules >= VALUE_OF_FOURTH_BIT) {
        ofRules -= VALUE_OF_FOURTH_BIT;
        // Makes sure to recurse left, marks left node as not null.
        hasLeft = true;
        recurseLeft = true;
    }
    // Checks if 3rd bit has been set. (Has right subtree)
    if (ofRules >= VALUE_OF_THIRD_BIT) {
        ofRules -= VALUE_OF_THIRD_BIT;
        // Sets boolean to recurse right.
        recurseRight = true;
    }
    // Checks 2nd bit has been set. (Left node is a leaf)
    if (ofRules >= VALUE_OF_SECOND_BIT) {
        ofRules -= VALUE_OF_SECOND_BIT;
        // Marks left node as leaf.
        hasLeft = true;
        // Makes a left node in the current tree, adds its symbol.
        ofCurrent->c0 = new HCNode(0, ofHeader->at(0));
        ofCurrent->c0->p = ofCurrent;
        // Removes the character, the node was created.
        ofHeader->erase(ofHeader->begin());
    }
    // Checks if 1st bit has been set. (Right node is a leaf)
    if (ofRules >= VALUE_OF_FIRST_BIT) {
        // Makes a right node in the current tree, adds its symbol.
        ofCurrent->c1 = new HCNode(0, ofHeader->at(0));
        ofCurrent->c1->p = ofCurrent;
        // Removes the character, the node was created.
        ofHeader->erase(ofHeader->begin());
    }

    // Checks if we should recurse left based on rules.
    if (recurseLeft) {
        // Makes the left node a parent of subtree.
        HCNode* ofLeft = new HCNode(0, 0);
        ofLeft->p = ofCurrent;
        ofCurrent->c0 = ofLeft;
        // Recurses left.
        recursiveBuild(ofHeader, ofLeft);
    }
    // Checks if we should recurse right based on rules.
    if (recurseRight) {
        // Makes the right node a parent of subtree.
        HCNode* ofRight = new HCNode(0, 0);
        ofRight->p = ofCurrent;
        ofCurrent->c1 = ofRight;
        // Recurses right.
        recursiveBuild(ofHeader, ofRight);
    }
    // Sets the symbol of the parent based on the left node.
    if (hasLeft) {
        ofCurrent->symbol = ofCurrent->c0->symbol;
    }
}
