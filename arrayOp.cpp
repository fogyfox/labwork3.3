#include <iostream>
#include <fstream>
#include "arrayOp.h"

using namespace std;

// Конструктор
Array::Array() {
    capacity = 1;
    size = 0;
    data = new NodeA[capacity];
}

// Деструктор (важно для предотвращения утечек памяти)
Array::~Array() {
    delete[] data;
}

// Приватный метод для увеличения емкости
void Array::resize() {
    capacity *= 2;
    NodeA* newData = new NodeA[capacity];
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

bool Array::isEmpty() const {
    return size == 0;
}

void Array::pushBack(const string& value) {
    if (size >= capacity) {
        resize();
    }
    data[size].value = value;
    size++;
}

void Array::addInx(const string& value, int inx) {
    if (inx < 0 || inx > size) {
        cout << "Выход за диапазон" << endl;
        return;
    }

    if (size >= capacity) {
        resize();
    }

    // Сдвиг вправо
    for (int i = size; i > inx; i--) {
        data[i] = data[i - 1];
    }

    data[inx].value = value;
    size++;
}

void Array::printArray() const {
    if (isEmpty()) {
        cout << "Массив пустой" << endl;
        return;
    }

    cout << "Вывод массива: ";
    for (int i = 0; i < size; i++) {
        cout << data[i].value << " ";
    }
    cout << endl;
}

string Array::getInx(int inx) const {
    if (inx < 0 || inx >= size) {
        cout << "Выход за диапазон" << endl;
        return "";
    }
    return data[inx].value;
}

void Array::changeInx(const string& newValue, int inx) {
    if (inx < 0 || inx >= size) {
        cout << "Выход за диапазон" << endl;
        return;
    }
    data[inx].value = newValue;
}

int Array::getSize() const {
    return size;
}

string Array::removeElArr(int inx) {
    if (inx < 0 || inx >= size) {
        cout << "Выход за диапазон" << endl;
        return "";
    }

    string removedValue = data[inx].value;

    // Сдвиг влево
    for (int i = inx; i < size - 1; i++) {
        data[i] = data[i + 1];
    }

    size--;
    return removedValue;
}

// Сохранение в бинарный файл
void Array::serialize(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) return;

    // 1. Записываем текущий размер массива
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));

    // 2. Записываем каждый элемент (строку)
    for (int i = 0; i < size; ++i) {
        const std::string& s = data[i].value;
        size_t len = s.size();
        // Сначала записываем длину строки
        ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
        // Затем сами данные строки
        ofs.write(s.data(), len);
    }
    ofs.close();
}

// Загрузка из бинарного файла
void Array::deserialize(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) return;

    // Очищаем текущие данные
    delete[] data;
    
    // 1. Читаем размер
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
    capacity = size > 0 ? size : 1;
    data = new NodeA[capacity];

    // 2. Читаем элементы
    for (int i = 0; i < size; ++i) {
        size_t len;
        ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
        std::string s(len, '\0');
        ifs.read(&s[0], len);
        data[i].value = s;
    }
    ifs.close();
}