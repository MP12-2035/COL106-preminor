#ifndef HEAP_HPP
#define HEAP_HPP
#include <vector>
#include <iostream>
#include <string>
#include "hash_map.hpp"
class heap {
public:    
    heap();
    ~heap();
    void ins(const std::string& key, int value);
    void rm(const std::string& key);
    void upd(const std::string& key, int new_val);
    void print_top(int num) const;
private:
    std::vector<std::pair<std::string, int>> elements;
    hash_map<std::string, int> key_to_idx;
    int parent(int i) const { return (i - 1) / 2; }
    int left_child(int i) const { return 2 * i + 1; }
    int right_child(int i) const { return 2 * i + 2; }
    void heapify_up(int idx);
    void heapify_down(int idx);
    void swap_els(int i, int j);
};
using hp = heap;

hp::hp() {}
hp::~hp() {}
void hp::swap_els(int i, int j) {
    std::swap(elements[i], elements[j]);
    key_to_idx.ins(elements[i].first, i);
    key_to_idx.ins(elements[j].first, j);
}
void hp::heapify_up(int idx) {
    while (idx > 0) {
        int p = parent(idx);
        if (elements[idx].second > elements[p].second) {
            swap_els(idx, p);
            idx = p;
        } else {
            break;
        }
    }
}
void hp::heapify_down(int idx) {
    int size = (int)elements.size();
    while (true) {
        int largest = idx;
        int left = left_child(idx);
        int right = right_child(idx);
        if (left < size && elements[left].second > elements[largest].second)
            largest = left;
        if (right < size && elements[right].second > elements[largest].second)
            largest = right;
        if (largest != idx) {
            swap_els(idx, largest);
            idx = largest;
        } else {
            break;
        }
    }
}
void hp::ins(const std::string& key, int value) {
    int dummy_idx;
    if (key_to_idx.find(key, dummy_idx)) {
        upd(key, value);
        return;
    }
    elements.push_back({key, value});
    int idx = (int)elements.size() - 1;
    key_to_idx.ins(key, idx);
    heapify_up(idx);
}
void hp::rm(const std::string& key) {
    int idx = -1;
    if (!key_to_idx.find(key, idx)) return;
    int last_idx = (int)elements.size() - 1;
    if (idx != last_idx) {
        swap_els(idx, last_idx);
    }
    elements.pop_back();
    key_to_idx.rm(key);
    if (idx < (int)elements.size()) {
        heapify_up(idx);
        heapify_down(idx);
    }
}
void hp::upd(const std::string& key, int new_val) {
    int idx = -1;
    if (!key_to_idx.find(key, idx)) return;
    int old_val = elements[idx].second;
    elements[idx].second = new_val;
    if (new_val > old_val) {
        heapify_up(idx);
    } else if (new_val < old_val) {
        heapify_down(idx);
    }
}
void hp::print_top(int num) const {
    if (num <= 0) return;
    int count = 0;
    for (auto& elem : elements) {
        std::cout << "Key: " << elem.first << ", Value: " << elem.second << std::endl;
        if (++count >= num) break;
    }
}
#endif // HEAP_HPP
