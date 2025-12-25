#include "stringDL.h"

using namespace std;

StringDL::StringDL() : head(nullptr), tail(nullptr) {}

StringDL::~StringDL() {
    clear();
}

void StringDL::clear() {
    while (head) {
        deleteHead();
    }
}

void StringDL::addHead(const string& name) {
    NodeDL* newNode = new NodeDL{name, head, nullptr};
    if (head) {
        head->prev = newNode;
    }
    head = newNode;
    if (!tail) {
        tail = newNode;
    }
}

void StringDL::addTail(const string& name) {
    NodeDL* newNode = new NodeDL{name, nullptr, tail};
    if (tail) {
        tail->next = newNode;
    }
    tail = newNode;
    if (!head) {
        head = newNode;
    }
}

void StringDL::addAfter(NodeDL* ptr, const string& name) {
    if (!ptr) return;
    NodeDL* newNode = new NodeDL{name, ptr->next, ptr};
    if (ptr->next) {
        ptr->next->prev = newNode;
    }
    ptr->next = newNode;
    if (ptr == tail) {
        tail = newNode;
    }
}

void StringDL::addBefore(NodeDL* ptr, const string& name) {
    if (!ptr) return;
    NodeDL* newNode = new NodeDL{name, ptr, ptr->prev};
    if (ptr->prev) {
        ptr->prev->next = newNode;
    }
    ptr->prev = newNode;
    if (ptr == head) {
        head = newNode;
    }
}

void StringDL::deleteHead() {
    if (!head) return;
    NodeDL* temp = head;
    head = head->next;
    if (head) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    delete temp;
}

void StringDL::deleteTail() {
    if (!tail) return;
    NodeDL* temp = tail;
    tail = tail->prev;
    if (tail) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }
    delete temp;
}

void StringDL::deleteByValue(const string& name) {
    NodeDL* el = find(name);
    if (!el) return;

    if (el == head) {
        deleteHead();
    } else if (el == tail) {
        deleteTail();
    } else {
        el->prev->next = el->next;
        el->next->prev = el->prev;
        delete el;
    }
}

StringDL::NodeDL* StringDL::find(const string& name) const {
    NodeDL* curr = head;
    while (curr) {
        if (curr->person == name) return curr;
        curr = curr->next;
    }
    return nullptr;
}

void StringDL::print() const {
    cout << "Вывод: ";
    NodeDL* curr = head;
    while (curr) {
        cout << curr->person << " ";
        curr = curr->next;
    }
    cout << endl;
}

void StringDL::printRecursive() const {
    cout << "Вывод рекурсией: ";
    printRecursivePrivate(head);
    cout << endl;
}

void StringDL::printRecursivePrivate(NodeDL* ptr) const {
    if (!ptr) return;
    cout << ptr->person << " ";
    printRecursivePrivate(ptr->next);
}