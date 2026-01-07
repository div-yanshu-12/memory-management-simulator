#include "cache/cache.h"
#include <algorithm>

using namespace std;

/*
 LRU (Least Recently Used):
 - Evicts the cache line that was accessed least recently
*/

bool Cache::access(size_t physical_addr) {
    timer++;
    size_t set_idx = get_set_index(physical_addr);
    size_t tag = get_tag(physical_addr);

    // HIT check
    for (auto &line : sets[set_idx]) {
        if (line.valid && line.tag == tag) {
            hits++;
            line.timestamp = timer; // Update recency
            return true;
        }
    }

    // MISS
    misses++;
    auto &set = sets[set_idx];

    auto it = min_element(set.begin(), set.end(),
        [](const CacheLine &a, const CacheLine &b) {
            return a.timestamp < b.timestamp;
        });

    *it = {true, tag, timer};
    return false;
}
