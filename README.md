# Memory Management Simulator

# Overview
This project is a user-space simulator that models core Operating System memory
management concepts, including dynamic allocation, fragmentation, caching,
and virtual memory using paging.
The simulator is educational in nature and focuses on correctness of behavior
rather than real hardware interaction.

# Features

# Physical Memory Management
Byte-addressable contiguous memory model
Linked-list based allocator

Allocation strategies:
First Fit
Best Fit
Worst Fit
Automatic block splitting and coalescing
Fragmentation metrics

# Buddy Allocator
Power-of-two allocation
Recursive splitting
XOR-based buddy computation
Recursive merging on free

# Cache Simulation
Two-level cache hierarchy (L1 → L2)
Configurable cache size
Configurable block (cache line) size
Configurable associativity
Replacement policies:
FIFO
LRU
Hit and miss statistics per cache level

# Virtual Memory
Paging-based virtual memory
Page table implementation
FIFO / LRU page replacement
Page fault and page hit tracking
Demand paging behavior

# Build Instructions
make clean
make
The build produces the executable:
./memsim

# Running the Simulator
./memsim
The simulator runs in an interactive command-line interface.

# Command Summary
# Physical Memory (Linear Allocator)
init memory <size>
set allocator first|best|worst
malloc <size>
free <id>
dump
stats

# uddy Allocator
buddy init <total_memory> <min_block_size>
buddy alloc <size>
buddy free <address>
buddy dump

# Cache Simulation
cache init l1 <size> <block_size> <associativity>
cache init l2 <size> <block_size> <associativity>
cache stats

# Virtual Memory
vm init <virtual_memory_size> <page_size>
access <virtual_address>
vm stats

# Notes
Allocation does not trigger paging or cache activity
Paging and cache behavior occur only on memory access
Subsystems are modular and explicitly initialized