
# Makefile – Memory Management Simulator
# Builds the complete simulator with all modules
# Usage:
#   make        -> build
#   make clean  -> remove binaries

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

INCLUDES = -Iinclude

SRC = \
    src/main.cpp \
    src/allocator/allocator_core.cpp \
    src/allocator/first_fit.cpp \
    src/allocator/best_fit.cpp \
    src/allocator/worst_fit.cpp \
    src/stats.cpp \
    src/buddy/buddy.cpp \
    src/cache/cache_fifo.cpp \
    src/cache/cache_hierarchy.cpp \
    src/virtual_memory/page_replacement.cpp \
    src/virtual_memory/virtual_memory.cpp

TARGET = memsim

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

clean:
	rm -f $(TARGET)
