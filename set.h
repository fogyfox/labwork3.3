#ifndef SET_H
#define SET_H

#include <vector>
#include <string>

class Set {
private:
    struct NodeSet {
        int key;
        NodeSet* next;
    };

    NodeSet** buckets;
    int tableSize;
    int itemCount;

    // Внутренние вспомогательные методы
    int hashFunction(int key) const;
    void rehashIfNeeded();

public:
    // Конструктор и деструктор
    Set(int initialSize = 101);
    ~Set();

    // Базовые операции
    bool insert(int key);
    bool contains(int key) const;
    bool remove(int key);
    int size() const;
    bool isEmpty() const;
    void clear();

    // Вспомогательные функции
    void print() const;

    // Работа с векторами и алгоритмы
    std::vector<int> toVector() const;
    static Set fromVector(const std::vector<int>& vec);
    bool partition(int subsetSum, std::vector<std::vector<int>>& result) const;
};

#endif