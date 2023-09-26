/* Start Header **************************************************************/
/*!
\file	Asset.h
\author Matthew

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header ****************************************************************/
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H
#define GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H

#include "System.h"
#include "Audio.h"
#include "Prefab.h"
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

static inline bool StringEndsIn(const std::string& str, const std::string& ends) {
    size_t str_len = str.length();
    size_t ends_len = ends.length();
    size_t pos = str.rfind(ends);
    return (pos != std::string::npos) && (pos + ends_len == str_len);
}

#pragma warning(push)
#pragma warning(disable: 4505)
static std::string GetFileName(const std::string& file_path) {
    size_t last_slash = file_path.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        return file_path.substr(last_slash + 1);
    }
    return file_path;
}
#pragma warning(pop)

namespace IS {
    enum class AllocationType {
        NoAllocation,
        SelfAllocated,
        StbAllocated
    };

    struct Image { 
        //image data should just be in image itself
        std::string mFileName{};
        int width{};
        int height{};
        int channels{};
        size_t size{};
        unsigned long mTextureData{};
        AllocationType mAllocationType{};

    };

    class AssetManager :public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string GetName() override;
        void HandleMessage(const Message& message) override;

        //default constructor
        AssetManager() {}
        AssetManager(const char* file_name);
        ~AssetManager() { mSoundList.clear(); mImageList.clear(); }

        const Image& GetImage(const std::string& file_name) const;
        void ImageLoad(const std::string& file_path);
        void SaveImageData(const Image image_data);
        void RemoveImageData(const std::string& file_name);
        void ImageFree(const std::string& file_name);
        Image ToGray(const Image& image);
        Image ToSepia(const Image& image);


        //extra image functions
        void ImageToGray(Image& image);
        void ImageToSepia(Image& image);
        
        //functions for Prefabs
        Prefab GetPrefab(std::string name);

        //function for Sounds
        FMOD::Channel* PlaySoundByName(const std::string& sound_name, bool loop = false, float volume = 1.0f, float pitch = 1.0f);
        FMOD::Channel* PlayMusicByName(const std::string& sound_name, bool loop = false, float volume = 1.0f, float pitch = 1.0f);

        void SaveSound(const std::string& name, FMOD::Sound* sound) {
            mSoundList[name] = sound;
        }
        void SaveChannel(const std::string& name, FMOD::Channel* channel) {
            mChannelList[name] = channel;
        }

        FMOD::Sound* GetSound(const std::string& name) {
            auto iter = mSoundList.find(name);
            if (iter != mSoundList.end()) {
                return iter->second;
            }
            return nullptr;
        }

        FMOD::Channel* GetChannel(const std::string& name) {
            auto iter = mChannelList.find(name);
            if (iter != mChannelList.end()) {
                return iter->second;
            }
            return nullptr;
        }

        void RemoveSound(const std::string& name) {
            auto iter = mSoundList.find(name);
            if (iter != mSoundList.end()) {
                iter->second->release();  // Important: Release the FMOD::Sound object
                mSoundList.erase(iter);
            }
        }

        //asset managers are supposed to save sounds and fonts as well
        std::unordered_map<std::string, FMOD::Sound*> mSoundList;
        std::unordered_map<std::string, FMOD::Channel*> mChannelList;
        std::unordered_map<std::string, Image>mImageList;
        std::vector<std::string>mImageNames;
        std::unordered_map<std::string, Prefab> mPrefabList;
        std::vector<std::string>mSceneList;

    private:
        //const char* filename;
        //int width;
        //int height;
        //int channels;
        //Image image;
    };

}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H
