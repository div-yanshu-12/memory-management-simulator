# Memory Management Simulator

## Overview
This project is a user-space simulator that models core Operating System memory
management concepts, including dynamic allocation, fragmentation, caching,
and virtual memory using paging.

The simulator is educational in nature and focuses on correctness of behavior
rather than real hardware interaction.

---

## Features

### Physical Memory Management
- Byte-addressable contiguous memory model
- Linked-list based allocator
- Allocation strategies:
  - First Fit
  - Best Fit
  - Worst Fit
- Automatic block splitting and coalescing
- Fragmentation metrics

### Buddy Allocator
- Power-of-two allocation
- Recursive splitting
- XOR-based buddy computation
- Recursive merging on free

### Cache Simulation
- Two-level cache hierarchy (L1 → L2)
- Configurable size, block size, associativity
- Replacement policies:
  - FIFO
  - LRU
- Hit/miss statistics per level

### Virtual Memory
- Paging-based virtual memory
- Page table implementation
- FIFO / LRU page replacement
- Page fault and page hit tracking

---

## Build Instructions

```bash
make clean
make
