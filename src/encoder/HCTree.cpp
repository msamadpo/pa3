/**
 * TODO: file header
 *
 * Author:
 */
#include "HCTree.hpp"
#include <stack>

typedef std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp>
    node_heap;

HCTree::HCTree() {
    this->root = nullptr;
    this->leaves = vector<HCNode*>(256);
    auto newVec = new vector<string>(256);
    this->ofEncoding = newVec;
}

/* TODO */
HCTree::~HCTree() {
    auto currentNode = this->root;
    deleteAll(currentNode);
    this->leaves.clear();
    this->ofEncoding->clear();
    delete this->ofEncoding;
}

/* TODO */
void HCTree::build(const vector<unsigned int>& freqs) {
    auto heapOfNodes = node_heap();  // READ
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs.at(i) > 0) {
            unsigned char ofNewNode = i;
            auto nodeTree = new HCNode(freqs.at(i), ofNewNode);
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
        ofNode->p = parentNode;
        this->leaves.at(ofNode->symbol) = ofNode;
        this->root = parentNode;
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
        leftNode->p = parentNode;
        rightNode->p = parentNode;
        heapOfNodes.push(parentNode);
        if (leftNode->c0 == NULL && leftNode->c1 == NULL) {
            this->leaves.at(leftNode->symbol) = leftNode;
        }
        if (rightNode->c0 == NULL && rightNode->c1 == NULL) {
            this->leaves.at(rightNode->symbol) = rightNode;
        }
    }
    this->root = heapOfNodes.top();
    heapOfNodes.pop();
}

/* TODO */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    HCNode* ofLeaf = this->leaves.at(symbol);
    auto ofEncoding = new vector<unsigned int>();
    while (ofLeaf->p != NULL) {
        if (ofLeaf->p->c0 == ofLeaf) {
            ofEncoding->push_back(0);
        } else {
            ofEncoding->push_back(1);
        }
        ofLeaf = ofLeaf->p;
    }
    for (int i = ofEncoding->size() - 1; i >= 0; i++) {
        out.writeBit(ofEncoding->at(i));
    }
}

/* TODO */
void HCTree::encode(byte symbol, ostream& out) const {
    std::stack<unsigned char> ofSymbols;
    HCNode* ofLeaf = nullptr;
    ofLeaf = this->leaves.at(symbol);
    if (ofEncoding->at((unsigned int)symbol).empty()) {
        while (1) {
            auto parentOfLeaf = ofLeaf->p;
            if (parentOfLeaf != NULL) {
                if (ofLeaf == parentOfLeaf->c0) {
                    ofSymbols.push('0');
                } else {
                    ofSymbols.push('1');
                }
                ofLeaf = parentOfLeaf;
            } else {
                break;
            }
        }
        string ofCurrent = "";
        ofCurrent.resize(ofSymbols.size());
        int i = 0;
        while (ofSymbols.size() != 0) {
            ofCurrent.at(i) = ofSymbols.top();
            ofSymbols.pop();
            i++;
        }
        ofEncoding->at((unsigned int)symbol) = ofCurrent;
        out << ofEncoding->at((unsigned int)symbol);
    } else {
        out << ofEncoding->at((unsigned int)symbol);
    }
}

/* TODO */
byte HCTree::decode(BitInputStream& in) const {
    auto currentNode = this->root;
    while (1) {
        auto currentBit = in.readBit();
        if (currentNode->c0 == NULL && currentNode->c1 == NULL) {
            break;
        }
        if (currentBit == '0') {
            currentNode = currentNode->c0;
        } else {
            currentNode = currentNode->c1;
        }
    }
    return currentNode->symbol;
}

/* TODO */
byte HCTree::decode(istream& in) const {
    auto currNode = this->root;
    unsigned char ofCurrent;
    while (1) {
        if (currNode->c0 == NULL && currNode->c1 == NULL) {
            break;
        }
        ofCurrent = in.get();
        if (ofCurrent == '0') {
            currNode = currNode->c0;
        } else if (ofCurrent == '1') {
            currNode = currNode->c1;
        }
    }
    return currNode->symbol;
}

void HCTree::deleteAll(HCNode* root) {
    if (root == NULL) {
        return;
    } else {
        deleteAll(root->c0);
        deleteAll(root->c1);
    }
    delete root;
}
