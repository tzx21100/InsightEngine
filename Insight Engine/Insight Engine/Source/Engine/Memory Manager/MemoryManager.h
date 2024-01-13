#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <algorithm>


namespace IS {


    class ParentAllocator {
    public:
        ParentAllocator(size_t size) : memorySize(size), usedMemorySize(0), firstAddress(nullptr) {}

        virtual ~ParentAllocator() {
            firstAddress = nullptr;
            usedMemorySize = 0;
            memorySize = 0;
        }

        virtual void* Allocate(size_t size) = 0;
        virtual void Free(void* ptr) = 0;

    protected:
        size_t memorySize;
        size_t usedMemorySize;
        void* firstAddress;

        static size_t CalculatePadding(size_t baseAddress, size_t alignment) {
            size_t multiplier = (baseAddress / alignment) + 1;
            size_t alignedAddress = multiplier * alignment;
            size_t padding = alignedAddress - baseAddress;
            return padding;
        }
    };


    class PoolAllocator : public ParentAllocator {
    public:
        PoolAllocator(size_t objectSize, size_t numObjects) : ParentAllocator(objectSize* numObjects), objectSize(objectSize) {
            assert(objectSize >= sizeof(void*)); // Ensure the object size can at least store a pointer

            firstAddress = ::operator new(memorySize);
            freeList = static_cast<void**>(firstAddress);

            void** current = freeList;
            for (size_t i = 0; i < numObjects - 1; ++i) {
                *current = static_cast<void*>(static_cast<char*>(firstAddress) + ((i + 1) * objectSize));
                current = static_cast<void**>(*current);
            }
            *current = nullptr; // End of the list
        }

        virtual ~PoolAllocator() {
            ::operator delete(firstAddress);
            firstAddress = nullptr;
        }

        void* Allocate(size_t size) override {
            assert(size == objectSize && "Allocation size must be equal to object size");

            if (freeList == nullptr) {
                return nullptr; // Pool is full
            }

            void* p = freeList;
            freeList = static_cast<void**>(*freeList);
            usedMemorySize += size;
            return p;
        }

        void* Allocate() {
            if (freeList == nullptr) {
                return nullptr; // Pool is full
            }

            void* p = freeList;
            freeList = static_cast<void**>(*freeList);
            usedMemorySize += objectSize;
            return p;
        }

        void Free(void* ptr) override {
            // Check if ptr is within the range of memory managed by the allocator
            char* start = static_cast<char*>(firstAddress);
            char* end = start + memorySize;
            char* checkPtr = static_cast<char*>(ptr);

            if (checkPtr >= start && checkPtr < end) {
                // Proceed with freeing the memory
                *((void**)ptr) = freeList;
                freeList = static_cast<void**>(ptr);
                usedMemorySize -= objectSize;
            }
            else {
                return;
            }
        }


    private:
        size_t objectSize;
        void** freeList;
    };

}