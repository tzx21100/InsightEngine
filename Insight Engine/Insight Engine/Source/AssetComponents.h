#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

namespace IS {
    enum class allocationType {
        NoAllocation,
        SelfAllocated,
        StbAllocated
    };

    struct Image{
        int width;
        int height;
        int channels;
        size_t size;
        uint8_t* data;
        allocationType allocation_type;
    };
}