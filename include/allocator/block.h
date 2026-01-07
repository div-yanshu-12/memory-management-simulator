#ifndef BLOCK_H
#define BLOCK_H

#include <cstddef>

struct Block {
    size_t start;   // Starting offset ("physical address")
    size_t size;    // Size of the block in bytes
    bool free;      // true  -> block is free
                    // false -> block is allocated
    int id;         // Allocation ID (unique), -1 if free

    Block* prev;    // Previous block in memory
    Block* next;    // Next block in memory

    /*
     Constructor
     Initializes a memory block with given parameters.
    */
    Block(size_t s, size_t sz, bool is_free, int alloc_id)
        : start(s), size(sz), free(is_free), id(alloc_id),
          prev(nullptr), next(nullptr) {}
};

#endif 
