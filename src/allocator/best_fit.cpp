#include "allocator/block.h"
#include <cstddef>
#include <limits>

using namespace std;

Block* best_fit_find(Block* head, size_t size) {
    Block* best = nullptr;
    size_t best_size = numeric_limits<size_t>::max();

    for (Block* cur = head; cur; cur = cur->next) {
        if (cur->free && cur->size >= size && cur->size < best_size) {
            best = cur;
            best_size = cur->size;
        }
    }
    return best;
}
