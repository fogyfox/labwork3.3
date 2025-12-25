#ifndef STRINGOL_H
#define STRINGOL_H

#include <string>
#include <iostream>

class StringOL {
private:
    struct Node {
        std::string person;
        Node* next;
    };

    Node* head;

    // Вспомогательный метод для рекурсии
    void printRecursivePrivate(Node* node) const;
    void clear();

public:
    StringOL();  // Конструктор
    ~StringOL(); // Деструктор

    void addHead(const std::string& name);
    void addTail(const std::string& name);
    
    // Поиск узла (теперь метод класса)
    Node* find(const std::string& name) const;

    void addAfter(const std::string& targetValue, const std::string& newValue);
    void addBefore(const std::string& targetValue, const std::string& newValue);

    void deleteHead();
    void deleteTail();
    void deleteByValue(const std::string& name);
    void deleteBefore(Node* ptr);
    void deleteAfter(Node* ptr);

    void print() const;
    void printRecursive() const;
};

#endif