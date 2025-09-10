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
    std::swap(elements[i], elements[j]);
    key_to_idx.rm(elements[i].first);
    key_to_idx.rm(elements[j].first);
    key_to_idx.ins(elements[i].first, i);
    key_to_idx.ins(elements[j].first, j);
}

void heap::heapify_up(int idx) {
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

void heap::heapify_down(int idx) {
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

void heap::ins(const std::string& key, int value) {
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

void heap::rm(const std::string& key) {
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

void heap::upd(const std::string& key, int new_val) {
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

void heap::print_top(int num) const {
    int n = std::min(num, (int)elements.size());
    if (n == 0) {
        std::cout << "Heap is empty.\n";
        return;
    }

    std::vector<std::pair<std::string, int>> temp_elements = elements;
    hash_map<std::string, int> temp_map;
    for (size_t i = 0; i < temp_elements.size(); ++i)
        temp_map.ins(temp_elements[i].first, i);

    std::cout << "Top " << n << " elements:\n";
    for (int i = 0; i < n; ++i) {
        if (temp_elements.empty()) break;

        auto top = temp_elements[0];
        std::cout << top.first << " : " << top.second << "\n";

        int last_idx = (int)temp_elements.size() - 1;
        if (last_idx > 0) {
            std::swap(temp_elements[0], temp_elements[last_idx]);
            temp_map.rm(temp_elements[last_idx].first);
            temp_elements.pop_back();

            int idx = 0;
            int size = (int)temp_elements.size();
            while (true) {
                int largest = idx;
                int left = 2 * idx + 1;
                int right = 2 * idx + 2;

                if (left < size && temp_elements[left].second > temp_elements[largest].second)
                    largest = left;
                if (right < size && temp_elements[right].second > temp_elements[largest].second)
                    largest = right;

                if (largest != idx) {
                    std::swap(temp_elements[idx], temp_elements[largest]);
                    idx = largest;
                } else {
                    break;
                }
            }
        } else {
            temp_map.rm(temp_elements[0].first);
            temp_elements.pop_back();
        }
    }
}

#endif // HEAP_HPP
