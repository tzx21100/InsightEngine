/*!
* \file MemoryManager.h
* \author  Tan Zheng Xun, t.zhengxun@digipen.edu
* \par Course: CSD2401
* \date 08-02-2024
* \brief
* This file declares and defines the functions of the parent allocator, pool
* allocator that we will be using for our ECS system's components.  The
* parent allocator has been defined in the case that we would switch to a different
* form of memory allocation next time.
* 
* \copyright
* All content (C) 2023 DigiPen Institute of Technology Singapore.
* All rights reserved.
* Reproduction or disclosure of this file or its contents without the prior written
* consent of DigiPen Institute of Technology is prohibited.
*____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_MEMORYMANAGER_H
#define GAM200_INSIGHT_ENGINE_MEMORYMANAGER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <algorithm>


namespace IS {

    /**
     * @brief Abstract base class for memory allocation.
     *
     * Defines the interface and common functionality for custom memory allocators,
     * including storing the size of allocated memory, used memory, and the starting
     * address of the allocated memory block.
     */
    class ParentAllocator {
    public:

        /**
         * @brief Constructs a ParentAllocator with a specified memory size.
         *
         * @param size The total size of the memory to be managed by the allocator.
         */
        ParentAllocator(size_t size) : memorySize(size), usedMemorySize(0), firstAddress(nullptr) {}

        /**
         * @brief Virtual destructor to clean up the allocator.
         */
        virtual ~ParentAllocator() {
            firstAddress = nullptr;
            usedMemorySize = 0;
            memorySize = 0;
        }

        /**
         * @brief Allocates a block of memory of a specified size.
         *
         * @param size The size of the memory block to allocate.
         * @return void* Pointer to the allocated memory block, or nullptr if allocation fails.
         */
        virtual void* Allocate(size_t size) = 0;

        /**
         * @brief Frees a previously allocated block of memory.
         *
         * @param ptr Pointer to the memory block to be freed.
         */
        virtual void Free(void* ptr) = 0;

    protected:
        size_t memorySize;
        size_t usedMemorySize;
        void* firstAddress;

        /**
         * @brief Calculates padding needed for a given address to satisfy an alignment requirement.
         *
         * @param baseAddress The starting address to align.
         * @param alignment The alignment requirement (must be a power of two).
         * @return size_t The padding size needed to meet the alignment.
         */
        static size_t CalculatePadding(size_t baseAddress, size_t alignment) {
            size_t multiplier = (baseAddress / alignment) + 1;
            size_t alignedAddress = multiplier * alignment;
            size_t padding = alignedAddress - baseAddress;
            return padding;
        }
    };

    /**
     * @brief A memory allocator that manages memory in fixed-size blocks, optimized for quickly allocating and freeing objects of a single size.
     */
    class PoolAllocator : public ParentAllocator {
    public:

        /**
         * @brief Constructs a PoolAllocator for objects of a specified size and quantity.
         *
         * @param objectSize The size of each object in the pool.
         * @param numObjects The number of objects to accommodate in the pool.
         */
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

        /**
         * @brief Destructor to clean up allocated memory blocks.
         */
        virtual ~PoolAllocator() {
            ::operator delete(firstAddress);
            firstAddress = nullptr;
        }


        /**
         * @brief Allocates a block of memory of the object size specified at construction.
         *
         * @param size The size of the memory block to allocate (must match the object size).
         * @return void* Pointer to the allocated memory block, or nullptr if allocation fails or if the size does not match.
         */
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

        /**
         * @brief Allocates a block of memory of the object size, without specifying size.
         *
         * This method is for convenience, directly calling Allocate with the object size.
         * @return void* Pointer to the allocated memory block, or nullptr if the pool is full.
         */
        void* Allocate() {
            if (freeList == nullptr) {
                ExpandPool();
            }

            return InternalAllocate();
        }

        /**
          * @brief Frees a previously allocated block of memory, returning it to the pool.
          *
          * @param ptr Pointer to the memory block to be freed.
          */
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
        std::vector<void*> blocks; // Stores all the memory blocks

        /**
         * @brief Expands the pool by allocating a new block of memory the same size as initially specified.
         */
        void ExpandPool() {
            void* newBlock = ::operator new(memorySize);
            blocks.push_back(newBlock); // Keep track of the new block

            freeList = static_cast<void**>(newBlock);

            // Initialize the free list for the new block
            void** current = freeList;
            for (size_t i = 0; i < (memorySize / objectSize) - 1; ++i) {
                *current = static_cast<void*>(static_cast<char*>(newBlock) + ((i + 1) * objectSize));
                current = static_cast<void**>(*current);
            }
            *current = nullptr; // End of the list
        }

        /**
         * @brief Helper method to allocate a block of memory from the pool, updating the free list.
         *
         * @return void* Pointer to the allocated memory block.
         */
        void* InternalAllocate() {
            void* p = freeList;
            freeList = static_cast<void**>(*freeList);
            usedMemorySize += objectSize;
            return p;
        }
    };

}

#endif