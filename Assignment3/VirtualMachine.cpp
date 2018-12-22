#include "VirtualMachine.h"

#include <iostream>
#include <set>
#include <map>

namespace gc {

VirtualMachine::VirtualMachine(std::size_t heapCapacity) : heap(heapCapacity), heapCapacity(heapCapacity) {}
    
void VirtualMachine::pushInt(std::int64_t value) {
    if(getHeapSize() + 2 > heapCapacity)
        gc();
    if(getHeapSize() + 2 > heapCapacity)
        throw OutOfMemoryException();
    else{
        MemoryCell& p = heap.push_back(MemoryCell(CellTag :: Int));
        heap.push_back(MemoryCell(value));
        stack.push_back(&p);
    }
}
    
void VirtualMachine::pushPair() {
    if(getStackSize() < 2)
        throw StackException();
    else{
        if(getHeapSize() + 3 > heapCapacity)
            gc();
        if(getHeapSize() + 3 > heapCapacity)
            throw OutOfMemoryException();
        else{
            MemoryCell *p2 = stack.back(); stack.pop_back();
            MemoryCell *p1 = stack.back(); stack.pop_back();
            MemoryCell& p0 = heap.push_back(MemoryCell(CellTag :: Pair));
            stack.push_back(&p0);
            heap.push_back(MemoryCell(p1));
            heap.push_back(MemoryCell(p2));
        }
    }
}

void VirtualMachine::pop() {
    // Implement this function
    if(getStackSize() < 1)
        throw StackException();
    else
        stack.pop_back();
}

void VirtualMachine::dup() {
    // Implement this function
    if(getStackSize() < 1)
        throw StackException();
    else{
        MemoryCell *p = stack.back();
        stack.push_back(p);
    }
}

void VirtualMachine::loadFirst() {
    // Implement this function
    if(getStackSize() < 1)
        throw StackException();
    else{
        MemoryCell *p1 = stack.back(); stack.pop_back();
        if(p1-> getCellType() != CellType :: Tag || p1 -> getTag() != CellTag::Pair)
            throw RuntimeTypeError();
        else{
            p1 ++;
            stack.push_back((*p1).getPointer());
        }
    }
}

void VirtualMachine::loadSecond() {
    // Implement this function
    if(getStackSize() < 1)
        throw StackException();
    else{
        MemoryCell *p1 = stack.back(); stack.pop_back();
        if(p1-> getCellType() != CellType :: Tag || p1 -> getTag() != CellTag::Pair)
            throw RuntimeTypeError();
        else{
            p1 += 2;
            stack.push_back((*p1).getPointer());
        }
    }
}
    
void VirtualMachine::storeFirst() {
    // Implement this function
    if(getStackSize() < 2)
        throw StackException();
    else{
        MemoryCell *p2 = stack.back(); stack.pop_back();
        MemoryCell *p1 = stack.back();
        if(p1-> getCellType() != CellType :: Tag || p1 -> getTag() != CellTag::Pair)
            throw RuntimeTypeError();
        else{
            p1 ++;
            *p1 = p2;
        }
    }
}

void VirtualMachine::storeSecond() {
    // Implement this function
    if(getStackSize() < 2)
        throw StackException();
    else{
        MemoryCell *p2 = stack.back(); stack.pop_back();
        MemoryCell *p1 = stack.back();
        if(p1-> getCellType() != CellType :: Tag || p1 -> getTag() != CellTag::Pair)
            throw RuntimeTypeError();
        else{
            p1 += 2;
            *p1 = p2;
        }
    }
}
    
void VirtualMachine::add() {
    if(getStackSize() < 2)
        throw StackException();
    else{
        if(getHeapSize() + 1 > heapCapacity)
            gc();
        if(getHeapSize() + 1 > heapCapacity)
            throw OutOfMemoryException();
        else{
            MemoryCell *p2 = stack.back(); stack.pop_back();
            MemoryCell *p1 = stack.back(); stack.pop_back();
            if(p2 -> getCellType() != CellType :: Tag || p2 -> getTag() != CellTag::Int || p1-> getCellType() != CellType :: Tag || p1 -> getTag() != CellTag::Int)
                throw RuntimeTypeError();
            else{
                p2 ++; p1 ++;
                pushInt(p2 -> getValue() + p1 -> getValue());
            }
        }
    }
}

//recursively add all reachable objects into the rootSet
void VirtualMachine::pointerAdd(MemoryCell& c, std::set<MemoryCell*>& rootSet){
    switch (c.getCellType()) {
        case CellType::Tag:{
            switch (c.getTag()) {
                case CellTag :: Int:{
                    MemoryCell *p1 = &c;
                    rootSet.insert(p1 + 1);
                }
                    break;
                case CellTag :: Pair:{
                    MemoryCell *p1 = &c;
                    if(!rootSet.count(p1 + 1)){
                        rootSet.insert(p1 + 1);
                        MemoryCell *p = (p1 + 1);
                        pointerAdd(*p, rootSet);
                    }
                    if(!rootSet.count(p1 + 2)){
                        rootSet.insert(p1 + 2);
                        MemoryCell *p = (p1 + 2);
                        pointerAdd(*p, rootSet);
                    }
                }
                    break;
            }
        }
            break;
        case CellType::Pointer:{
            MemoryCell *p1 = c.getPointer();
            if(!rootSet.count(p1)){
                rootSet.insert(p1);
                pointerAdd(*p1, rootSet);
            }
        }
            break;
        default:
            break;
    }
}
//Garbage Collector by generating new heaps and stacks
void VirtualMachine::gc() {
    std::set<MemoryCell*> rootSet;
    for(auto itr = stack.begin(); itr != stack.end(); ++ itr){
        rootSet.insert(*itr);
        MemoryCell *p1 = *itr;
        pointerAdd(*p1, rootSet);
    }
    Heap newHeap(heapCapacity);
    std::map<MemoryCell*, MemoryCell*> rootMap;
    for(auto itr = rootSet.begin(); itr != rootSet.end(); ++ itr){
        MemoryCell *p1 = *itr;
        if(p1 -> getCellType() == CellType::Pointer)
            p1 -> setPointer(rootMap[p1 -> getPointer()]);
        rootMap[p1] = &newHeap.push_back(*p1);
    }
    Stack newStack;
    for(auto itr = stack.begin(); itr != stack.end(); ++ itr){
        MemoryCell *p1 = *itr;
        newStack.push_back(rootMap[p1]);
    }
    std::swap(heap, newHeap);
    std::swap(stack, newStack);
}

const MemoryCell& VirtualMachine::getStackReference(std::size_t addr) const {
    if (stack.size() <= addr)
        throw StackException();
    return *stack[addr];
}

size_t VirtualMachine::getStackSize() const {
    return stack.size();
}

size_t VirtualMachine::getHeapSize() const {
    return heap.size();
}

std::ostream& operator<<(std::ostream& os, const VirtualMachine& vm) {
    os << vm.stack << '\n' << vm.heap << '\n';
    return os;
}
}
