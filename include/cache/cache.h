#ifndef CACHE_H
#define CACHE_H

#include <cstddef>
#include <vector>

struct CacheLine {
    bool valid;          // Valid bit
    size_t tag;          // Tag extracted from address
    size_t timestamp;    // Used by FIFO
};

class Cache {
public:
    Cache(size_t cache_size,
          size_t block_size,
          size_t associativity);

    // Access a physical address
    // Returns true on HIT, false on MISS
    bool access(size_t physical_addr);

    // Statistics
    size_t get_hits() const;
    size_t get_misses() const;

private:
    size_t cache_size;
    size_t block_size;
    size_t associativity;
    size_t num_sets;

    size_t timer; // Global logical time

    // Cache sets: vector of sets, each set is a vector of cache lines
    std::vector<std::vector<CacheLine>> sets;

    size_t hits;
    size_t misses;

    // Address helpers
    size_t get_set_index(size_t addr) const;
    size_t get_tag(size_t addr) const;
};

#endif
