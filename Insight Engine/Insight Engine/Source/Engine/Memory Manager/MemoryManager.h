#include <vector>


/* This is a memory manager made specifically with our
   ECS in mind. We are going to ensure that the components
   allocated are stored here and freed when added and deleted.

    Also since components are frequently getting added and destroyed,
    having a memory manager to allocate the memory will be much faster.
*/
class ComponentMemoryManager {
public:
    // we use 100 as the default size for now
    ComponentMemoryManager(size_t data_size = 100) {
        mCapacity = data_size;
        mSize = 0;
    }

    ~ComponentMemoryManager() {

    }

    template <typename T>
    T* AddComponent() {
        T* component = new T;
        mAllocatedComponents.push_back(component);
        return component;
    }
    
    template <typename T>
    void DeallocateComponent() {

    }


private:
    std::vector<void*> mAllocatedComponents;
    size_t mCapacity;
    size_t mSize;
};