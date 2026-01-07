#include "cache/cache.h"
#include <algorithm>

using namespace std;

/*
 FIFO (First-In-First-Out):
 - Evicts the cache line that was inserted earliest
*/

Cache::Cache(size_t csize, size_t bsize, size_t assoc)
    : cache_size(csize), block_size(bsize), associativity(assoc), timer(0),
      hits(0), misses(0) {

    num_sets = (cache_size / block_size) / associativity;
    sets.resize(num_sets, vector<CacheLine>(associativity, {false, 0, 0}));
}

size_t Cache::get_set_index(size_t addr) const {
    size_t block_addr = addr / block_size;
    return block_addr % num_sets;
}

size_t Cache::get_tag(size_t addr) const {
    size_t block_addr = addr / block_size;
    return block_addr / num_sets;
}

bool Cache::access(size_t physical_addr) {
    timer++;
    size_t set_idx = get_set_index(physical_addr);
    size_t tag = get_tag(physical_addr);

    // Check for HIT
    for (auto &line : sets[set_idx]) {
        if (line.valid && line.tag == tag) {
            hits++;
            return true;
        }
    }

    // MISS: replace FIFO
    misses++;
    auto &set = sets[set_idx];

    auto it = min_element(set.begin(), set.end(),
        [](const CacheLine &a, const CacheLine &b) {
            return a.timestamp < b.timestamp;
        });

    *it = {true, tag, timer};
    return false;
}

size_t Cache::get_hits() const {
    return hits;
}

size_t Cache::get_misses() const {
    return misses;
}
