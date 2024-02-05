/* Start Header **************************************************************/
/*!
 * \file Asset.h
 * \author Matthew Ng, matthewdeen.ng@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * Declaration of the AssetManager class for managing game assets.
 *
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 */
 /* End Header ****************************************************************/

/*include guards*/
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H
#define GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H

/*includes */
#include "Engine/ECS/System.h"
#include "Engine/Systems/Audio/Audio.h"
#include "Engine/JSON/Prefab.h"
#include "Engine/Systems/Particle/Particle.h"
#include "Scene/SceneManager.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#define ON_ERROR(condition, msg) \
//do { \
//    if (condition) { \
//        std::cerr << "Error in function: " << __func__ << " at line " << __LINE__ << std::endl; \
//        perror(msg); \
//    } \
//} while (0);
//
//static inline bool StringEndsIn(const std::string& str, const std::string& ends) {
//    size_t str_len = str.length();
//    size_t ends_len = ends.length();
//    size_t pos = str.rfind(ends);
//    return (pos != std::string::npos) && (pos + ends_len == str_len);
//}

#pragma warning(push)
#pragma warning(disable: 4505)
//static std::string GetFileName(const std::string& file_path) {
//    size_t last_slash = file_path.find_last_of("/\\");
//    if (last_slash != std::string::npos) {
//        return file_path.substr(last_slash + 1);
//    }
//    return file_path;
//}
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
        GLuint texture_id{}; // actual ID given my openGL
        unsigned long mTextureData{};
        int texture_index{}; // Sequential, first texture IN USE has index 0, second has 1 ...
        //AllocationType mAllocationType{};

        /*!
         * \brief == operator overload.
         * \param other The image to compare with.
         * \return Boolean flag indicating the outcome.
         */
        bool operator==(Image const& other)
        {
            return other.mFileName == mFileName;
        }

        /*!
         * \brief != operator overload.
         * \param other The image to compare with.
         * \return Boolean flag indicating the outcome.
         */
        bool operator!=(Image const& other)
        {
            return other.mFileName != mFileName;
        }
    };

    //converstion to C#
    struct SimpleImage {
        const char* mFileName; // pointer to null-terminated string
        int width;
        int height;
        int channels;
        size_t size;
        GLuint texture_id;
        unsigned long mTextureData;
        int texture_index;
    };

    // Convert Image to SimpleImage
    inline SimpleImage ConvertToSimpleImage(Image* image) {
        SimpleImage simg;
        simg.mFileName = image->mFileName.c_str();
        simg.width = image->width;
        simg.height = image->height;
        simg.channels = image->channels;
        simg.size = image->size;
        simg.texture_id = image->texture_id;
        simg.mTextureData = image->mTextureData;
        simg.texture_index = image->texture_index;
        return simg;
    }
    // Convert from SimpleImage
    inline Image ConvertToImage(const SimpleImage &simg) {
        Image img;
        img.mFileName = std::string(simg.mFileName); // Convert char* to std::string

        img.width = simg.width;
        img.height = simg.height;
        img.channels = simg.channels;
        img.size = simg.size;
        img.texture_id = simg.texture_id;
        img.mTextureData = simg.mTextureData;
        img.texture_index = simg.texture_index;
        return img;
    }

    /*!
     * \brief The AssetManager class manages game assets.
     *
     * The AssetManager class provides functions to load, unload, and access various game assets such as textures, models, and audio files.
     * It ensures that assets are efficiently loaded and unloaded as needed.
     */
    class AssetManager :public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string GetName() override;
        void HandleMessage(const Message& message) override;

        /*!
         * \brief Default constructor for the AssetManager class.
         */
        AssetManager() {}

        /*!
         * \brief Constructor for the AssetManager class.
         *
         * \param file_name The name of the asset file to be used.
         */
        AssetManager(const char* file_name);

        /*!
         * \brief Destructor for the AssetManager class.
         *
         * Unloads all assets and releases resources.
         */
        ~AssetManager() { 
            // Release memory for images
            for (auto& imagePair : mImageList) {
                if (imagePair.second.texture_id) {
                    glDeleteTextures(1, &imagePair.second.texture_id); // release OpenGL texture
                }
            }
            mImageList.clear();
            mImageNames.clear();

            // Release FMOD Sound objects
            mSoundList.clear();
            mChannelList.clear();

            // Clear other lists
            mPrefabList.clear();
            mSceneList.clear();
        }

        void LoadImage(std::string const& filepath);
        void LoadAudio(std::filesystem::path const& filepath);
        void LoadPrefab(std::string const& filepath);

        /*!
         * \brief Retrieves image data by file name.
         *
         * \param file_name The name of the image file.
         * \return A reference to the Image structure containing image data.
         */
        Image* GetImage(const std::string& file_name);
        Image* GetIcon(const std::string& file_name);

        /*!
         * \brief Loads image data from a file.
         *
         * \param file_path The file path to the image.
         */
        void ImageLoad(const std::string& file_path);

        void IconLoad(const std::string& file_path);
        /*!
         * \brief Saves image data.
         *
         * \param image_data The Image structure containing image data to be saved.
         */
        void SaveImageData(const Image image_data);

        void SaveIconData(const Image icon_data);

        /*!
         * \brief Removes image data by file name.
         *
         * \param file_name The name of the image file to remove.
         */
        void RemoveImageData(const std::string& file_name);

        /*!
         * \brief Frees memory associated with an image.
         *
         * \param file_name The name of the image file to free.
         */
        void ImageFree(const std::string& file_name);


        /*!
         * \brief Converts an image to grayscale.
         *
         * \param image The Image structure to convert to grayscale.
         */
        Image ToGray(const Image& image);

        /*!
         * \brief Converts an image to sepia.
         *
         * \param image The Image structure to convert to sepia.
         */
        Image ToSepia(const Image& image);

        /*!
         * \brief Retrieves a Prefab object by name.
         *
         * \param name The name of the Prefab to retrieve.
         * \return The Prefab object.
         */
        Prefab GetPrefab(std::string name);

        /*!
         * \brief Plays a sound by name.
         *
         * \param sound_name The name of the sound to play.
         * \param loop Specifies whether the sound should loop (default is false).
         * \param volume The volume at which to play the sound (default is 1.0f).
         * \param pitch The pitch at which to play the sound (default is 1.0f).
         * \return A pointer to the FMOD::Channel playing the sound.
         */
        FMOD::Channel* PlaySoundByName(const std::string& sound_name, bool loop = false, float volume = 1.0f, float pitch = 1.0f);

        /*!
         * \brief Plays music by name.
         *
         * \param sound_name The name of the music to play.
         * \param loop Specifies whether the music should loop (default is false).
         * \param volume The volume at which to play the music (default is 1.0f).
         * \param pitch The pitch at which to play the music (default is 1.0f).
         * \return A pointer to the FMOD::Channel playing the music.
         */
        FMOD::Channel* PlayMusicByName(const std::string& sound_name, bool loop = true, float volume = 1.0f, float pitch = 1.0f);

        /*!
         * \brief Saves a sound by name.
         *
         * \param name The name to save the sound as.
         * \param sound A pointer to the FMOD::Sound to save.
         */
        void SaveSound(const std::string& name, FMOD::Sound* sound) {
            mSoundList[name] = sound;
        }

        /*!
         * \brief Saves a channel by name.
         *
         * \param name The name to save the channel as.
         * \param channel A pointer to the FMOD::Channel to save.
         */
        void SaveChannel(const std::string& name, FMOD::Channel* channel) {
            mChannelList[name] = channel;
        }

        /*!
         * \brief Retrieves a sound by name.
         *
         * \param name The name of the sound to retrieve.
         * \return A pointer to the FMOD::Sound with the specified name.
         */
        FMOD::Sound* GetSound(const std::string& name) {
            auto iter = mSoundList.find(name);
            if (iter != mSoundList.end()) {
                return iter->second;
            }
            return nullptr;
        }

        /*!
         * \brief Retrieves a channel by name.
         *
         * \param name The name of the channel to retrieve.
         * \return A pointer to the FMOD::Channel with the specified name.
         */
        FMOD::Channel* GetChannel(const std::string& name) {
            auto iter = mChannelList.find(name);
            if (iter != mChannelList.end()) {
                return iter->second;
            }
            return nullptr;
        }

        /*!
         * \brief Removes a sound by name.
         *
         * \param name The name of the sound to remove.
         */
        void RemoveSound(const std::string& name) {
            auto iter = mSoundList.find(name);
            if (iter != mSoundList.end()) {
                iter->second->release();  // Important: Release the FMOD::Sound object
                mSoundList.erase(iter);
            }
        }


        void ClearAllSounds() {
            for (auto sound : mChannelList) {
                sound.second->stop();
            }
        }


        //saving and loading particles

        void SaveParticleToFile(const Particle& particle, const std::string& filename) {
            std::ofstream file(filename, std::ios::out | std::ios::trunc); // Open file in write mode, truncating existing content

            if (file.is_open()) {
                file << particle.Serialize();
                file.close();
            }
            else {
                // Handle the error, e.g., by throwing an exception or logging an error message
                std::cerr << "Error: Unable to open file for writing." << std::endl;
            }
        }

        Particle LoadParticleFromFile(const std::string& filename) {
            std::ifstream file(filename);
            std::stringstream data;
            if (file.is_open()) {
                data << file.rdbuf();
                file.close();
            }
            return Particle::Deserialize(data.str());
        }

        void SaveParticle(Particle part, std::string filename) {
            filename = PARTICLE_DIRECTORY + filename;
            SaveParticleToFile(part, filename);
        
        }
        void LoadParticle(std::string filename) {
           
            IS_CORE_DEBUG("{}", filename);
            mParticleList[filename]=LoadParticleFromFile(filename);

        }

        Particle GetParticle(std::string filename) {
            filename = PARTICLE_DIRECTORY + filename;
            return mParticleList[filename];
        }

        void RefreshDirectiories();

        static void LoadWindowIcon(GLFWwindow* window, const char* filepath);
        static GLFWcursor* LoadWindowCursor(const char* filepath);

        // for save sounds and fonts
        std::unordered_map<std::string, FMOD::Sound*> mSoundList;
        std::unordered_map<std::string, FMOD::Channel*> mChannelList;
        std::unordered_map<std::string, Image>mImageList;
        std::unordered_map <std::string, Image> mIconList;
        std::vector<std::string>mImageNames;
        std::vector<std::string>mIconNames;
        std::unordered_map<std::string, Prefab> mPrefabList;
        std::vector<std::string>mSceneList;
        std::vector<std::string>mScriptList;

        
        std::unordered_map<std::string,Particle>mParticleList;


        static constexpr const char* TEXTURE_DIRECTORY  = "Assets/Textures/";
        static constexpr const char* ICON_DIRECTORY     = "Assets/Icons/";
        static constexpr const char* PREFAB_DIRECTORY   = "Assets/Prefabs/";
        static constexpr const char* SCENE_DIRECTORY    = "Assets/Scenes/";
        static constexpr const char* SOUND_DIRECTORY    = "Assets/Sounds/";
        static constexpr const char* PARTICLE_DIRECTORY = "Assets/Particles/";
        static constexpr const char* SCRIPT_DIRECTORY    = "Assets/Scripts/";

    private:
        int mCurrentTexId{};
        //const char* filename;
        //int width;
        //int height;
        //int channels;
        //Image image;
    };

}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H
