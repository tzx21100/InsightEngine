#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H
#define GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H

#include "System.h"
#include "Audio.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <filesystem>


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

#pragma warning(push)
#pragma warning(disable: 4505)
static std::string getFileName(const std::string& file_path) {
    size_t last_slash = file_path.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        return file_path.substr(last_slash + 1);
    }
    return file_path;
}
#pragma warning(pop)

namespace IS {
    enum class allocationType {
        NoAllocation,
        SelfAllocated,
        StbAllocated
    };


    struct Image {
        
        //image data should just be in image itself
        //creating an extra image handler just makes it more tedious
        std::string file_name{};
        int width{};
        int height{};
        int channels{};
        size_t size{};
        uint8_t* data{};
        unsigned long texture_data{};
        allocationType allocation_type{};

    };

    class AssetManager :public ParentSystem {
    public:

        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string getName() override;
        void HandleMessage(const Message& message) override;

        //default constructor
        AssetManager() {}
        AssetManager(const char* file_name);
        ~AssetManager() { mSoundList.clear(); mImageList.clear(); }

        const Image& GetImage(const std::string& filename) const;
        Image ImageLoad(const std::string& filepath);
        void SaveImageData(const Image& image_data);
        void RemoveImageData(const std::string& filename);
        void ImageFree(const std::string& filename);
        Image ToGray(const Image& image);
        Image ToSepia(const Image& image);


        //extra image functions
        void ImageToGray(Image& image);
        void ImageToSepia(Image& image);


        //asset managers are supposed to save sounds and fonts as well
        void SaveSound(std::string str, FMOD::Channel* sound) { mSoundList.insert({ str,sound }); }
        std::unordered_map<std::string,FMOD::Channel*>mSoundList;
        std::unordered_map<std::string, Image>mImageList;
        std::vector<std::string>mImageNames;
        std::unordered_map<std::string, Signature> mPrefabList;


    private:
        //const char* filename;
        //int width;
        //int height;
        //int channels;
        //Image image;
    };


}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H  