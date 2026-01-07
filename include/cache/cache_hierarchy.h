#ifndef CACHE_HIERARCHY_H
#define CACHE_HIERARCHY_H

#include "cache/cache.h"

class CacheHierarchy {
public:
    CacheHierarchy(Cache* l1, Cache* l2);

    // returns true if hit in any cache level
    bool access(size_t addr);

    void print_stats() const;

private:
    Cache* L1;
    Cache* L2;
};

#endif
