#include "stack.h"

using namespace std;

Stack::Stack() : head(nullptr), stackSize(0) {}

Stack::~Stack() {
    clear();
}

void Stack::clear() {
    while (!isEmpty()) {
        pop();
    }
}

bool Stack::isEmpty() const {
    return head == nullptr;
}

int Stack::size() const {
    return stackSize;
}

// Добавление в стек (LIFO - последним пришел, первым ушел)
void Stack::push(const string& data) {
    NodeS* newNode = new NodeS{data, head};
    head = newNode;
    stackSize++;
}

// Извлечение из стека
string Stack::pop() {
    if (isEmpty()) {
        cout << "Стек пустой" << endl;
        return "";
    }
    
    NodeS* temp = head;
    string val = temp->key;
    head = head->next;
    
    delete temp;
    stackSize--;
    return val;
}

string Stack::peek() const {
    if (isEmpty()) return "";
    return head->key;
}

string Stack::getByIndex(int inx) const {
    if (inx < 0 || inx >= stackSize) {
        cout << "Выход за диапазон" << endl;
        return "";
    }
    NodeS* curr = head;
    for (int i = 0; i < inx; i++) {
        curr = curr->next;
    }
    return curr->key;
}

void Stack::print() const {
    if (isEmpty()) {
        cout << "Стек пустой" << endl;
        return;
    }

    NodeS* curr = head;
    cout << "Вывод стека (от вершины): ";
    while (curr != nullptr) {
        cout << curr->key << " ";
        curr = curr->next;
    }
    cout << endl;
}