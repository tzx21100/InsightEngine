/* Start Header **************************************************************/
/*!
\file	Asset.cpp
\author Tan Zheng Xun , t.zhengxun@digipen.edu  (80%)
        Matthew Ng, matthewdeen.ng@digipen.edu (20%)
\par Course: CSD2451
\date 27-09-2023
\brief
Definition of the AssetManager class for managing game assets.

All content (C) 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header ****************************************************************/

/* includes*/
#include "Pch.h"
#include "Engine/Core/CoreEngine.h"
#include "Engine/Systems/Window/WindowSystem.h"
#include "Graphics/Core/Graphics.h"

#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4100)
#pragma warning(disable: 26451)
#pragma warning(disable: 26819)
#pragma warning(disable: 6262)
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 26451)
#pragma warning(disable: 26819)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#pragma warning(pop)

namespace IS {



    void AssetManager::Initialize() {//call once
        InsightEngine& engine = InsightEngine::Instance();
        auto const window = engine.GetSystem<WindowSystem>("Window");
        namespace fs = std::filesystem;
        std::string path = TEXTURE_DIRECTORY; // Path to the Assets directory

        for (const auto& entry : fs::directory_iterator(path)) {
            std::string file_path = entry.path().string();
            std::string extension = entry.path().extension().string();

            // Check image extensions
            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
                mThreads.emplace_back([file_path]() {
                    ASSET_MANAGER->WorkerLoadImageData(file_path);
                    });
            }
            else if (!entry.is_directory())
            {
                window->ShowMessageBox("Image file must be either \".png\", \".jpg\" or \".jpeg\"!");
            }
        }

        for (const auto& entry : fs::directory_iterator(ICON_DIRECTORY)) {
            std::string file_path = entry.path().string();
            std::string extension = entry.path().extension().string();

            // Check image extensions
            if (extension == ".png") {
                IconLoad(file_path);
                IS_CORE_INFO("Loaded Icon: {} ", file_path);
            }
            else if (!entry.is_directory())
            {
                window->ShowMessageBox("Icon file must be \".png\"!");
            }
        }

        // Gets all prefabs and loads them to a list.
        path = PREFAB_DIRECTORY;
        for (const auto& entry : fs::directory_iterator(path)) {
            std::string file_path = entry.path().string();
            std::string extension = entry.path().extension().string();

            // Check for json extensions
            if (extension == ".json")
            {
                mThreads.emplace_back([file_path]() {
                    ASSET_MANAGER->LoadPrefab(file_path);
                    });
                IS_CORE_INFO("Loaded Prefab: {} ", file_path);
            }
            else if (!entry.is_directory())
            {
                window->ShowMessageBox("Prefab file must be \".json\"!");
            }
        }


        // Gets all the scenes and stores them into a list
        path = SCENE_DIRECTORY;
        for (const auto& entry : fs::directory_iterator(path)) {
            std::string file_path = entry.path().filename().string();
            std::string extension = entry.path().extension().string();
            if (extension == ".insight")
            {
                mSceneList.emplace_back(file_path);
                IS_CORE_INFO("Loaded Scene: {} ", file_path);
            }
            else if (!entry.is_directory())
            {
                window->ShowMessageBox("Scene file must be \".insight\"!");
            }
        }        
        
        path = PARTICLE_DIRECTORY;
        for (const auto& entry : fs::directory_iterator(path)) {
            std::string file_path = entry.path().string();
            std::string extension = entry.path().extension().string();
            if (extension == ".txt") {
                mThreads.emplace_back([file_path]() {
                    ASSET_MANAGER->LoadParticle(file_path);
                    });
                IS_CORE_INFO("Loaded Particle: {} ", file_path);
            }
            else if (!entry.is_directory())
            {
                window->ShowMessageBox("Script file must be \".cs\"!");
            }
        }

        path = SCRIPT_DIRECTORY;
        for (const auto& entry : fs::directory_iterator(path))
        {
            std::string file_path = entry.path().filename().string();
            std::string extension = entry.path().extension().string();
            if (extension == ".cs")
            {
                mScriptList.emplace_back(file_path);
                IS_CORE_INFO("Loaded Script: {} ", file_path);
            }
            else if (!entry.is_directory())
            {
                window->ShowMessageBox("Script file must be \".cs\"!");
            }
        }


