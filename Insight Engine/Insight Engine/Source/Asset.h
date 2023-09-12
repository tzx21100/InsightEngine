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

static std::string getFileName(const std::string& file_path) {
    size_t last_slash = file_path.find_last_of("/\\");
    if ((last_slash != std::string::npos)) {
        return file_path.substr(last_slash + 1);
    }
    return file_path;
}

namespace IS {
    enum class allocationType {
        NoAllocation,
        SelfAllocated,
        StbAllocated
    };

    struct ImageData {
        std::string file_name;
        int width;
        int height;
        int channels;
        size_t size;
        uint8_t* data;
        allocationType allocation_type;
  
    };

    class Image {
    public:
        Image();
        ~Image();

        const std::vector<ImageData>& getImages() const;
        void saveImageData(const ImageData& image_data);
        void removeImageData(const ImageData& image_data);
        uint8_t* getImageData();
        friend std::ostream& operator<<(std::ostream& os, const Image& image);


    private:
        std::vector<ImageData> images;
    };

    class ISAsset :public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string getName() override;
        void HandleMessage(const Message& message) override;

        //default constructor
        ISAsset() = default;
        ISAsset(const char* file_name);
        ~ISAsset();
        
        const ImageData& getImageData() const;

        uint8_t* ISImageLoad(Image& image_manager);
        void ISImageCreate(Image& image_manager, bool zeroed);
        void ISImageSave(Image& image_manager, const char* file_name);
        void ISImageFree(Image& image_manager);
        void ISImageToGray(Image& image_manager);
        void ISImageToSepia(Image& image_manager);



    private:
        const char* filename;
        int width;
        int height;
        int channels;
        ImageData image;
    };


}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H  