/**
 * TODO: file header
 *
 * Author:
 */
#include "HCTree.hpp"

typedef std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp>
    node_heap;

HCTree::HCTree() {
    this->root = nullptr;
    this->leaves;
}

/* TODO */
HCTree::~HCTree() {}

/* TODO */
void HCTree::build(const vector<unsigned int>& freqs) {
    auto heapOfNodes = node_heap();  // READ
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs.at(i) > 0) {
            auto nodeTree = new HCNode(freqs.at(i), i);
            heapOfNodes.push(nodeTree);
        }
    }
    if (heapOfNodes.size() == 0) {
        return;
    }
    if (heapOfNodes.size() == 1) {
        auto ofNode = heapOfNodes.top();
        heapOfNodes.pop();
        auto parentNode = new HCNode(ofNode->count, ofNode->symbol);
        parentNode->c0 = ofNode;
        this->leaves.push_back(ofNode);
        return;
    }
    while (heapOfNodes.size() > 1) {
        auto leftNode = heapOfNodes.top();
        heapOfNodes.pop();
        auto rightNode = heapOfNodes.top();
        heapOfNodes.pop();
        unsigned int newFreq = leftNode->count + rightNode->count;
        auto parentNode = new HCNode(newFreq, leftNode->symbol);
        parentNode->c0 = leftNode;
        parentNode->c1 = rightNode;
        heapOfNodes.push(parentNode);
        if (leftNode->c0 == NULL && leftNode->c1 == NULL) {
            this->leaves.push_back(leftNode);
        }
        if (rightNode->c0 == NULL && rightNode->c1 == NULL) {
            this->leaves.push_back(rightNode);
        }
    }
    this->root = heapOfNodes.top();
    heapOfNodes.pop();
}

/* TODO */
// void HCTree::encode(byte symbol, BitOutputStream& out) const {}

/* TODO */
void HCTree::encode(byte symbol, ostream& out) const {
    auto listOfChars = new vector<char>();
    HCNode* ofLeaf;
    for (int i = 0; i < this->leaves.size(); i++) {
        if (this->leaves.at(i)->symbol == symbol) {
            ofLeaf = this->leaves.at(i);
            break;
        }
    }
    if (ofLeaf == NULL) {
        return;
    }
    while (1) {
        auto parentOfLeaf = ofLeaf->p;
        if (parentOfLeaf != NULL) {
            if (ofLeaf == parentOfLeaf->c0) {
                listOfChars->push_back('0');
            } else {
                listOfChars->push_back('1');
            }
            ofLeaf = parentOfLeaf;
        } else {
            break;
        }
    }
    if (listOfChars->size() == 0) {
        return;
    } else {
        for (int i = listOfChars->size() - 1; i >= 0; i--) {
            char atIndex = listOfChars->at(i);
            out.put(atIndex);
        }
    }
}

/* TODO */
// byte HCTree::decode(BitInputStream& in) const { return ' '; }

/* TODO */
byte HCTree::decode(istream& in) const { return ' '; }
