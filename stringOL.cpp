#include "stringOL.h"

using namespace std;

StringOL::StringOL() : head(nullptr) {}

StringOL::~StringOL() {
    clear();
}

void StringOL::clear() {
    while (head) {
        deleteHead();
    }
}

void StringOL::addHead(const string& name) {
    head = new Node{ name, head }; 
}

void StringOL::addTail(const string& name) {
    Node* newNode = new Node{ name, nullptr };
    if (!head) {
        head = newNode; 
        return;
    }
    Node* cur = head;
    while (cur->next) cur = cur->next; 
    cur->next = newNode;
}

void StringOL::addAfter(const string& targetValue, const string& newValue) {
    Node* cur = head;
    while (cur) {
        if (cur->person == targetValue) {
            cur->next = new Node{ newValue, cur->next }; 
            return;
        }
        cur = cur->next;
    }
}

void StringOL::addBefore(const string& targetValue, const string& newValue) {
    if (!head) return;
    if (head->person == targetValue) {
        addHead(newValue);
        return;
    }
    Node* prev = nullptr;
    Node* cur = head;
    while (cur && cur->person != targetValue) {
        prev = cur;
        cur = cur->next;
    }
    if (cur && prev) {
        prev->next = new Node{ newValue, cur }; 
    }
}

void StringOL::deleteHead() {
    if (!head) return;
    Node* t = head;
    head = head->next; 
    delete t;
}

void StringOL::deleteTail() {
    if (!head) return;
    if (!head->next) {
        delete head;
        head = nullptr; 
        return;
    }
    Node* cur = head;
    while (cur->next && cur->next->next) cur = cur->next;
    delete cur->next;
    cur->next = nullptr; 
}

void StringOL::deleteByValue(const string& name) {
    while (head && head->person == name) {
        deleteHead(); 
    }
    Node* cur = head;
    while (cur && cur->next) {
        if (cur->next->person == name) {
            Node* t = cur->next;
            cur->next = t->next; 
            delete t;
        } else {
            cur = cur->next;
        }
    }
}

StringOL::Node* StringOL::find(const string& name) const {
    Node* el = head;
    while (el) {
        if (el->person == name) return el; 
        el = el->next;
    }
    return nullptr;
}

void StringOL::print() const {
    Node* cur = head;
    bool first = true;
    while (cur) {
        if (!first) cout << " ";
        cout << cur->person;
        first = false;
        cur = cur->next;
    }
    cout << endl; 
}

void StringOL::printRecursive() const {
    printRecursivePrivate(head);
    cout << endl;
}

void StringOL::printRecursivePrivate(Node* node) const {
    if (!node) return;
    cout << node->person;
    if (node->next) {
        cout << " ";
        printRecursivePrivate(node->next); 
    }
}

void StringOL::deleteAfter(Node* ptr) {
    if (!head || !ptr || !ptr->next) return;
    Node* t = ptr->next;
    ptr->next = t->next; 
    delete t;
}

void StringOL::deleteBefore(Node* ptr) {
    if (!head || !ptr || head == ptr) return;
    if (head->next == ptr) {
        deleteHead();
        return;
    }
    Node* prev = nullptr;
    Node* cur = head;
    while (cur && cur->next && cur->next != ptr) {
        prev = cur;
        cur = cur->next;
    }
    if (cur && cur->next == ptr && prev) {
        prev->next = ptr;
        delete cur; 
    }
}