#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>
#include <iostream>
#include <string>
#include "hash_map.hpp"

class heap {
public:
    heap() {}
    ~heap() {}

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

void heap::swap_els(int i, int j) {
    key_to_idx.rm(elements[i].first);
    key_to_idx.rm(elements[j].first);

    std::swap(elements[i], elements[j]);

    key_to_idx.ins(elements[i].first, i);
    key_to_idx.ins(elements[j].first, j);
}

void heap::heapify_up(int idx) {
    while (idx > 0) {
        int p = parent(idx);
        if (elements[idx].second > elements[p].second) {
            swap_els(idx, p);
            idx = p;
        } else break;
    }
}

void heap::heapify_down(int idx) {
    int n = elements.size();
    while (true) {
        int largest = idx;
        int l = left_child(idx), r = right_child(idx);
        if (l < n && elements[l].second > elements[largest].second) largest = l;
        if (r < n && elements[r].second > elements[largest].second) largest = r;
        if (largest != idx) { swap_els(idx, largest); idx = largest; }
        else break;
    }
}

void heap::ins(const std::string& key, int value) {
    int idx;
    if (key_to_idx.find(key, idx)) { upd(key, value); return; }
    elements.push_back({key, value});
    int new_idx = elements.size() - 1;
    key_to_idx.ins(key, new_idx);
    heapify_up(new_idx);
}

void heap::rm(const std::string& key) {
    int idx;
    if (!key_to_idx.find(key, idx)) return;
    int last = elements.size() - 1;
    if (idx != last) swap_els(idx, last);
    key_to_idx.rm(elements.back().first);
    elements.pop_back();
    if (idx < (int)elements.size()) { heapify_up(idx); heapify_down(idx); }
}

void heap::upd(const std::string& key, int new_val) {
    int idx;
    if (!key_to_idx.find(key, idx)) return;
    int old_val = elements[idx].second;
    elements[idx].second = new_val;
    if (new_val > old_val) heapify_up(idx);
    else if (new_val < old_val) heapify_down(idx);
}

void heap::print_top(int num) const {
    if (num <= 0 || elements.empty()) { std::cout << "Heap is empty.\n"; return; }

    // Use a temporary min-heap index list (no full copy)
    std::vector<std::pair<std::string,int>> temp = elements;
    auto temp_swap = [](std::vector<std::pair<std::string,int>>& arr, int i, int j){
        std::swap(arr[i], arr[j]);
    };

    int n = std::min(num, (int)temp.size());
    for (int i = 0; i < n; ++i) {
        std::cout << temp[0].first << " : " << temp[0].second << "\n";
        int last = temp.size() - 1;
        temp_swap(temp, 0, last);
        temp.pop_back();

        int idx = 0;
        while (true) {
            int largest = idx;
            int l = 2*idx + 1, r = 2*idx + 2;
            if (l < (int)temp.size() && temp[l].second > temp[largest].second) largest = l;
            if (r < (int)temp.size() && temp[r].second > temp[largest].second) largest = r;
            if (largest != idx) { temp_swap(temp, idx, largest); idx = largest; }
            else break;
        }
    }
}

#endif // HEAP_HPP
