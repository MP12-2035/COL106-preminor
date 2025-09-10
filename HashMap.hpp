#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <vector>
#include <string>
#include <iostream>
#include "TreeNode.hpp"

template <typename K, typename V>
class HashMap {
    struct Node {
        K key;
        V value;
        Node* next;
        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    std::vector<Node*> table;
    int size;
    int capacity;
    const std::vector<int> capacities = {7, 17, 37, 79, 163, 331, 673, 1009};
    int capacityIndex;
    float maxLoad = 1.0f;

    // Hash functions: overload for int and string
    int hashFunction(const int& key) const {
        return key % capacity;
    }

    int hashFunction(const std::string& key) const {
        const int p = 31;
        int hash = 0;
        int power = 1;
        for (char c : key) {
            hash = (hash + (c - 'a' + 1) * power) % capacity;
            power = (power * p) % capacity;
        }
        return hash;
    }

    // Generic hash triggers compile error if unsupported type
    template<typename T>
    int hashFunction(const T&) const {
        static_assert(sizeof(T) == 0, "Hash function not defined for this key type.");
        return 0;
    }

    void clear();

public:
    HashMap();
    ~HashMap();

    void resize();
    void insert(const K& key, const V& value);
    bool find(const K& key, V& value_out) const;
    bool remove(const K& key);
    float getLoadFactor() const { return static_cast<float>(size) / capacity; }

    template <typename Func>
    void iterate(Func func) {
        for (int i = 0; i < capacity; ++i) {
            Node* curr = table[i];
            while (curr) {
                func(curr->key, curr->value);
                curr = curr->next;
            }
        }
    }

    void iterate() {
        iterate([](const K& key, V&) {
            std::cout << "Key: " << key << std::endl;
        });
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

// Clear all nodes
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

// Resize and rehash all keys
template <typename K, typename V>
void HashMap<K, V>::resize() {
    if (capacityIndex + 1 >= int(capacities.size())) {
        std::cout << "Max capacity reached, cannot resize further." << std::endl;
        return;
    }
    int newCapacity = capacities[++capacityIndex];
    std::vector<Node*> newTable(newCapacity, nullptr);

    for (int i = 0; i < capacity; ++i) {
        Node* curr = table[i];
        while (curr) {
            Node* nextNode = curr->next;
            // Use correct hashFunction depending on key type:
            int newIndex = 0;
            if constexpr (std::is_same_v<K, int>) {
                newIndex = curr->key % newCapacity;
            } else if constexpr (std::is_same_v<K, std::string>) {
                const int p = 31;
                int hash = 0;
                int power = 1;
                for (char c : curr->key) {
                    hash = (hash + (c - 'a' + 1) * power) % newCapacity;
                    power = (power * p) % newCapacity;
                }
                newIndex = hash;
            }
            curr->next = newTable[newIndex];
            newTable[newIndex] = curr;
            curr = nextNode;
        }
    }
    table = std::move(newTable);
    capacity = newCapacity;
}

// Insert or update key/value
template <typename K, typename V>
void HashMap<K, V>::insert(const K& key, const V& value) {
    int index = hashFunction(key);
    Node* curr = table[index];
    while (curr) {
        if (curr->key == key) {
            curr->value = value;
            return;
        }
        curr = curr->next;
    }
    Node* newNode = new Node(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    ++size;
    if (getLoadFactor() > maxLoad) resize();
}

// Find a value by key
template <typename K, typename V>
bool HashMap<K, V>::find(const K& key, V& value_out) const {
    int index = hashFunction(key);
    Node* curr = table[index];
    while (curr) {
        if (curr->key == key) {
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
    int index = hashFunction(key);
    Node* curr = table[index];
    Node* prev = nullptr;
    while (curr) {
        if (curr->key == key) {
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
