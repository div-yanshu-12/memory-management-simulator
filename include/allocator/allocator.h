#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "block.h"
#include <cstddef>

// Allocation Strategies

enum class AllocatorType {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

// Core Allocator Control

/*
 Initializes the physical memory with the given size (bytes)
*/
void init_memory(size_t size);

/*
 Selects the allocation strategy to be used
*/
void set_allocator(AllocatorType type);

// Allocation / Deallocation API

/*
 Allocates a block of the requested size.

 Returns:
 - allocation ID on success
 - -1 on failure (no suitable free block)
*/
int mem_malloc(size_t size);

/*
 Frees a previously allocated block using allocation ID

 Returns:
 - true  if successful
 - false if ID is invalid
*/
bool mem_free(int id);
/*
 Dumps current physical memory layout
*/
void dump_memory();

#endif
