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
    this->leaves();
}

/* TODO */
HCTree::~HCTree() {}

/* TODO */
void HCTree::build(const vector<unsigned int>& freqs) {
    auto heapOfNodes = node_heap();  // READ
    for (int i = 0; i < freqs.size(), i++) {
        if (freqs.at(i) > 0) {
            auto nodeTree = new HCNode(freqs.at(i), i);
            heapOfNodes.push(nodeTree);
        }
    }
    if (node_heap.size() == 0) {
        return;
    }
    if (node_heap.size() == 1) {
        auto ofNode = heapOfNodes.top();
        heapOfNodes.pop();
        auto parentNode = new HCNode(ofNode->count, ofNode->symbol);
        parentNode->c0 = ofNode;
        this->leaves.push_back(ofNode);
        return;
    }
    while (node_heap.size() > 1) {
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
    // for(int i=0; i < this->leaves.size)
}

/* TODO */
// byte HCTree::decode(BitInputStream& in) const { return ' '; }

/* TODO */
byte HCTree::decode(istream& in) const { return ' '; }