        // loads all audio and store it
        path = SOUND_DIRECTORY;
        auto audio = engine.GetSystem<ISAudio>("Audio");
        for (const auto& entry : fs::directory_iterator(path)) {
            auto const& filepath = entry.path();
            std::string extension = entry.path().extension().string();

            // Check audio extensions (assuming mp3 and wav for this example, add more if needed)
            if (extension == ".MP3" || extension == ".WAV" || extension == ".wav" || extension == ".mp3")
            {
                LoadAudio(filepath);
            }
            else if (!entry.is_directory())
            {
                window->ShowMessageBox("Audio file must be either \".mp3\" or \".wav\"!");
            }
        }


        // stores shaders
        path = SHADER_DIRECTORY;
        for (const auto& entry : fs::directory_iterator(path)) {
            auto const& filepath = entry.path().filename().string();
            std::string extension = entry.path().extension().string();

            // Check shader extensions
            if (extension == ".frag" || extension == ".vert" )
            {
                mShaderList.emplace_back(filepath);
                IS_CORE_INFO("Loaded Shaders: {} ", filepath);
            }
            else if (!entry.is_directory())
            {
                window->ShowMessageBox("Shader file must be either \".vert\" or \".frag\"!");
            }
        }


        //mThreads.emplace_back(std::thread(LoadEverythingButImages));

        // Wait for all threads to finish and then join them
        for (auto& thread : mThreads) {
            if (thread.joinable()) { // Check if the thread is joinable
                thread.join(); // Join the thread if it's joinable
            }
        }

        for (auto& [name,data] : mImageData) {
            LoadImage(name, data);
        }

