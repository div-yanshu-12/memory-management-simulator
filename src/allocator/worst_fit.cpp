#include "allocator/block.h"

Block* worst_fit_find(Block* head, size_t size) {
    Block* worst = nullptr;
    size_t worst_size = 0;

    for (Block* cur = head; cur; cur = cur->next) {
        if (cur->free && cur->size >= size && cur->size > worst_size) {
            worst = cur;
            worst_size = cur->size;
        }
    }
    return worst;
}
