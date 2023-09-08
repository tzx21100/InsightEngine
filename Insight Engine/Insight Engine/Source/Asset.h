#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H
#define GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H

#include "System.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#define ON_ERROR(condition, msg) \
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
    enum class allocationType {
        NoAllocation,
        SelfAllocated,
        StbAllocated
    };

    struct ImageData {
        int width;
        int height;
        int channels;
        std::vector<unsigned char> data;
     /*   size_t size;
        uint8_t* data;
        allocationType allocation_type;*/
    };

    struct Imageo {
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

        ISAsset(const char* file_name);
        ~ISAsset();

        bool loadImage();

        const ImageData& getImageData() const;

        void ISImageLoad(Imageo* img, const char* file_name);
        void ISImageCreate(Imageo * img, int width, int height, int channels, bool zeroed);
        void ISImageSave(const Imageo * img, const char* file_name);
        void ISImageFree(Imageo * img);

        void ISImageToGray(const Imageo * original, Imageo * gray);
        void ISImageToSepia(const Imageo * original, Imageo * sepia);

    private:
        const char* filename;
        int width;
        int height;
        int channels;
        ImageData image;
    };

    class Image {
    public:
        Image();
        ~Image();

        const std::vector<ImageData>& getImages() const;
        void saveImageData(const ImageData& imageData);


    private:
        std::vector<ImageData> images;
    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H  