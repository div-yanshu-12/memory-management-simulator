#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H
using namespace std;
#pragma once
#include <vector>
#include <cstddef>

struct PageTableEntry {
    bool valid;
    size_t frame;
    size_t timestamp;
};

class VirtualMemory {
public:
    VirtualMemory(size_t num_pages,
                  size_t page_size,
                  size_t num_frames);

    size_t access(size_t vaddr, bool use_lru);
    void print_stats() const;

private:
    size_t pages;
    size_t page_size;
    size_t frames;
    size_t timer;

    size_t page_faults;
    size_t page_hits;

    vector<PageTableEntry> page_table;

    void handle_page_fault(size_t page, bool use_lru);
};

#endif 
