#include "Pch.h"
#include "CoreEngine.h"

#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4100)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#pragma warning(pop)

namespace IS {
    void AssetManager::Initialize() {//call once
        InsightEngine& engine = InsightEngine::Instance();
        namespace fs = std::filesystem;
        std::string path = "Assets/"; // Path to the Assets directory

        for (const auto& entry : fs::directory_iterator(path)) {
            std::string filePath = entry.path().string();
            std::string extension = entry.path().extension().string();

            // Check image extensions
            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
                Image img = ImageLoad(filePath);
            }
        }

        path = "Assets/Prefabs";
        for (const auto& entry : fs::directory_iterator(path)) {
            std::string filePath = entry.path().string();
            std::string extension = entry.path().extension().string();

            // Check for json extensions
            if (extension == ".json") {
               Prefab to_list=engine.LoadPrefabFromFile(filePath);
               mPrefabList[to_list.mName] = to_list;
            }
        }


    }

    Prefab AssetManager::GetPrefab(std::string name) {
        return mPrefabList[name];
    }


    void AssetManager::Update([[maybe_unused]] float deltaTime) {//every frame
               
    }

    

    std::string AssetManager::GetName() {
        return "Asset";
    }

    void AssetManager::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            std::cout << "Handling Debug" << std::endl;
        }
    }


    const Image& AssetManager::GetImage(const std::string& filename) const {
        auto it = mImageList.find(filename);
        if (it != mImageList.end()) {
            return it->second;
        }
        throw std::runtime_error("Image not found.");
    }

    Image AssetManager::ImageLoad(const std::string& filepath) {
        int width, height, channels;
        uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        if (data) {

            Image image;
            image.file_name = filepath;
            image.width = width;
            image.height = height;
            image.channels = channels;
            image.data = data;
            image.allocation_type = allocationType::StbAllocated;
            std::shared_ptr<ISGraphics> graphics = InsightEngine::Instance().GetSystem<ISGraphics>("Graphics");
            GLuint result = graphics->initTextures(image);
            std::cout << result;
            image.texture_data = result;
            image.data = nullptr;
            SaveImageData(image);
            return image;
        }

       

        throw std::runtime_error("Failed to load image.");
    }

    void AssetManager::SaveImageData(const Image image_data) {
        mImageList[image_data.file_name] = image_data;
        mImageNames.emplace_back(image_data.file_name);
    }

    void AssetManager::RemoveImageData(const std::string& filename) {
        auto it = mImageList.find(filename);
        if (it != mImageList.end()) {
            stbi_image_free(it->second.data);
            mImageList.erase(it);
        }
        mImageNames.erase(std::remove(mImageNames.begin(), mImageNames.end(), filename), mImageNames.end());
    }

    void AssetManager::ImageFree(const std::string& filename) {
        RemoveImageData(filename);
    }

    //i have no idea what this does
    //void AssetManager::ImageCreate([[maybe_unused]] Image& image, bool zeroed) {
    //    size_t size = width * height * channels;

    //    if (zeroed) {
    //        image.data = new uint8_t[size]();
    //    }
    //    else
    //    {
    //        image.data = new uint8_t[size];
    //    }

    //    if (image.data != nullptr) {
    //        image.width = width;
    //        image.height = height;
    //        image.size = size;
    //        image.channels = channels;
    //        image.allocation_type = allocationType::SelfAllocated;
    //    }
    //}

    Image AssetManager::ToGray(const Image& image) {
        Image grayImage = image; // Make a copy of the original image

        for (unsigned char* p = grayImage.data, *pg = grayImage.data; p != grayImage.data + grayImage.size; p += grayImage.channels, pg += grayImage.channels) {
            *pg = static_cast<uint8_t>((*p + *(p + 1) + *(p + 2)) / 3.0);
            if (grayImage.channels == 4) {
                *(pg + 1) = *(p + 3);
            }
        }

        // Update the channels and allocation type of the new image
        if (grayImage.allocation_type != allocationType::SelfAllocated) {
            grayImage.allocation_type = allocationType::SelfAllocated;
        }
        auto map = (InsightEngine::Instance().GetSystemPointer().find("Graphics"));
        auto graphicsys = std::dynamic_pointer_cast<ISGraphics>(map->second);
        auto result = graphicsys->initTextures(grayImage);
        grayImage.texture_data = result;
        // You may want to save the modified image data to the image_manager here if needed
        SaveImageData(grayImage);

        return grayImage;
    }

    Image AssetManager::ToSepia(const Image& image) {
        Image sepiaImage = image; // Make a copy of the original image

        for (unsigned char* p = sepiaImage.data; p != sepiaImage.data + sepiaImage.size; p += sepiaImage.channels) {
            int r = *p, g = *(p + 1), b = *(p + 2);
            int new_r = static_cast<int>((0.393 * r) + (0.769 * g) + (0.189 * b));
            int new_g = static_cast<int>((0.349 * r) + (0.686 * g) + (0.168 * b));
            int new_b = static_cast<int>((0.272 * r) + (0.534 * g) + (0.131 * b));
            *p = static_cast<uint8_t>(std::min(255, std::max(0, new_r)));
            *(p + 1) = static_cast<uint8_t>(std::min(255, std::max(0, new_g)));
            *(p + 2) = static_cast<uint8_t>(std::min(255, std::max(0, new_b)));
        }

        // Update the channels and allocation type of the new image
        if (sepiaImage.allocation_type != allocationType::SelfAllocated) {
            sepiaImage.allocation_type = allocationType::SelfAllocated;
        }
        auto map = (InsightEngine::Instance().GetSystemPointer().find("Graphics"));
        auto graphicsys = std::dynamic_pointer_cast<ISGraphics>(map->second);
        auto result = graphicsys->initTextures(sepiaImage);
        sepiaImage.texture_data = result;
        // You may want to save the modified image data to the image_manager here if needed
        SaveImageData(sepiaImage);
        return sepiaImage;
    }



}