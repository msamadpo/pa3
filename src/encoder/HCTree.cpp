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
    HCNode* ofLeaf = nullptr;
    ofLeaf = this->leaves.at((unsigned int)symbol);
    auto ofEncoding = vector<unsigned int>();
    while (ofLeaf->p != NULL) {
        if (ofLeaf->p->c0 == ofLeaf) {
            ofEncoding.push_back((unsigned int)0);
        } else {
            ofEncoding.push_back((unsigned int)1);
        }
        ofLeaf = ofLeaf->p;
    }
    for (int i = ofEncoding.size() - 1; i >= 0; i--) {
        out.writeBit(ofEncoding.at(i));
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
        if (currentNode->c0 == NULL && currentNode->c1 == NULL) {
            break;
        }
        auto currentBit = in.readBit();
        if (currentBit == 0) {
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

void HCTree::recursiveIteration(vector<char>* ofInfo, HCNode* ofCurrent) {
    if (ofCurrent == NULL) {
        ofCurrent = this->root;
    }
    unsigned char ofRules = 0;
    bool addLeft = false;
    bool addRight = false;
    if (ofCurrent->c1 != NULL) {
        if (ofCurrent->c1->c0 != NULL) {
            ofRules = ofRules + 0x04;
            recursiveIteration(ofInfo, ofCurrent->c1);
        } else {
            addRight = true;
            ofRules = ofRules + 0x01;
        }
    }
    if (ofCurrent->c0 != NULL) {
        if (ofCurrent->c0->c0 != NULL) {
            ofRules = ofRules + 0x08;
            recursiveIteration(ofInfo, ofCurrent->c0);
        } else {
            addLeft = true;
            ofRules = ofRules + 0x02;
        }
    }

    if (addRight) {
        ofInfo->push_back(ofCurrent->c1->symbol);
    }
    if (addLeft) {
        ofInfo->push_back(ofCurrent->c0->symbol);
    }
    ofInfo->push_back(ofRules);
}

void HCTree::recursiveBuild(vector<char>* ofHeader, HCNode* ofCurrent) {
    if (ofHeader->size() == 0) {
        return;
    }
    if (ofCurrent == NULL) {
        ofCurrent = new HCNode(0, 0);
        this->root = ofCurrent;
    }
    unsigned int ofRules = ofHeader->at(0);
    bool hasLeft;
    bool recurseLeft = false;
    bool recurseRight = false;
    ofHeader->erase(ofHeader->begin());
    if (ofRules >= 8) {
        ofRules -= 8;
        hasLeft = true;
        recurseLeft = true;
    }
    if (ofRules >= 4) {
        ofRules -= 4;
        recurseRight = true;
    }
    if (ofRules >= 2) {
        ofRules -= 2;
        hasLeft = true;
        ofCurrent->c0 = new HCNode(0, ofHeader->at(0));
        ofCurrent->c0->p = ofCurrent;
        ofHeader->erase(ofHeader->begin());
    }
    if (ofRules >= 1) {
        ofCurrent->c1 = new HCNode(0, ofHeader->at(0));
        ofCurrent->c1->p = ofCurrent;
        ofHeader->erase(ofHeader->begin());
    }

    if (recurseLeft) {
        HCNode* ofLeft = new HCNode(0, 0);
        ofLeft->p = ofCurrent;
        ofCurrent->c0 = ofLeft;
        recursiveBuild(ofHeader, ofLeft);
    }
    if (recurseRight) {
        HCNode* ofRight = new HCNode(0, 0);
        ofRight->p = ofCurrent;
        ofCurrent->c1 = ofRight;
        recursiveBuild(ofHeader, ofRight);
    }
    if (hasLeft) {
        ofCurrent->symbol = ofCurrent->c0->symbol;
    }
}
