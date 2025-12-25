#ifndef STRINGDL_H
#define STRINGDL_H

#include <string>
#include <iostream>

class StringDL {
private:
    struct NodeDL {
        std::string person;
        NodeDL* next;
        NodeDL* prev;
    };

    NodeDL* head;
    NodeDL* tail;

    // Вспомогательный метод для рекурсивного вывода
    void printRecursivePrivate(NodeDL* ptr) const;
    // Вспомогательный метод для очистки памяти
    void clear();

public:
    StringDL();  // Конструктор (заменяет init)
    ~StringDL(); // Деструктор (освобождает память)

    void addHead(const std::string& name);
    void addTail(const std::string& name);
    
    // В ООП лучше возвращать указатель или работать через итераторы
    NodeDL* find(const std::string& name) const;

    void addAfter(NodeDL* ptr, const std::string& name);
    void addBefore(NodeDL* ptr, const std::string& name);

    void deleteHead();
    void deleteTail();
    void deleteAfter(NodeDL* ptr);
    void deleteBefore(NodeDL* ptr);
    void deleteByValue(const std::string& name);

    void print() const;
    void printRecursive() const;
};

#endif