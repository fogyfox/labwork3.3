#ifndef ARRAYOP_H
#define ARRAYOP_H

#include <string>

struct NodeA {
    std::string value;
};

class Array {
private:
    NodeA* data;
    int size;
    int capacity;

    void resize(); // Вспомогательный метод для расширения массива

public:
    Array(); // Конструктор (вместо createArray)
    ~Array(); // Деструктор для очистки памяти

    void pushBack(const std::string& value);
    void addInx(const std::string& value, int inx);
    void printArray() const;
    void changeInx(const std::string& newValue, int inx);
    std::string getInx(int inx) const;
    int getSize() const;
    bool isEmpty() const;
    std::string removeElArr(int inx);
    void serialize(const std::string& filename) const;
    void deserialize(const std::string& filename);
};

#endif