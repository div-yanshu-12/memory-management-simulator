#ifndef STATS_H
#define STATS_H

#include <cstddef>

// Returns total allocated (used) memory in bytes
size_t get_used_memory();

// Returns total free memory in bytes
size_t get_free_memory();

// Returns size of the largest free block
size_t get_largest_free_block();

// Returns internal fragmentation in bytes
size_t get_internal_fragmentation();

// Returns external fragmentation percentage (0–100)
double get_external_fragmentation();

// Prints all statistics in human-readable form
void print_stats();

#endif 
