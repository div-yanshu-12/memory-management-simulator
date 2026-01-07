#include "virtual_memory/page_table.h"
#include <vector>
#include <limits>

using namespace std;

/*
 Page Replacement Algorithms
 Implements FIFO and LRU replacement policies for paging.

 NOTE:
 - Replacement operates on page table entries
 - Frame allocation is symbolic
*/

// Select victim page using FIFO
size_t select_fifo_victim(const vector<PageTableEntry>& pt) {
    size_t victim = 0;
    size_t oldest = numeric_limits<size_t>::max();

    for (size_t i = 0; i < pt.size(); i++) {
        if (pt[i].valid && pt[i].timestamp < oldest) {
            oldest = pt[i].timestamp;
            victim = i;
        }
    }
    return victim;
}

// Select victim page using LRU
size_t select_lru_victim(const vector<PageTableEntry>& pt) {
    size_t victim = 0;
    size_t least_recent = numeric_limits<size_t>::max();

    for (size_t i = 0; i < pt.size(); i++) {
        if (pt[i].valid && pt[i].timestamp < least_recent) {
            least_recent = pt[i].timestamp;
            victim = i;
        }
    }
    return victim;
}
