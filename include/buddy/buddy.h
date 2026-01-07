#ifndef BUDDY_H
#define BUDDY_H

#include <cstddef>
#include <vector>
#include <unordered_map>

using namespace std;

class BuddyAllocator {
public:
    // Construct a buddy allocator with total memory and minimum block size
    BuddyAllocator(size_t total_memory, size_t min_block_size);

    // Allocate at least `size` bytes, rounded up to nearest power of two
    // Returns starting address (offset) or -1 on failure
    long long alloc(size_t size);

    // Free a previously allocated block by its starting address
    void free_block(size_t addr);

    // Dump free lists for debugging and demonstration
    void dump() const;

private:
    size_t total_memory;      // Total managed memory (power of two)
    size_t min_block_size;    // Smallest allocatable block
    size_t max_order;         // log2(total_memory / min_block_size)

    // Free lists indexed by order
    // order k represents blocks of size min_block_size * 2^k
    vector<vector<size_t>> free_lists;

    unordered_map<size_t, size_t> allocated_blocks;

    // Helper functions
    size_t size_to_order(size_t size) const;
    size_t order_to_size(size_t order) const;
    size_t buddy_of(size_t addr, size_t order) const;
};

#endif
