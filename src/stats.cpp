#include "stats.h"
#include "allocator/block.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Accessors from allocator_core.cpp
Block* get_head();
size_t get_total_memory();

/*
 Used Memory
 Sum of sizes of all allocated blocks
*/
size_t get_used_memory() {
    size_t used = 0;
    for (Block* cur = get_head(); cur; cur = cur->next)
        if (!cur->free)
            used += cur->size;
    return used;
}

/*
 Free Memory
 Sum of sizes of all free blocks
*/
size_t get_free_memory() {
    size_t free_mem = 0;
    for (Block* cur = get_head(); cur; cur = cur->next)
        if (cur->free)
            free_mem += cur->size;
    return free_mem;
}

/*
 Largest Free Block
 Used to compute external fragmentation
*/
size_t get_largest_free_block() {
    size_t largest = 0;
    for (Block* cur = get_head(); cur; cur = cur->next)
        if (cur->free)
            largest = max(largest, cur->size);
    return largest;
}

/*
 Internal Fragmentation
 Defined here as:
   Sum of (allocated block size - requested size)

 Since this simulator allocates exact sizes, internal fragmentation = 0
*/
size_t get_internal_fragmentation() {
    return 0;
}

/*
 External Fragmentation
 Formula:
   1 - (largest_free_block / total_free_memory)
*/
double get_external_fragmentation() {
    size_t free_mem = get_free_memory();
    if (free_mem == 0) return 0.0;

    double frag = 1.0 -
        (double)get_largest_free_block() / free_mem;

    return frag * 100.0;
}

/*
 Print All Stats
*/
void print_stats() {
    size_t total = get_total_memory();
    size_t used = get_used_memory();
    size_t free_mem = get_free_memory();

    double utilization = (double)used / total * 100.0;

    cout << "Total memory: " << total << " bytes\n";
    cout << "Used memory: " << used << " bytes\n";
    cout << "Free memory: " << free_mem << " bytes\n";
    cout << "Memory utilization: "
         << fixed << setprecision(2)
         << utilization << "%\n";
    cout << "External fragmentation: "
         << get_external_fragmentation() << "%\n";
}
