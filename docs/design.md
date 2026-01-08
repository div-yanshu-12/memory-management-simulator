Please note, the simulator separates allocation, virtual memory, and caching into independent subsystems to reflect real OS architecture. These subsystems interact only during memory access, which is modeled via a unified access command that performs address translation followed by cache lookup

.......


1. Overview

This project implements a user-space simulator for core Operating System memory management concepts. It models physical memory allocation, fragmentation behavior, buddy allocation, virtual memory using paging, and a multilevel CPU cache hierarchy.

The simulator is not a real operating system or kernel. All memory addresses are simulated as byte offsets, and all timing behavior is symbolic rather than cycle-accurate.

2. Assumptions & Simplifications

To keep the simulator focused and understandable, the following assumptions are made:

Memory is byte-addressable

Physical memory is contiguous

Single process, single-threaded execution

No memory protection bits (read/write/execute)

No Translation Lookaside Buffer (TLB) simulation

Cache and disk latency are symbolic (hit/miss counting only)

Disk storage is not explicitly modeled; page faults represent disk access

These simplifications are intentional and documented.

3. Physical Memory Model (Linear Allocator)

Physical memory is represented as a doubly linked list of memory blocks.

Each block stores:

Start address (byte offset)

Size (bytes)

Allocation state (free or used)

Allocation ID

Blocks are always maintained in sorted address order, enabling:

Block splitting during allocation

Block coalescing during deallocation

This models a classical heap-style allocator.

4. Dynamic Allocation Strategies

Allocation policy is separated from the allocation mechanism. The simulator supports three strategies:

4.1 First Fit

Selects the first free block large enough to satisfy a request.

Fast

Can cause external fragmentation near low addresses

4.2 Best Fit

Selects the smallest free block that satisfies the request.

Minimizes internal wastage

May increase external fragmentation due to small leftover blocks

4.3 Worst Fit

Selects the largest available free block.

Leaves larger free regions

Higher traversal cost

Block splitting and coalescing occur automatically, independent of policy.

5. Buddy Memory Allocation

An independent buddy allocator is implemented, separate from the linear allocator.

Features:

Memory managed in power-of-two block sizes

Recursive splitting on allocation

XOR-based buddy address computation

Recursive coalescing on free

This allocator trades internal fragmentation for fast allocation and deterministic merging, closely resembling kernel-level allocators.

6. Cache Simulation
6.1 Cache Model

The simulator implements a two-level cache hierarchy (L1 and L2).

Each cache level supports:

Configurable total size

Configurable block (cache line) size

Configurable associativity (direct-mapped or set-associative)

Each cache level is an independent cache instance with its own metadata and statistics.

6.2 Replacement Policies

FIFO (mandatory) – implemented

LRU (optional) – implemented

LFU – not implemented

Replacement decisions are made at the cache-set level.

6.3 Multilevel Cache Behavior

On a memory access:

L1 cache is checked first

On L1 miss, access propagates to L2

On L2 miss, data is fetched from simulated main memory

On L2 hit, the block is promoted to L1

This models miss penalty propagation between cache levels.

6.4 Cache Statistics

The simulator tracks:

Cache hits per level

Cache misses per level

Only CPU-initiated accesses are counted.
Internal cache fill operations do not affect statistics, matching real hardware behavior.

7. Virtual Memory & Paging

Virtual memory is implemented using paging.

Features:

Fixed page size

Virtual address split into page number and offset

Page table with valid bit and frame mapping

Physical memory conceptually divided into frames

Virtual memory is an address translation mechanism, not a storage system.

7.1 Page Fault Handling

On a page fault:

A free frame is used if available

Otherwise, a victim page is selected using FIFO or LRU

The victim page is evicted

The required page is loaded

Page table entries are updated

Disk access is symbolically represented via page fault counting.

7.2 Virtual Memory Statistics

The simulator tracks:

Page hits

Page faults

These values represent demand paging behavior.

7.3 Integration with Cache (Actual Implementation)

The simulator enforces the following execution order during memory access:

Virtual Address → Page Table → Physical Address → Cache → Physical Memory


This order is implemented through a unified access command, which:

Performs virtual-to-physical address translation

Propagates the physical address through the cache hierarchy

Allocation subsystems remain independent and do not implicitly trigger paging or cache activity.

8. Statistics & Metrics

The simulator reports:

Total memory

Used memory

Free memory

Memory utilization

External fragmentation

Cache hits and misses

Page hits and page faults

Internal fragmentation is zero in the linear allocator because allocations are exact-sized.

9. Command-Line Interface Summary
Linear Memory Allocation
init memory <size>
set allocator first|best|worst
malloc <size>
free <id>
dump
stats

Buddy Allocator
buddy init <total_memory> <min_block_size>
buddy alloc <size>
buddy free <address>
buddy dump

Cache
cache init l1 <size> <block> <assoc>
cache init l2 <size> <block> <assoc>
cache stats

Virtual Memory
vm init <virtual_memory_size> <page_size>
access <virtual_address>
vm stats

10. Limitations

No TLB simulation

No multi-process page tables

No write-back cache modeling

No real timing or latency simulation

No explicit disk storage model

These limitations are intentional.

11. Conclusion

This project provides a modular and accurate simulator for core OS memory management concepts. It correctly models allocation strategies, fragmentation behavior, buddy allocation, paging, and multilevel cache behavior, while maintaining clear separation between subsystems and enforcing a realistic execution order during memory access.