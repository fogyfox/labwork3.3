#include <iostream>
#include <algorithm>
#include "fullBinaryTree.h"

using namespace std;

FullBinaryTree::FullBinaryTree() : root(nullptr) {}

FullBinaryTree::~FullBinaryTree() { 
    clear(root); 
}

NodeFBT* FullBinaryTree::insertNode(NodeFBT* node, int key) {
    if (!node) return new NodeFBT(key);
    if (key < node->key)
        node->left = insertNode(node->left, key);
    else
        node->right = insertNode(node->right, key);
    return node;
}

NodeFBT* FullBinaryTree::insertTree(NodeFBT* targetRoot, NodeFBT* sourceNode) {
    if (!sourceNode) return targetRoot;
    targetRoot = insertNode(targetRoot, sourceNode->key);
    targetRoot = insertTree(targetRoot, sourceNode->left);
    targetRoot = insertTree(targetRoot, sourceNode->right);
    return targetRoot;
}

bool FullBinaryTree::isFull(NodeFBT* node) const {
    if (!node) return true;
    if (!node->left && !node->right) return true;
    if (node->left && node->right)
        return isFull(node->left) && isFull(node->right);
    return false;
}

void FullBinaryTree::topToDown(NodeFBT* node) const {
    if (!node) return;
    cout << node->key << " ";
    topToDown(node->left);
    topToDown(node->right);
}

void FullBinaryTree::leftToRight(NodeFBT* node) const {
    if (!node) return;
    leftToRight(node->left);
    cout << node->key << " ";
    leftToRight(node->right);
}

void FullBinaryTree::downToTop(NodeFBT* node) const {
    if (!node) return;
    downToTop(node->left);
    downToTop(node->right);
    cout << node->key << " ";
}

int FullBinaryTree::height(NodeFBT* node) const {
    if (!node) return 0;
    return max(height(node->left), height(node->right)) + 1;
}

void FullBinaryTree::printLevel(NodeFBT* node, int level) const {
    if (!node) return;
    if (level == 1)
        cout << node->key << " ";
    else {
        printLevel(node->left, level - 1);
        printLevel(node->right, level - 1);
    }
}

bool FullBinaryTree::searchTree(NodeFBT* node, int value) const {
    if (!node) return false;
    if (node->key == value) return true;
    if (value < node->key)
        return searchTree(node->left, value);
    else
        return searchTree(node->right, value);
}

void FullBinaryTree::clear(NodeFBT* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

void FullBinaryTree::insert(int key) { root = insertNode(root, key); }

void FullBinaryTree::insertSubtree(const FullBinaryTree& other) {
    root = insertTree(root, other.root);
}

bool FullBinaryTree::checkFull() const { return isFull(root); }

void FullBinaryTree::printTopToDown() const { topToDown(root); cout << endl; }

void FullBinaryTree::printLeftToRight() const { leftToRight(root); cout << endl; }

void FullBinaryTree::printDownToTop() const { downToTop(root); cout << endl; }

int FullBinaryTree::getHeight() const { return height(root); }

void FullBinaryTree::printLevelOrder() const {
    int h = getHeight();
    for (int i = 1; i <= h; i++) {
        printLevel(root, i);
    }
    cout << endl;
}

bool FullBinaryTree::exists(int value) const { return searchTree(root, value); }