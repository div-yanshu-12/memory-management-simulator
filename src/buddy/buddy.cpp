#include "buddy/buddy.h"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

/*
 Buddy Allocator Implementation
 This implementation follows the classical buddy system:

 - Memory is divided into blocks of size 2^k
 - Allocation splits larger blocks recursively
 - Freeing merges buddies using XOR logic

 This module is intentionally independent of the linked-list allocator.
*/

BuddyAllocator::BuddyAllocator(size_t total_mem, size_t min_block)
    : total_memory(total_mem), min_block_size(min_block) {

    // Compute maximum order
    max_order = static_cast<size_t>(log2(total_memory / min_block_size));

    free_lists.resize(max_order + 1);

    // Initially, one big free block at max order
    free_lists[max_order].push_back(0);
}

/*
 Converts requested size to the smallest order that can fit it
*/
size_t BuddyAllocator::size_to_order(size_t size) const {
    size_t required = max(size, min_block_size);
    size_t order = 0;
    size_t block = min_block_size;

    while (block < required) {
        block <<= 1;
        order++;
    }
    return order;
}

size_t BuddyAllocator::order_to_size(size_t order) const {
    return min_block_size << order;
}

/*
 Computes buddy address using XOR operation
*/
size_t BuddyAllocator::buddy_of(size_t addr, size_t order) const {
    return addr ^ order_to_size(order);
}

/*
 Allocate memory
*/
long long BuddyAllocator::alloc(size_t size) {
    size_t order = size_to_order(size);
    size_t cur_order = order;

    while (cur_order <= max_order && free_lists[cur_order].empty())
        cur_order++;

    if (cur_order > max_order)
        return -1;

    size_t addr = free_lists[cur_order].back();
    free_lists[cur_order].pop_back();

    while (cur_order > order) {
        cur_order--;
        size_t buddy = addr + order_to_size(cur_order);
        free_lists[cur_order].push_back(buddy);
    }

    allocated_blocks[addr] = order;   
    return static_cast<long long>(addr);
}

/*
 Free a block and recursively merge buddies
*/
void BuddyAllocator::free_block(size_t addr) {
    if (allocated_blocks.find(addr) == allocated_blocks.end())
        return; // Invalid free

    size_t order = allocated_blocks[addr];
    allocated_blocks.erase(addr);

    while (order < max_order) {
        size_t buddy = buddy_of(addr, order);
        auto &list = free_lists[order];

        auto it = find(list.begin(), list.end(), buddy);
        if (it == list.end())
            break;

        list.erase(it);
        addr = min(addr, buddy);
        order++;
    }

    free_lists[order].push_back(addr);
}

/*
 Dump free lists for visualization
*/
void BuddyAllocator::dump() const {
    cout << "Buddy Free Lists:\n";
    for (size_t i = 0; i <= max_order; i++) {
        cout << "Order " << i
             << " (size=" << order_to_size(i) << "): ";
        for (size_t addr : free_lists[i])
            cout << addr << " ";
        cout << '\n';
    }
}
