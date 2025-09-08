#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>
#include <iostream>
#include <string>
#include "HashMap.hpp"

class Heap {
public:    
    Heap();
    ~Heap();

    // Insert a new (key, value) pair into the heap
    void insert(const std::string& key, int value);

    // Remove the node associated with key
    void remove(const std::string& key);

    // Update the value associated with key
    void update(const std::string& key, int newValue);

    // Print top 'num' elements (max values first)
    void printTop(int num) const;

private:
    std::vector<std::pair<std::string, int>> elements; // key, value pairs
    HashMap<std::string, int> keyToIndex;              // map key -> index in elements

    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }

    void heapifyUp(int index);
    void heapifyDown(int index);
    void swapElements(int i, int j);
};

// Constructor
Heap::Heap() {
    // nothing to initialize
}
// Destructor
Heap::~Heap() {
    // nothing to cleanup (vector and hashmap clean themselves)
}

// Swap heap elements at positions i and j and update keyToIndex map
void Heap::swapElements(int i, int j) {
    std::swap(elements[i], elements[j]);
    keyToIndex.insert(elements[i].first, i);
    keyToIndex.insert(elements[j].first, j);
}

// Heapify up after insert or update (for increased value)
void Heap::heapifyUp(int index) {
    while (index > 0) {
        int p = parent(index);
        if (elements[index].second > elements[p].second) {
            swapElements(index, p);
            index = p;
        } else {
            break;
        }
    }
}

// Heapify down after removal or update (for decreased value)
void Heap::heapifyDown(int index) {
    int size = (int)elements.size();
    while (true) {
        int largest = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < size && elements[left].second > elements[largest].second)
            largest = left;
        if (right < size && elements[right].second > elements[largest].second)
            largest = right;

        if (largest != index) {
            swapElements(index, largest);
            index = largest;
        } else {
            break;
        }
    }
}

// Insert new key-value pair
void Heap::insert(const std::string& key, int value) {
    if (keyToIndex.find(key, value)) {
        // key exists, use update instead
        update(key, value);
        return;
    }
    elements.push_back({key, value});
    int index = (int)elements.size() - 1;
    keyToIndex.insert(key, index);
    heapifyUp(index);
}

// Remove node by key
void Heap::remove(const std::string& key) {
    int index = -1;
    if (!keyToIndex.find(key, index)) return; // key not found

    int lastIndex = (int)elements.size() - 1;
    if (index != lastIndex) {
        swapElements(index, lastIndex);
    }

    elements.pop_back();
    keyToIndex.remove(key);

    if (index < (int)elements.size()) {
        // Fix heap property from changed position
        heapifyUp(index);
        heapifyDown(index);
    }
}

// Update the value at the node with given key
void Heap::update(const std::string& key, int newValue) {
    int index = -1;
    if (!keyToIndex.find(key, index)) return; // key not found

    int oldValue = elements[index].second;
    elements[index].second = newValue;

    if (newValue > oldValue) {
        heapifyUp(index);
    } else if (newValue < oldValue) {
        heapifyDown(index);
    }
}

// Print top 'num' elements maintaining original heap order
void Heap::printTop(int num) const {
    if (num <= 0) return;
    int count = 0;
    for (auto& elem : elements) {
        std::cout << "Key: " << elem.first << ", Value: " << elem.second << std::endl;
        if (++count >= num) break;
    }
}

#endif // HEAP_HPP
