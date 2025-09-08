#ifndef HEAP_HPP
#define HEAP_HPP

// Heap to track files by recent modification or size (total versions)

class Heap {
public:
    Heap();
    ~Heap();

    void insert(/* parameters */);
    void remove(/* parameters */);
    void update(/* parameters */);
    void printTop(int num);

private:
    // Internal storage and helper functions

    // TODO: Implement your heap data structure here
};

#endif // HEAP_HPP
