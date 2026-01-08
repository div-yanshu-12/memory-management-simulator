Please note, the simulator separates allocation, virtual memory, and caching into independent subsystems to reflect real OS architecture. These subsystems interact only during memory access, which is modeled via a unified access command that performs address translation followed by cache lookup

.......


1. Overview

This project implements a user-space simulator for Operating System memory management concepts. It models physical memory allocation, fragmentation behavior, cache memory, and virtual memory using paging.

The simulator is not a real operating system or kernel. All memory addresses are simulated as offsets, and timing behavior is symbolic rather than cycle-accurate.

2. Assumptions & Simplifications

The following assumptions are made to keep the simulator focused and manageable:

Memory is byte-addressable

Physical memory is contiguous

Single-process, single-threaded execution

No memory protection bits (read/write/execute)

No TLB simulation

Cache and disk latency are symbolic (hit/miss counting only)


3. Physical Memory Model

Physical memory is represented as a doubly linked list of memory blocks.

Each block contains:

Start address (byte offset)

Size (in bytes)

Allocation state (free/used)

Allocation ID

Blocks are always maintained in sorted address order, which enables:

Block splitting during allocation

Block coalescing during deallocation


4. Dynamic Allocation Strategies

The simulator supports three allocation policies. Allocation policy is separated from allocation mechanism.

4.1 First Fit

Selects the first free block large enough to satisfy a request.

Fast

May cause external fragmentation near the beginning of memory

4.2 Best Fit

Selects the smallest free block that satisfies the request.

Minimizes internal wastage

Often increases external fragmentation due to small leftover blocks

4.3 Worst Fit

Selects the largest available free block.

Leaves larger remaining free blocks

Higher traversal cost

Block splitting and coalescing are performed automatically, independent of policy.

5. Buddy Memory Allocation

An independent buddy allocation system is implemented.

Features:

Memory managed in power-of-two block sizes

Recursive splitting on allocation

XOR-based buddy address computation

Recursive coalescing on free

This allocator trades internal fragmentation for fast allocation and deterministic merging.

6. Cache Simulation
6.1 Cache Model

The simulator implements a two-level cache hierarchy (L1 and L2).

Each cache level supports:

Configurable total size

Configurable block size

Configurable associativity (direct-mapped or set-associative)

Each cache level is an independent cache instance with its own metadata and statistics.

6.2 Replacement Policies

FIFO (mandatory) – implemented

LRU (optional) – implemented

LFU – not implemented

Replacement is performed at the cache-set level.

6.3 Multilevel Cache Behavior

On memory access:

L1 cache is checked first

On L1 miss, access propagates to L2

On L2 miss, data is fetched from simulated main memory

On L2 hit, data is promoted to L1

This models miss penalty propagation between cache levels.

6.4 Cache Statistics

The simulator tracks:

Cache hits per level

Cache misses per level

Hit ratio is derivable from these values but not explicitly printed.

7. Virtual Memory & Paging

Virtual memory is implemented using paging.

Features:

Fixed page size

Virtual address split into page number and offset

Page table with valid bit and frame mapping

Physical memory divided into frames

7.1 Page Fault Handling

On a page fault:

A free frame is used if available

Otherwise, a victim page is selected using FIFO or LRU

The victim page is evicted

The required page is loaded into memory

Page table entries are updated

7.2 Statistics

The simulator tracks:

Page hits

Page faults

Disk access latency is symbolically represented by page fault counting.

7.3 Integration with Cache (Conceptual)

The intended execution order is:

Virtual Address → Page Table → Physical Address → Cache → Physical Memory


In the current implementation:

Virtual memory translation and cache access are implemented as separate modules

The execution order is conceptually documented, but not enforced through a single unified command

This design allows independent testing of each subsystem.

8. Statistics & Metrics

The simulator reports:

Total memory

Used memory

Free memory

Memory utilization

External fragmentation

Cache hits and misses

Page hits and page faults

Internal fragmentation is zero in the linked-list allocator since allocations are exact-sized.

9. Command-Line Interface Summary
Memory Allocation
init memory <size>
set allocator first|best|worst
malloc <size>
free <id>
dump
stats

Buddy Allocator
buddy init <total> <min_block>
buddy alloc <size>
buddy free <address>
buddy dump

Cache
cache init l1 <size> <block> <assoc>
cache init l2 <size> <block> <assoc>
cache access <physical_address>
cache stats

Virtual Memory
vm init <memory> <page_size>
vm access <virtual_address>
vm stats

10. Limitations

No TLB simulation

No multi-process page tables

No automatic VM–cache execution chaining

No write-back cache modeling

No real timing or latency simulation

11. Conclusion

This project provides a modular, extensible simulator for core OS memory management concepts. It accurately models allocation strategies, fragmentation behavior, paging, and multilevel cache behavior, while maintaining clear separation between components and policies.