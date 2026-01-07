#include "cache/cache_hierarchy.h"
#include <iostream>

using namespace std;

CacheHierarchy::CacheHierarchy(Cache* l1, Cache* l2)
    : L1(l1), L2(l2) {}

bool CacheHierarchy::access(size_t addr) {
    // Try L1
    if (L1->access(addr)) {
        return true; // L1 hit
    }

    // L1 miss → Try L2
    if (L2->access(addr)) {
        // L2 hit → bring block into L1
        L1->access(addr);
        return true;
    }

    // Miss in both → fetch from memory
    L2->access(addr); // fill L2
    L1->access(addr); // fill L1
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
