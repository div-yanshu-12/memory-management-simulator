#include "cache/cache_hierarchy.h"
#include <iostream>

using namespace std;

CacheHierarchy::CacheHierarchy(Cache* l1, Cache* l2)
    : L1(l1), L2(l2) {}

bool CacheHierarchy::access(size_t addr) {
    // CPU → L1
    if (L1->access(addr, true)) {
        return true; // L1 hit
    }

    // L1 miss → L2
    if (L2->access(addr, true)) {
        // Fill L1, but DO NOT count stats
        L1->access(addr, false);
        return true;
    }

    // Miss in both → fetch from memory
    // Fill caches WITHOUT counting stats
    L2->access(addr, false);
    L1->access(addr, false);

    return false;
}


void CacheHierarchy::print_stats() const {
    cout << "L1 Cache:\n";
    cout << "  Hits: " << L1->get_hits() << "\n";
    cout << "  Misses: " << L1->get_misses() << "\n";

    cout << "L2 Cache:\n";
    cout << "  Hits: " << L2->get_hits() << "\n";
    cout << "  Misses: " << L2->get_misses() << "\n";
}
