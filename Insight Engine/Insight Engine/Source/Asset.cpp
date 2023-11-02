/* Start Header **************************************************************/
/*!
\file	Asset.cpp
\author Matthew Ng, matthewdeen.ng@digipen.edu
\par Course: CSD2401
\date 27-09-2023
\brief
Definition of the AssetManager class for managing game assets.

All content (C) 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header ****************************************************************/

/* includes*/
#include "Pch.h"
#include "CoreEngine.h"

#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4100)
#pragma warning(disable: 26451)
#pragma warning(disable: 26819)
#pragma warning(disable: 6262)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 26451)
#pragma warning(disable: 26819)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#pragma warning(pop)

namespace IS {

    void AssetManager::Initialize() {//call once
        InsightEngine& engine = InsightEngine::Instance();
        namespace fs = std::filesystem;
        std::string path = TEXTURE_DIRECTORY; // Path to the Assets directory

        for (const auto& entry : fs::directory_iterator(path)) {
            std::string file_path = entry.path().string();
            std::string extension = entry.path().extension().string();

            // Check image extensions
            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
                LoadImage(file_path);
            }
        }

        for (const auto& entry : fs::directory_iterator(ICON_DIRECTORY)) {
            std::string file_path = entry.path().string();
            std::string extension = entry.path().extension().string();

            // Check image extensions
            if (extension == ".png") {
                IconLoad(file_path);                
            }
        }

        // Gets all prefabs and loads them to a list.
        path = PREFAB_DIRECTORY;
        for (const auto& entry : fs::directory_iterator(path)) {
            std::string file_path = entry.path().string();
            std::string extension = entry.path().extension().string();

            // Check for json extensions
            if (extension == ".json") {
                LoadPrefab(file_path);
            }
        }

        // Gets all the scenes and stores them into a list
        path = SCENE_DIRECTORY;
        for (const auto& entry : fs::directory_iterator(path)) {
            std::string file_path = entry.path().filename().string();
            mSceneList.emplace_back(file_path);
            IS_CORE_INFO("Loaded Scene: {} ", file_path);
        }

