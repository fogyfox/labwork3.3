#include "queue.h"

using namespace std;

// Конструктор: инициализирует пустую очередь
Queue::Queue() : head(nullptr), tail(nullptr), queueSize(0) {}

// Деструктор: очищает всю память
Queue::~Queue() {
    clear();
}

void Queue::clear() {
    while (!isEmpty()) {
        pop();
    }
}

bool Queue::isEmpty() const {
    return head == nullptr;
}

int Queue::size() const {
    return queueSize;
}

// Добавление элемента в конец очереди (push)
void Queue::push(const string& data) {
    NodeQ* newNode = new NodeQ{data, nullptr};
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    queueSize++;
}

// Извлечение значения из начала очереди (pop)
string Queue::pop() {
    if (isEmpty()) {
        cout << "Очередь пустая" << endl;
        return "";
    }
    
    NodeQ* temp = head;
    string val = temp->value;
    head = head->next;
    
    if (head == nullptr) {
        tail = nullptr;
    }
    
    delete temp;
    queueSize--;
    return val;
}

// Получение значения по индексу
string Queue::getByIndex(int inx) const {
    if (inx < 0 || inx >= queueSize) {
        cout << "Выход за диапазон" << endl;
        return "";
    }
    NodeQ* curr = head;
    for (int i = 0; i < inx; i++) {
        curr = curr->next;
    }
    return curr->value;
}

// Вывод очереди на экран
void Queue::print() const {
    if (isEmpty()) {
        cout << "Очередь пустая" << endl;
        return;
    }

    NodeQ* curr = head;
    cout << "Вывод очереди: ";
    while (curr != nullptr) {
        cout << curr->value << " ";
        curr = curr->next;
    }
    cout << endl;
}