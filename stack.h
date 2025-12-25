#ifndef STACK_H
#define STACK_H

#include <string>
#include <iostream>

class Stack {
private:
    struct NodeS {
        std::string key;
        NodeS* next;
    };

    NodeS* head;
    int stackSize;

    // Вспомогательный метод для полной очистки
    void clear();

public:
    Stack();  // Конструктор
    ~Stack(); // Деструктор

    bool isEmpty() const;
    int size() const;

    void push(const std::string& data);
    std::string pop();      // Удаляет и возвращает значение
    std::string peek() const; // Просто возвращает значение вершины

    std::string getByIndex(int inx) const;
    void print() const;
};

#endif