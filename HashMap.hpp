#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP
#include <vector>
#include <string>
#include <iostream>
#include "tree_node.hpp"
template <typename K, typename V>
class hash_map {
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
    int cap_in;
    float max_load = 0.8f;
    // Hash functions: overload for int and string
    int hash_fn(const int& key) const {
        return key % capacity;
    }
    int hash_fn(const std::string& key) const {
        const uint64_t base = 131;
        uint64_t hash = 0;
        for (char c : key) {
            hash = hash * base + static_cast<unsigned char>(c);
        }
        return static_cast<int>(hash % capacity);
    }
    template<typename T>
    int hash_fn(const T&) const {
        static_assert(sizeof(T) == 0, "Hash function not defined for this key type.");
        return 0;
    }

    void clear();
public:
    hash_map();
    ~hash_map();
    void resize();
    void ins(const K& key, const V& value);
    bool find(const K& key, V& value_out) const;
    bool rm(const K& key);
    float get_load_factor() const { return static_cast<float>(size) / capacity; }
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
using hm = hash_map;

template <typename K, typename V>
hm<K, V>::hm() : size(0), cap_in(0) {
    capacity = capacities[cap_in];
    table.resize(capacity, nullptr);
}
template <typename K, typename V>
hm<K, V>::~hm() {
    clear();
}
template <typename K, typename V>
void hm<K, V>::clear() {
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
void hm<K, V>::resize() {
    if (cap_in + 1 >= int(capacities.size())) {
        std::cout << "Max capacity reached, cannot resize further." << std::endl;
        return;
    }
    int new_capacity = capacities[++cap_in];
    std::vector<Node*> new_table(new_capacity, nullptr);
    for (int i = 0; i < capacity; ++i) {
        Node* curr = table[i];
        while (curr) {
            Node* next_node = curr->next;
            int new_index = 0;
            if constexpr (std::is_same_v<K, int>) {
                new_index = curr->key % new_capacity;
            } else if constexpr (std::is_same_v<K, std::string>) {
                const uint64_t base = 131;
                uint64_t hash = 0;
                for (char c : curr->key) {
                    hash = hash * base + static_cast<unsigned char>(c);
                }
                new_index = static_cast<int>(hash % new_capacity);
            }
            curr->next = new_table[new_index];
            new_table[new_index] = curr;
            curr = next_node;
        }
    }
    table = std::move(new_table);
    capacity = new_capacity;
}
template <typename K, typename V>
void hm<K, V>::ins(const K& key, const V& value) {
    int index = hash_fn(key);
    Node* curr = table[index];
    while (curr) {
        if (curr->key == key) {
            curr->value = value;
            return;
        }
        curr = curr->next;
    }
    Node* new_node = new Node(key, value);
    new_node->next = table[index];
    table[index] = new_node;
    ++size;
    if (get_load_factor() > max_load) resize();
}
template <typename K, typename V>
bool hm<K, V>::find(const K& key, V& value_out) const {
    int index = hash_fn(key);
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
template <typename K, typename V>
bool hm<K, V>::rm(const K& key) {
    int index = hash_fn(key);
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
#endif // HASH_MAP_HPP
