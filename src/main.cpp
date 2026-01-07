#include <iostream>
#include <sstream>
#include <string>

#include "allocator/allocator.h"
#include "stats.h"
#include "buddy/buddy.h"
#include "cache/cache.h"
#include "cache/cache_hierarchy.h"
#include "virtual_memory/page_table.h"
using namespace std;

VirtualMemory* vm = nullptr;


/*
 main.cpp – CLI Driver & System Integration
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    BuddyAllocator* buddy = nullptr;

    // Multilevel cache components
    Cache* l1_cache = nullptr;
    Cache* l2_cache = nullptr;
    CacheHierarchy* hierarchy = nullptr;

    cout << "Memory Management Simulator\n";
    cout << "Type commands, 'exit' to quit.\n";

    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;

        stringstream ss(line);
        string cmd;
        ss >> cmd;

        // EXIT
        if (cmd == "exit") {
            break;
        }

        // INITIALIZE PHYSICAL MEMORY
        // init memory <size>
        else if (cmd == "init") {
            string type;
            size_t size;
            ss >> type >> size;

            if (type == "memory") {
                init_memory(size);
                cout << "Initialized physical memory: "
                     << size << " bytes\n";
            }
        }

        // SET ALLOCATOR STRATEGY
        // set allocator first|best|worst
        else if (cmd == "set") {
            string what, alg;
            ss >> what >> alg;

            if (what == "allocator") {
                if (alg == "first")
                    set_allocator(AllocatorType::FIRST_FIT);
                else if (alg == "best")
                    set_allocator(AllocatorType::BEST_FIT);
                else if (alg == "worst")
                    set_allocator(AllocatorType::WORST_FIT);

                cout << "Allocator set to " << alg << " fit\n";
            }
        }

        // MALLOC
        // malloc <size>
        else if (cmd == "malloc") {
            size_t size;
            ss >> size;
            int id = mem_malloc(size);

            if (id == -1)
                cout << "Allocation failed\n";
            else
                cout << "Allocated block id=" << id << '\n';
        }

        // FREE
        // free <id>
        else if (cmd == "free") {
            int id;
            ss >> id;
            if (mem_free(id))
                cout << "Block freed\n";
            else
                cout << "Invalid block id\n";
        }

        // DUMP MEMORY
        // dump
        else if (cmd == "dump") {
            dump_memory();
        }

        // STATS
        // stats
        else if (cmd == "stats") {
            print_stats();
        }

        // BUDDY ALLOCATOR
        else if (cmd == "buddy") {
            string sub;
            ss >> sub;

            if (sub == "init") {
                size_t total, minb;
                ss >> total >> minb;
                delete buddy;
                buddy = new BuddyAllocator(total, minb);
                cout << "Buddy allocator initialized\n";
            }
            else if (sub == "alloc") {
                size_t sz;
                ss >> sz;
                long long addr = buddy->alloc(sz);
                if (addr == -1)
                    cout << "Buddy allocation failed\n";
                else
                    cout << "Buddy allocated at address "
                         << addr << '\n';
            }
            else if (sub == "free") {
                size_t addr;
                ss >> addr;
                buddy->free_block(addr);
                cout << "Buddy block freed\n";
            }
            else if (sub == "dump") {
                buddy->dump();
            }

        }
        // VIRTUAL MEMORY
        // vm init <vmem_size> <page_size>
        // vm access <vaddr>
        // vm stats
        else if (cmd == "vm") {
            string sub;
            ss >> sub;

            if (sub == "init") {
                size_t vmem, page;
                ss >> vmem >> page;

                size_t num_pages = vmem / page;
                size_t num_frames = num_pages / 2; // symbolic physical memory

                delete vm;
                vm = new VirtualMemory(num_pages, page, num_frames);
                cout << "Virtual memory initialized\n";
            }
            else if (sub == "access") {
                size_t addr;
                ss >> addr;

                if (!vm) {
                    cout << "Virtual memory not initialized\n";
                } else {
                    size_t pa = vm->access(addr, true);
                    cout << "Physical address: " << pa << '\n';
                }
            }
            else if (sub == "stats") {
                if (!vm) {
                    cout << "Virtual memory not initialized\n";
                } else {
                    vm->print_stats();
                }
            }
        }

        // MULTILEVEL CACHE
        // cache init l1 <size> <block> <assoc>
        // cache init l2 <size> <block> <assoc>
        // cache access <addr>
        // cache stats
        else if (cmd == "cache") {
            string sub;
            ss >> sub;

            if (sub == "init") {
                string level;
                size_t c, b, a;
                ss >> level >> c >> b >> a;

                if (level == "l1") {
                    delete l1_cache;
                    l1_cache = new Cache(c, b, a);
                    cout << "L1 cache initialized\n";
                }
                else if (level == "l2") {
                    delete l2_cache;
                    l2_cache = new Cache(c, b, a);
                    cout << "L2 cache initialized\n";
                }

                if (l1_cache && l2_cache && !hierarchy) {
                    hierarchy = new CacheHierarchy(l1_cache, l2_cache);
                    cout << "Cache hierarchy created (L1 -> L2)\n";
                }
            }

            else if (sub == "access") {
                size_t addr;
                ss >> addr;

                if (!hierarchy) {
                    cout << "Cache hierarchy not initialized\n";
                } else {
                    hierarchy->access(addr);
                }
            }

            else if (sub == "stats") {
                if (!hierarchy) {
                    cout << "Cache hierarchy not initialized\n";
                } else {
                    hierarchy->print_stats();
                }
            }
        }

        else {
            cout << "Unknown command\n";
        }
    }

    delete buddy;
    delete hierarchy;
    delete l1_cache;
    delete l2_cache;

    return 0;
}
