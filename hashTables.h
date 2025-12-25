#ifndef HASHTABLES_H
#define HASHTABLES_H

#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <stdexcept>
#include <string>
#include <iomanip>
#include <chrono>   
#include <random>    
#include <algorithm> 

using namespace std;
using namespace std::chrono;

// ==========================================================
// 1. БАЗОВЫЙ АБСТРАКТНЫЙ КЛАСС
// ==========================================================
template<typename K, typename V>
class HashTable {
protected:
    size_t capacity; 
    size_t size;
    double loadFactorThreshold;

    virtual void rehash() = 0;

public:
    HashTable(size_t initialCapacity = 16, double loadFactor = 0.9)
        : capacity(initialCapacity), size(0), loadFactorThreshold(loadFactor) {}

    virtual ~HashTable() {}

    virtual void insert(const K& key, const V& value) = 0;
    virtual bool find(const K& key, V& value) const = 0;
    virtual bool remove(const K& key) = 0;
    virtual void display() const = 0;

    double loadFactor() const {
        return static_cast<double>(size) / capacity;
    }

    size_t getSize() const { return size; }
    size_t getCapacity() const { return capacity; }
    
    virtual double measureFindTime(const vector<K>& keysToSearch, int m) const {
        V dummy;
        auto start = high_resolution_clock::now();
        
        for (int i = 0; i < m; i++) {
            for (const auto& key : keysToSearch) {
                find(key, dummy);
            }
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        return duration.count() / 1000000.0;
    }
};

// ==========================================================
// 2. ХЕШ-ТАБЛИЦА: МЕТОД ЦЕПОЧЕК (Chaining)
// ==========================================================

template<typename K, typename V>
class ChainingHashTable : public HashTable<K, V> {
private:
    std::vector<std::list<std::pair<K, V>>> table;

    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % this->capacity;
    }

    void rehash() override {
        size_t newCapacity = this->capacity * 2;
        std::vector<std::list<std::pair<K, V>>> newTable(newCapacity);

        for (const auto& chain : table) {
            for (const auto& pair : chain) {
                size_t newIndex = std::hash<K>{}(pair.first) % newCapacity;
                newTable[newIndex].push_back(pair);
            }
        }
        table = std::move(newTable);
        this->capacity = newCapacity;
    }

public:
    ChainingHashTable(size_t initialCapacity = 16, double loadFactor = 0.9)
        : HashTable<K, V>(initialCapacity, loadFactor), table(initialCapacity) {}

    void insert(const K& key, const V& value) override {
        if (this->loadFactor() >= this->loadFactorThreshold) rehash();

        size_t index = hash(key);
        for (auto& pair : table[index]) {
            if (pair.first == key) {
                pair.second = value;
                return;
            }
        }
        table[index].emplace_back(key, value);
        this->size++;
    }

    bool find(const K& key, V& value) const override {
        size_t index = hash(key);
        for (const auto& pair : table[index]) {
            if (pair.first == key) {
                value = pair.second;
                return true;
            }
        }
        return false;
    }

    bool remove(const K& key) override {
        size_t index = hash(key);
        auto& chain = table[index];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->first == key) {
                chain.erase(it);
                this->size--;
                return true;
            }
        }
        return false;
    }

    void display() const override {
        std::cout << "\nХЕШ-ТАБЛИЦА С МЕТОДОМ ЦЕПОЧЕК\n";
        for (size_t i = 0; i < table.size(); ++i) {
            if (!table[i].empty()) {
                std::cout << "Ячейка [" << i << "]: ";
                for (const auto& pair : table[i]) 
                    std::cout << "{" << pair.first << " = " << pair.second << "} ";
                std::cout << std::endl;
            }
        }
    }
};

// ==========================================================
// 3. ХЕШ-ТАБЛИЦА: ОТКРЫТАЯ АДРЕСАЦИЯ (Double Hashing)
// ==========================================================

template<typename K, typename V>
class OpenAddressingHashTable : public HashTable<K, V> {
private:
    enum class EntryState { EMPTY, OCCUPIED, DELETED };

    struct Entry {
        K key;
        V value;
        EntryState state;
        Entry() : state(EntryState::EMPTY) {}
    };

    std::vector<Entry> table;

    size_t hash(const K& key, size_t attempt) const {
        size_t h1 = std::hash<K>{}(key) % this->capacity;
        size_t h2 = 1 + (std::hash<K>{}(key) % (this->capacity - 1));
        return (h1 + attempt * h2) % this->capacity;
    }

    void rehash() override {
        size_t oldCapacity = this->capacity;
        size_t newCapacity = oldCapacity * 2;
        std::vector<Entry> oldTable = std::move(table);
        
        table = std::vector<Entry>(newCapacity);
        this->capacity = newCapacity;
        this->size = 0;

        for (const auto& entry : oldTable) {
            if (entry.state == EntryState::OCCUPIED) {
                insert(entry.key, entry.value);
            }
        }
    }

public:
    OpenAddressingHashTable(size_t initialCapacity = 16, double loadFactor = 0.9)
        : HashTable<K, V>(initialCapacity, loadFactor), table(initialCapacity) {}

    void insert(const K& key, const V& value) override {
        if (this->loadFactor() >= this->loadFactorThreshold) rehash();

        for (size_t attempt = 0; attempt < this->capacity; ++attempt) {
            size_t index = hash(key, attempt);
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                table[index].value = value;
                return;
            }
            if (table[index].state != EntryState::OCCUPIED) {
                table[index].key = key;
                table[index].value = value;
                table[index].state = EntryState::OCCUPIED;
                this->size++;
                return;
            }
        }
        rehash();
        insert(key, value);
    }

    bool find(const K& key, V& value) const override {
        for (size_t attempt = 0; attempt < this->capacity; ++attempt) {
            size_t index = hash(key, attempt);
            if (table[index].state == EntryState::EMPTY) return false;
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                value = table[index].value;
                return true;
            }
        }
        return false;
    }

    bool remove(const K& key) override {
        for (size_t attempt = 0; attempt < this->capacity; ++attempt) {
            size_t index = hash(key, attempt);
            if (table[index].state == EntryState::EMPTY) return false;
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                table[index].state = EntryState::DELETED;
                this->size--;
                return true;
            }
        }
        return false;
    }

    void display() const override {
        std::cout << "\nХЕШ-ТАБЛИЦА С ОТКРЫТОЙ АДРЕСАЦИЕЙ\n";
        for (size_t i = 0; i < table.size(); ++i) {
            if (table[i].state == EntryState::OCCUPIED) {
                std::cout << "Ячейка [" << i << "]: {" << table[i].key << " = " << table[i].value << "}\n";
            }
        }
    }
};

#endif