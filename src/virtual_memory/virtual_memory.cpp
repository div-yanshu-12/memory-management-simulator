#include "virtual_memory/page_table.h"
#include <iostream>

using namespace std;

// Replacement helpers
size_t select_fifo_victim(const vector<PageTableEntry>& pt);
size_t select_lru_victim(const vector<PageTableEntry>& pt);

VirtualMemory::VirtualMemory(size_t num_pages,
                             size_t psize,
                             size_t num_frames)
    : pages(num_pages),
      page_size(psize),
      frames(num_frames),
      timer(0),
      page_faults(0),
      page_hits(0) {

    page_table.resize(num_pages, {false, 0, 0});
}

size_t VirtualMemory::access(size_t vaddr, bool use_lru) {
    timer++;

    size_t page = vaddr / page_size;
    size_t offset = vaddr % page_size;

    if (!page_table[page].valid) {
        page_faults++;
        handle_page_fault(page, use_lru);
    } else {
        page_hits++;
    }

    page_table[page].timestamp = timer;
    return page_table[page].frame * page_size + offset;
}

void VirtualMemory::print_stats() const {
    cout << "Page hits: " << page_hits << '\n';
    cout << "Page faults: " << page_faults << '\n';
}

void VirtualMemory::handle_page_fault(size_t page, bool use_lru) {
    // Look for free frame
    for (size_t i = 0; i < frames; i++) {
        bool used = false;
        for (auto &pte : page_table)
            if (pte.valid && pte.frame == i)
                used = true;

        if (!used) {
            page_table[page] = {true, i, timer};
            return;
        }
    }

    // No free frame → replacement
    size_t victim = use_lru
        ? select_lru_victim(page_table)
        : select_fifo_victim(page_table);

    page_table[page] = {
        true,
        page_table[victim].frame,
        timer
    };
}
