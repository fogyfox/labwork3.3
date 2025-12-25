#include "set.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>

using namespace std;

Set::Set(int initialSize) : tableSize(initialSize), itemCount(0) {
    buckets = new NodeSet*[tableSize];
    for (int i = 0; i < tableSize; i++) {
        buckets[i] = nullptr;
    }
}

Set::~Set() {
    clear();
    delete[] buckets;
}

int Set::hashFunction(int key) const {
    hash<int> hasher;
    return hasher(key) % tableSize;
}

void Set::rehashIfNeeded() {
    double loadFactor = static_cast<double>(itemCount) / tableSize;
    if (loadFactor > 0.7) {
        int oldSize = tableSize;
        tableSize *= 2;
        NodeSet** newBuckets = new NodeSet*[tableSize];
        for (int i = 0; i < tableSize; i++) newBuckets[i] = nullptr;

        for (int i = 0; i < oldSize; i++) {
            NodeSet* current = buckets[i];
            while (current) {
                NodeSet* next = current->next;
                int newIndex = hashFunction(current->key);
                current->next = newBuckets[newIndex];
                newBuckets[newIndex] = current;
                current = next;
            }
        }
        delete[] buckets;
        buckets = newBuckets;
    }
}

bool Set::insert(int key) {
    if (contains(key)) return false;
    
    int index = hashFunction(key);
    buckets[index] = new NodeSet{key, buckets[index]};
    itemCount++;
    rehashIfNeeded();
    return true;
}

bool Set::contains(int key) const {
    int index = hashFunction(key);
    NodeSet* current = buckets[index];
    while (current) {
        if (current->key == key) return true;
        current = current->next;
    }
    return false;
}

bool Set::remove(int key) {
    int index = hashFunction(key);
    NodeSet* current = buckets[index];
    NodeSet* prev = nullptr;
    
    while (current) {
        if (current->key == key) {
            if (prev == nullptr) buckets[index] = current->next;
            else prev->next = current->next;
            delete current;
            itemCount--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

void Set::clear() {
    for (int i = 0; i < tableSize; i++) {
        NodeSet* current = buckets[i];
        while (current) {
            NodeSet* next = current->next;
            delete current;
            current = next;
        }
        buckets[i] = nullptr;
    }
    itemCount = 0;
}

int Set::size() const { return itemCount; }

bool Set::isEmpty() const { return itemCount == 0; }

void Set::print() const {
    cout << "{";
    bool first = true;
    for (int i = 0; i < tableSize; i++) {
        NodeSet* current = buckets[i];
        while (current) {
            if (!first) cout << ", ";
            cout << current->key;
            first = false;
            current = current->next;
        }
    }
    cout << "}" << endl;
}

vector<int> Set::toVector() const {
    vector<int> result;
    for (int i = 0; i < tableSize; i++) {
        NodeSet* current = buckets[i];
        while (current) {
            result.push_back(current->key);
            current = current->next;
        }
    }
    return result;
}

// Статический вспомогательный метод для рекурсии разбиения
static bool backtrack(vector<int>& nums, vector<vector<int>>& subsets, vector<int>& sums, int target, int idx) {
    if (idx == nums.size()) return true;
    for (int i = 0; i < subsets.size(); i++) {
        if (sums[i] + nums[idx] <= target) {
            subsets[i].push_back(nums[idx]);
            sums[i] += nums[idx];
            if (backtrack(nums, subsets, sums, target, idx + 1)) return true;
            sums[i] -= nums[idx];
            subsets[i].pop_back();
            if (sums[i] == 0) break;
        }
    }
    return false;
}

bool Set::partition(int subsetSum, vector<vector<int>>& result) const {
    vector<int> nums = toVector();
    int total = 0;
    for (int n : nums) total += n;
    
    if (total % subsetSum != 0) return false;
    int k = total / subsetSum;
    
    sort(nums.begin(), nums.end(), greater<int>());
    if (nums.empty() || nums[0] > subsetSum) return false;

    vector<vector<int>> subsets(k);
    vector<int> sums(k, 0);
    if (backtrack(nums, subsets, sums, subsetSum, 0)) {
        result = subsets;
        return true;
    }
    return false;
}