        // loads all audio and store it
        path = SOUND_DIRECTORY;
        auto audio = engine.GetSystem<ISAudio>("Audio");
        for (const auto& entry : fs::directory_iterator(path)) {
            auto const& filepath = entry.path();
            //std::string file_path = entry.path().string();
            std::string extension = entry.path().extension().string();

            // Check audio extensions (assuming mp3 and wav for this example, add more if needed)
            if (extension == ".MP3" || extension == ".WAV" || extension ==".wav" || extension==".mp3") {
                //FMOD::Channel* channel = audio->ISAudioLoadSound(file_path.c_str());
                //FMOD::Sound* sound = audio->ISAudioLoadSoundS(file_path.c_str());
                //std::string sound_name = entry.path().filename().string();
                //SaveSound(sound_name, sound);
                //SaveChannel(sound_name, channel);
                //IS_CORE_INFO("Loaded Sound: {} ", sound_name);
                LoadAudio(filepath);
            }
        }
    }

    Prefab AssetManager::GetPrefab(std::string name) { return mPrefabList[name]; }

    FMOD::Channel* AssetManager::PlaySoundByName(const std::string& sound_name, bool loop , float volume, float pitch ) {
        FMOD::Sound* sound = GetSound(sound_name);
        if (sound){
            auto audio = InsightEngine::Instance().GetSystem<ISAudio>("Audio");
            return audio->PlaySound(sound, loop, volume, pitch);
        }
        return nullptr;
    }

    FMOD::Channel* AssetManager::PlayMusicByName(const std::string& sound_name, bool loop, float volume, float pitch) {
        FMOD::Sound* sound = GetSound(sound_name);
        FMOD::Channel* channel = GetChannel(sound_name);
        auto audio = InsightEngine::Instance().GetSystem<ISAudio>("Audio");
        if (audio->IsSoundPlaying(channel)) {
            bool is_paused = false;
            channel->getPaused(&is_paused);
            if (is_paused) {
                channel->setPaused(false);
            }
            else {
                channel->setPaused(true);
            }
        }
        else {
            mChannelList[sound_name]= audio->PlaySound(sound, loop, volume, pitch);
            return mChannelList[sound_name];
        }
        return nullptr;
    }

    void AssetManager::Update([[maybe_unused]] float deltaTime) {//every frame
         
    }    

    std::string AssetManager::GetName() { return "Asset"; }

    void AssetManager::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            IS_CORE_INFO("Handling Debug");
        }
    }
    
    void AssetManager::LoadImage(std::string const& filepath)
    {
        ImageLoad(filepath);
        Image* img = GetImage(filepath);
        img->texture_index = mCurrentTexId;
        mCurrentTexId++;
        ISGraphics::textures.emplace_back(*img);
    }

    void AssetManager::LoadAudio(std::filesystem::path const& path)
    {
        std::string const& filepath = path.string();
        auto& engine = InsightEngine::Instance();
        auto audio = engine.GetSystem<ISAudio>("Audio");
        FMOD::Channel* channel = audio->ISAudioLoadSound(filepath.c_str());
        FMOD::Sound* sound = audio->ISAudioLoadSoundS(filepath.c_str());
        std::string sound_name = filepath;
        SaveSound(sound_name, sound);
        SaveChannel(sound_name, channel);
        IS_CORE_INFO("Loaded Sound: {} ", sound_name);
    }

    void AssetManager::LoadPrefab(std::string const& filepath)
    {
        auto& engine = InsightEngine::Instance();
        Prefab to_list = engine.LoadPrefabFromFile(filepath);
        mPrefabList[to_list.mName] = to_list;
        IS_CORE_INFO("Loaded Prefab: {} ", filepath);
    }

    Image* AssetManager::GetImage(const std::string& file_name)  {
        auto iter = mImageList.find(file_name);
        if (iter != mImageList.end()) {
            return &(iter->second);
        }
        throw std::runtime_error("Image not found.");
    }

    Image* AssetManager::GetIcon(const std::string& file_name) {
        auto iter = mIconList.find(file_name);
        if (iter != mIconList.end()) {
            return &(iter->second);
        }
        throw std::runtime_error("Image not found.");
    }

    void AssetManager::ImageLoad(const std::string& filepath) {
        
        Image new_image;
        std::shared_ptr<ISGraphics> graphics = InsightEngine::Instance().GetSystem<ISGraphics>("Graphics");
        graphics->initTextures(filepath,new_image);
        SaveImageData(new_image);
        IS_CORE_INFO("Using Texture: {} \"{}\"", new_image.texture_id, filepath.substr(7));
    }

    void AssetManager::IconLoad(const std::string& filepath) {

        Image new_image;
        std::shared_ptr<ISGraphics> graphics = InsightEngine::Instance().GetSystem<ISGraphics>("Graphics");
        graphics->initTextures(filepath, new_image);
        SaveIconData(new_image);
        IS_CORE_INFO("Using Icon: {} \"{}\"", new_image.texture_id, filepath.substr(7));
    }

    void AssetManager::SaveIconData(const Image icon_data){
        mIconList[icon_data.mFileName] = icon_data;
        mIconNames.emplace_back(icon_data.mFileName);
    }

    void AssetManager::SaveImageData(const Image image_data) {
        mImageList[image_data.mFileName] = image_data;
        mImageNames.emplace_back(image_data.mFileName);
    }

    void AssetManager::RemoveImageData(const std::string& file_name) {
        auto iter = mImageList.find(file_name);
        if (iter != mImageList.end()) {
            mImageList.erase(iter);
        }
        mImageNames.erase(std::remove(mImageNames.begin(), mImageNames.end(), file_name), mImageNames.end());
    }

    void AssetManager::ImageFree(const std::string& file_name) {
        RemoveImageData(file_name);
    }

    Image AssetManager::ToGray(const Image& image) {
        Image gray_image = image; // Make a copy of the original image

        //for (unsigned char* p = gray_image.data, *pg = gray_image.data; p != gray_image.data + gray_image.size; p += gray_image.channels, pg += gray_image.channels) {
        //    *pg = static_cast<uint8_t>((*p + *(p + 1) + *(p + 2)) / 3.0);
        //    if (gray_image.channels == 4) {
        //        *(pg + 1) = *(p + 3);
        //    }
        //}

        // Update the channels and allocation type of the new image
        //if (gray_image.mAllocationType != AllocationType::SelfAllocated) {
        //    gray_image.mAllocationType = AllocationType::SelfAllocated;
        //}
        auto map = (InsightEngine::Instance().GetSystemPointer().find("Graphics"));
        auto graphicsys = std::dynamic_pointer_cast<ISGraphics>(map->second);
        //auto result = graphicsys->initTextures(gray_image);
        //gray_image.texture_id = result;
        // You may want to save the modified image data to the image_manager here if needed
        SaveImageData(gray_image);

        return gray_image;
    }

    Image AssetManager::ToSepia(const Image& image) {
        Image sepia_image = image; // Make a copy of the original image

        //for (unsigned char* p = sepia_image.data; p != sepia_image.data + sepia_image.size; p += sepia_image.channels) {
        //    int r = *p, g = *(p + 1), b = *(p + 2);
        //    int new_r = static_cast<int>((0.393 * r) + (0.769 * g) + (0.189 * b));
        //    int new_g = static_cast<int>((0.349 * r) + (0.686 * g) + (0.168 * b));
        //    int new_b = static_cast<int>((0.272 * r) + (0.534 * g) + (0.131 * b));
        //    *p = static_cast<uint8_t>(std::min(255, std::max(0, new_r)));
        //    *(p + 1) = static_cast<uint8_t>(std::min(255, std::max(0, new_g)));
        //    *(p + 2) = static_cast<uint8_t>(std::min(255, std::max(0, new_b)));
        //}

        // Update the channels and allocation type of the new image
        //if (sepia_image.mAllocationType != AllocationType::SelfAllocated) {
        //    sepia_image.mAllocationType = AllocationType::SelfAllocated;
        //}
        auto map = (InsightEngine::Instance().GetSystemPointer().find("Graphics"));
        auto graphicsys = std::dynamic_pointer_cast<ISGraphics>(map->second);
        //auto result = graphicsys->initTextures(sepia_image);
        //sepia_image.texture_id = result;
        // You may want to save the modified image data to the image_manager here if needed
        SaveImageData(sepia_image);
        return sepia_image;
    }

}