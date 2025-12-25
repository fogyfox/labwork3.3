#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <iostream>

class Queue {
private:
    struct NodeQ {
        std::string value;
        NodeQ* next;
    };

    NodeQ* head;
    NodeQ* tail;
    int queueSize;

    // Вспомогательный метод для очистки памяти
    void clear();

public:
    Queue();  // Конструктор (вместо createQueue)
    ~Queue(); // Деструктор для освобождения памяти

    bool isEmpty() const;
    int size() const;

    void push(const std::string& data);
    std::string pop();                  // Возвращает значение и удаляет узел
    
    std::string getByIndex(int inx) const;
    void print() const;
};

#endif