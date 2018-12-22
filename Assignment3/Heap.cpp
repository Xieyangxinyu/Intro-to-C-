#include "Heap.h"

#include <ostream>

namespace gc {

Heap::Heap(std::size_t n) {
    memoryPool.reserve(n);
}

std::size_t Heap::size() const {
    return memoryPool.size();
}

std::size_t Heap::capacity() const {
    return memoryPool.capacity();
}
    
MemoryCell& Heap::push_back(MemoryCell c){
    memoryPool.push_back(c);
    return memoryPool.back();
}

std::ostream& operator<<(std::ostream& os, const Heap& heap) {
    os << "Heap( ";
    auto const& memPool = heap.memoryPool;
    auto printHeapCell = [&os](const MemoryCell& cell) {
        os << std::hex << &cell << ": " << std::dec;
        os << cell;
    };

    if (!memPool.empty()) {
        printHeapCell(memPool.front());
        for (std::size_t i = 1, e = memPool.size(); i < e; ++i) {
            os << "; ";
            printHeapCell(memPool[i]);
        }
    }
    os << " )";

    return os;
}
}
