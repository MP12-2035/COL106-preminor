#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <vector>
#include <string>
#include <iostream>   // for std::cout
#include "TreeNode.hpp"

template <typename K, typename V>
class HashMap {
    struct Node {
        int key;
        V value;
        Node* next;
        Node(int k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    std::vector<Node*> table;
    int size;                // Number of elements stored
    int capacity;            // Current capacity (number of buckets)
    const std::vector<int> capacities = {7, 17, 37, 79, 163, 331, 673, 1009};
    int capacityIndex;       // Index into capacities vector
    float maxLoad = 0.8f;

    int hashFunction(int key) const {
        return key % capacity;
    }

    void clear();

public:
    HashMap();
    ~HashMap();

    void resize();

    void insert(const K& key, const V& value);
    bool find(const K& key, V& value_out) const;
    bool remove(const K& key);
    float getLoadFactor() const {
        return static_cast<float>(size) / capacity;
    }

    // Simple iterate with a lambda/function
    template <typename Func>
    void iterate(Func func) const {
        for (int i = 0; i < capacity; ++i) {
            Node* curr = table[i];
            while (curr) {
                func(curr->key, curr->value);
                curr = curr->next;
            }
        }
    }
};

// Constructor
template <typename K, typename V>
HashMap<K, V>::HashMap() : size(0), capacityIndex(0) {
    capacity = capacities[capacityIndex];
    table.resize(capacity, nullptr);
}

// Destructor
template <typename K, typename V>
HashMap<K, V>::~HashMap() {
    clear();
}

// Clear all nodes and reset
template <typename K, typename V>
void HashMap<K, V>::clear() {
    for (auto head : table) {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    table.clear();
    size = 0;
}


template <typename K, typename V>
void HashMap<K, V>::resize() {
    if (capacityIndex + 1 >= static_cast<int>(capacities.size())) {
        std::cout << "Max capacity reached, cannot resize further." << std::endl;
        return;
    }
    int newCapacity = capacities[++capacityIndex];
    std::vector<Node*> newTable(newCapacity, nullptr);

    for (int i = 0; i < capacity; ++i) {
        Node* curr = table[i];
        while (curr) {
            Node* nextNode = curr->next;
            int newIndex = curr->key % newCapacity;
            curr->next = newTable[newIndex];
            newTable[newIndex] = curr;
            curr = nextNode;
        }
    }

    table = std::move(newTable);
    capacity = newCapacity;
}


template <typename K, typename V>
void HashMap<K, V>::insert(const K& key, const V& value) {
    int hashedKey = key;
    int index = hashFunction(hashedKey);

    Node* curr = table[index];
    while (curr) {
        if (curr->key == hashedKey) {
            curr->value = value;
            return;
        }
        curr = curr->next;
    }

    Node* newNode = new Node(hashedKey, value);
    newNode->next = table[index];
    table[index] = newNode;
    ++size;

    if (getLoadFactor() > maxLoad) {
        resize();
    }
}

// Find value by key
template <typename K, typename V>
bool HashMap<K, V>::find(const K& key, V& value_out) const {
    int hashedKey = key;
    int index = hashedKey % capacity;
    Node* curr = table[index];
    while (curr) {
        if (curr->key == hashedKey) {
            value_out = curr->value;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

// Remove node by key
template <typename K, typename V>
bool HashMap<K, V>::remove(const K& key) {
    int hashedKey = key;
    int index = hashedKey % capacity;
    Node* curr = table[index];
    Node* prev = nullptr;

    while (curr) {
        if (curr->key == hashedKey) {
            if (prev) {
                prev->next = curr->next;
            } else {
                table[index] = curr->next;
            }
            delete curr;
            --size;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

#endif // HASHMAP_HPP