        mImageData.clear();
        mThreads.clear();

    }

    void AssetManager::WorkerLoadImageData(const std::string& filepath) {
        ImageData imageData = ISGraphics::loadImageData(filepath);
       
            std::lock_guard<std::mutex> lock(mImageDataMutex);
            mImageData.emplace_back(filepath, imageData);

    }

    Prefab AssetManager::GetPrefab(std::string name) { return mPrefabList[name]; }

    FMOD::Channel* AssetManager::PlaySoundByName(const std::string& sound_name, bool loop, float volume, float pitch) {
        FMOD::Sound* sound = GetSound(sound_name);
        if (!sound) {
            // Handle error: sound not found
            return nullptr;
        }

        auto audio = InsightEngine::Instance().GetSystem<ISAudio>("Audio");
        if (!audio) {
            // Handle error: audio system not found
            return nullptr;
        }



        return audio->PlaySound(sound, loop, volume * AUDIO_MANAGER->mAudioConfig.mMasterControl.mVolume, pitch);
    }


    FMOD::Channel* AssetManager::PlayMusicByName(const std::string& sound_name, bool loop, float volume, float pitch) {
        FMOD::Sound* sound = GetSound(sound_name);
        if (!sound) {
            // Handle error: sound not found
            return nullptr;
        }

        auto audio = InsightEngine::Instance().GetSystem<ISAudio>("Audio");
        if (!audio) {
            // Handle error: audio system not found
            return nullptr;
        }

        FMOD::Channel* channel = GetChannel(sound_name);
        if (audio->IsSoundPlaying(channel)) {
            bool is_paused = false;
            channel->getPaused(&is_paused);
            channel->setPaused(!is_paused);
            return channel;
        }
        else {
            FMOD::Channel* newChannel = audio->PlaySound(sound, loop, volume * AUDIO_MANAGER->mAudioConfig.mMasterControl.mVolume, pitch);
            if (newChannel) {
                mChannelList[sound_name] = newChannel;
            }
            return newChannel;
        }
    }

    void AssetManager::LoadWindowIcon(GLFWwindow* window, const char* filepath)
    {
        int width, height, channels;
        unsigned char* pixels = stbi_load(filepath, &width, &height, &channels, 0);
        if (!pixels)
        {
            IS_CORE_ERROR("Failed to load ICON image: {}", filepath);
            return;
        }

        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = pixels;

        glfwSetWindowIcon(window, 1, &image);
        stbi_image_free(image.pixels);
    }

    GLFWcursor* AssetManager::LoadWindowCursor(const char* filepath)
    {
        int width, height, channels;
        unsigned char* pixels = stbi_load(filepath, &width, &height, &channels, 0);
        if (!pixels)
        {
            IS_CORE_ERROR("Failed to load cursor image: {}", filepath);
            return nullptr;
        }

        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = pixels;

        GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
        if (!cursor)
        {
            IS_CORE_ERROR("Failed to create cursor from image: {}", filepath);
            stbi_image_free(pixels); // Free the image data
        }

        stbi_image_free(pixels); // Free the image data

        IS_CORE_INFO("Loaded Cursor: {} ", filepath);

        return cursor;
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
    
    void AssetManager::LoadImage(std::string const& filepath , ImageData image_data)
    {
        ImageLoad(filepath,image_data);
        Image* img = GetImage(std::filesystem::path(filepath).filename().string());
        img->texture_index = mCurrentTexId;
        img->mFileName = std::filesystem::path(filepath).filename().string();
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
        std::string sound_name = path.filename().string();
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

    void AssetManager::LoadShader(std::string const& filepath)
    {
        if (std::find(mShaderList.begin(), mShaderList.end(), filepath) != mShaderList.end())
            mShaderList.emplace_back(filepath);
    }

    Image* AssetManager::GetImage(const std::string& file_name)  {
        auto iter = mImageList.find(file_name);
        if (iter != mImageList.end()) {
            return &(iter->second);
        }
        return nullptr;
    }

    Image* AssetManager::GetIcon(const std::string& file_name) {
        auto iter = mIconList.find(file_name);
        if (iter != mIconList.end()) {
            return &(iter->second);
        }
        throw std::runtime_error("Image not found.");
    }

    void AssetManager::ImageLoad(const std::string& filepath, ImageData image_data) {
        Image new_image;
        std::shared_ptr<ISGraphics> graphics = InsightEngine::Instance().GetSystem<ISGraphics>("Graphics");
        graphics->initTextures(filepath,new_image,image_data);
        SaveImageData(new_image);
        IS_CORE_INFO("Using Texture: {} \"{}\"", new_image.texture_id, filepath.substr(7));
    }

    void AssetManager::IconLoad(const std::string& filepath) {

        Image new_image;
        std::shared_ptr<ISGraphics> graphics = InsightEngine::Instance().GetSystem<ISGraphics>("Graphics");
        graphics->initFastTextures(filepath, new_image);
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


    void AssetManager::RefreshDirectiories() {

        //IS_CORE_DEBUG("SIZE OF mSoundList: {}", mSoundList.size());
        //IS_CORE_DEBUG("SIZE OF mChannelList: {}", mChannelList.size());
        //IS_CORE_DEBUG("SIZE OF mImageList: {}", mImageList.size());
        //IS_CORE_DEBUG("SIZE OF mIconList: {}", mIconList.size());
        //IS_CORE_DEBUG("SIZE OF mImageNames: {}", mImageNames.size());
        //IS_CORE_DEBUG("SIZE OF mIconNames: {}", mIconNames.size());
        //IS_CORE_DEBUG("SIZE OF mPrefabList: {}", mPrefabList.size());
        //IS_CORE_DEBUG("SIZE OF mSceneList: {}", mSceneList.size());
        //IS_CORE_DEBUG("SIZE OF mScriptList: {}", mScriptList.size());
        //IS_CORE_DEBUG("SIZE OF mParticleList: {}", mParticleList.size());
        //IS_CORE_DEBUG("SIZE OF mImageData: {}", mImageData.size());
        //IS_CORE_DEBUG("SIZE OF mThreads: {}", mThreads.size());
        //IS_CORE_DEBUG("VALUE OF mCurrentTexId: {}", mCurrentTexId);


        IS_PROFILE_FUNCTION();
        std::shared_ptr<ISGraphics> graphics = InsightEngine::Instance().GetSystem<ISGraphics>("Graphics");
        for (auto& img : mIconList) {
            graphics->deleteTexture(img.second);
            IS_CORE_DEBUG("DELETED TEXTURE:{} {}", img.second.texture_id,img.first);
        }
        for (auto& img : mImageList) {
            graphics->deleteTexture(img.second);
            IS_CORE_DEBUG("DELETED TEXTURE:{} {}", img.second.texture_id, img.first);
        }
        ISGraphics::textures.clear();

        mSoundList.clear();
        mChannelList.clear();
        mImageList.clear();
        mImageNames.clear();
        mIconNames.clear();
        mIconList.clear();
        mPrefabList.clear();
        mSceneList.clear();
        mScriptList.clear();
        mParticleList.clear();
        mImageData.clear();
        mThreads.clear();
        mCurrentTexId = 0;


        Initialize();
        InsightEngine::Instance().mRuntime = false;
        SceneManager::Instance().ReloadActiveScene();


    }
}