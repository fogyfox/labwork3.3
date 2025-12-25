#ifndef FULLBINARYTREE_H
#define FULLBINARYTREE_H

// Структура узла дерева
struct NodeFBT {
    int key;
    NodeFBT* left;
    NodeFBT* right;
    NodeFBT(int k) : key(k), left(nullptr), right(nullptr) {}
};

class FullBinaryTree {
private:
    NodeFBT* root;

    // Вспомогательные рекурсивные функции
    NodeFBT* insertNode(NodeFBT* node, int key);
    NodeFBT* insertTree(NodeFBT* targetRoot, NodeFBT* sourceNode);
    bool isFull(NodeFBT* node) const;
    void topToDown(NodeFBT* node) const;
    void leftToRight(NodeFBT* node) const;
    void downToTop(NodeFBT* node) const;
    int height(NodeFBT* node) const;
    void printLevel(NodeFBT* node, int level) const;
    bool searchTree(NodeFBT* node, int value) const;
    void clear(NodeFBT* node);

public:
    FullBinaryTree();
    ~FullBinaryTree();

    void insert(int key);
    void insertSubtree(const FullBinaryTree& other);
    bool checkFull() const;
    void printTopToDown() const;
    void printLeftToRight() const;
    void printDownToTop() const;
    int getHeight() const;
    void printLevelOrder() const;
    bool exists(int value) const;
};

#endif