#include "allocator/block.h"

// Returns the first suitable free block
Block* first_fit_find(Block* head, size_t size) {
    for (Block* cur = head; cur; cur = cur->next) {
        if (cur->free && cur->size >= size)
            return cur;
    }
    return nullptr;
}
