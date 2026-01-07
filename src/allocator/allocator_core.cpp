#include "allocator/block.h"
#include "allocator/allocator.h"

#include <iostream>
using namespace std;

// allocator policy state 
static AllocatorType current_policy = AllocatorType::FIRST_FIT;

Block* first_fit_find(Block* head, size_t size);
Block* best_fit_find(Block* head, size_t size);
Block* worst_fit_find(Block* head, size_t size);

// Head of the block list (lowest address)
static Block* head = nullptr;

// Total physical memory size (bytes)
static size_t TOTAL_MEMORY = 0;

// Generates unique allocation IDs
static int NEXT_ID = 1;

static void clear_blocks() {
    Block* cur = head;
    while (cur) {
        Block* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = nullptr;
}

// Initialize Physical Memory
/*
 init_memory(size)
 Creates a single large free block representing all physical memory.

 Example:
   init_memory(1024)
   -> [0 .. 1023] FREE
*/
void init_memory(size_t size) {
    clear_blocks();

    TOTAL_MEMORY = size;
    NEXT_ID = 1;

    // Entire memory starts as one free block
    head = new Block(0, size, true, -1);
}

void set_allocator(AllocatorType type) {
    current_policy = type;
}

// Core Allocation Mechanism 
/*
 allocate_from_block(block, size)
 --------------------------------
 Takes a *chosen* free block and allocates memory from it.

 If the block is larger than requested:
 - Split it into:
   [ALLOCATED | FREE]

 If the block is exactly the size:
 - No split is performed
*/
int allocate_from_block(Block* blk, size_t size) {
    if (!blk || !blk->free || blk->size < size)
        return -1;

    // Split if block is larger than needed
    if (blk->size > size) {
        Block* remainder = new Block(
            blk->start + size,          // New block starts after allocation
            blk->size - size,           // Remaining size
            true,                       // Free
            -1
        );

        // Insert remainder into list
        remainder->next = blk->next;
        remainder->prev = blk;
        if (blk->next)
            blk->next->prev = remainder;
        blk->next = remainder;
    }

    // Mark allocated block
    blk->size = size;
    blk->free = false;
    blk->id = NEXT_ID++;

    return blk->id;
}

int mem_malloc(size_t size) {
    if (size == 0) return -1;

    Block* chosen = nullptr;

    switch (current_policy) {
        case AllocatorType::FIRST_FIT:
            chosen = first_fit_find(head, size);
            break;
        case AllocatorType::BEST_FIT:
            chosen = best_fit_find(head, size);
            break;
        case AllocatorType::WORST_FIT:
            chosen = worst_fit_find(head, size);
            break;
    }

    if (!chosen) return -1;
    return allocate_from_block(chosen, size);
}

// Core Deallocation Mechanism
/*
 coalesce(block)
 ---------------
 Merges a free block with adjacent free blocks.

 Coalescing is done in two steps:
 1. Merge with previous block if free
 2. Merge with next block if free
*/
static void coalesce(Block* blk) {
    // Merge with previous block
    if (blk->prev && blk->prev->free) {
        Block* prev = blk->prev;
        prev->size += blk->size;
        prev->next = blk->next;
        if (blk->next)
            blk->next->prev = prev;
        delete blk;
        blk = prev;
    }

    // Merge with next block
    if (blk->next && blk->next->free) {
        Block* nxt = blk->next;
        blk->size += nxt->size;
        blk->next = nxt->next;
        if (nxt->next)
            nxt->next->prev = blk;
        delete nxt;
    }
}

/*
 free_by_id(id)
 --------------
 Frees a previously allocated block using its allocation ID.
*/
bool free_by_id(int id) {
    Block* cur = head;
    while (cur) {
        if (!cur->free && cur->id == id) {
            cur->free = true;
            cur->id = -1;
            coalesce(cur);
            return true;
        }
        cur = cur->next;
    }
    return false; // Invalid ID
}

// Debug
/*
 dump_blocks()
 -------------
 Prints the current memory layout.
*/
void dump_blocks() {
    Block* cur = head;
    while (cur) {
        cout << "[0x" << hex << cur->start
             << " - 0x" << (cur->start + cur->size - 1)
             << "] ";
        cout << dec;
        if (cur->free)
            cout << "FREE";
        else
            cout << "USED (id=" << cur->id << ")";
        cout << '\n';
        cur = cur->next;
    }
}

// Accessors
Block* get_head() {
    return head;
}

size_t get_total_memory() {
    return TOTAL_MEMORY;
}

bool mem_free(int id) {
    return free_by_id(id);
}

void dump_memory() {
    dump_blocks();
}
