#pragma once

#include "MemoryCell.h"

#include <iosfwd>
#include <stdexcept>
#include <vector>
#include <set>

namespace gc {

class OutOfMemoryException : public std::exception {};

class Heap {
private:
    using SemiSpace = std::vector<MemoryCell>;
    SemiSpace memoryPool;

public:
    Heap(std::size_t);

    std::size_t capacity() const;
    std::size_t size() const;
    
    //push the memoryCell into the memoryPool and return its address in the heap
    MemoryCell& push_back(MemoryCell);
    
    friend std::ostream& operator<<(std::ostream&, const Heap&);
};

std::ostream& operator<<(std::ostream&, const Heap&);
}
