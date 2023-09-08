#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H
#define GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H

#include "System.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#define ON_ERROR_EXIT(condition, msg) \
do { \
    if (condition) { \
        std::cerr << "Error in function: " << __func__ << " at line " << __LINE__ << std::endl; \
        perror(msg); \
    } \
} while (0);

static inline bool stringEndsIn(const std::string& str, const std::string& ends) {
    size_t str_len = str.length();
    size_t ends_len = ends.length();
    size_t pos = str.rfind(ends);
    return (pos != std::string::npos) && (pos + ends_len == str_len);
}

namespace IS {
    //img class here
    //macro and func here

    enum class allocationType {
        NoAllocation,
        SelfAllocated,
        StbAllocated
    };

    struct Image {
        int width;
        int height;
        int channels;
        size_t size;
        uint8_t* data;
        allocationType allocation_type;
    };

    class ISAsset :public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string getName() override;
        void HandleMessage(const Message& message) override;

        ISAsset();
        ~ISAsset();

        void ISImageLoad(Image* img, const char* file_name);
        void ISImageCreate(Image * img, int width, int height, int channels, bool zeroed);
        void ISImageSave(const Image * img, const char* file_name);
        void ISImageFree(Image * img);

        void ISImageToGray(const Image * original, Image * gray);
        void ISImageToSepia(const Image * original, Image * sepia);

    private:
        //create a container to store img data
        //make it able for ppl to get data
    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H  