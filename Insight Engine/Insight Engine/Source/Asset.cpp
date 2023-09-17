#include "Pch.h"

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
        //Example usage:
       // const char* filename = "sky.jpg"; // Replace with your image file path
       // const char* filename2 = "shapes.png"; // Replace with your image file path

       //const char* filename = "Assets/placeholder_background.png"; // Replace with your image file path
       //AssetManager asset(filename);
       //Image image_manager;
       //asset.ISImageLoad(image_manager);
       //asset.ISImageCreate(image_manager, false);
       //asset.ISImageSave(image_manager, "Assets/placeholder_background.png");
       //asset.ISImageFree(image_manager);

       // AssetManager asset2(filename2);
       // asset2.ISImageLoad(image_manager);
       // asset2.ISImageCreate(image_manager, false);*/
       // asset2.ISImageSave(image_manager, "shapes.jpg");
       // asset.ISImageToGray(image_manager);
       // asset2.ISImageToGray(image_manager);
       // asset.ISImageSave(image_manager, "skygr.jpg");
       // asset2.ISImageSave(image_manager, "shapesgr.jpg");
       // asset.ISImageToSepia(image_manager);
       // asset2.ISImageToSepia(image_manager);
       // asset.ISImageSave(image_manager, "skyse.jpg");
       // asset2.ISImageSave(image_manager, "shapesse.jpg");
       // std::cout << image_manager;
       
       // std::cout << image_manager;
       // asset2.ISImageFree(image_manager);
       // std::cout << image_manager;


    }

    void AssetManager::Update([[maybe_unused]] float deltaTime) {//every frame
               
    }

    

    std::string AssetManager::getName() {
        return "AssetManager";
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
            auto map = (InsightEngine::Instance().GetSystemPointer().find("Graphics"));
            auto graphicsys = std::dynamic_pointer_cast<ISGraphics>(map->second);
            auto result=graphicsys->initTextures(image);
            image.texture_data = result;
            SaveImageData(image);
            std::cout << "THE tex data: " <<image.texture_data;
            return image;
        }

       

        throw std::runtime_error("Failed to load image.");
    }

    void AssetManager::SaveImageData(const Image& image_data) {
        mImageList[image_data.file_name] = image_data;
    }

    void AssetManager::RemoveImageData(const std::string& filename) {
        auto it = mImageList.find(filename);
        if (it != mImageList.end()) {
            stbi_image_free(it->second.data);
            mImageList.erase(it);
        }
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

    void AssetManager::ImageToGray(Image& image) {
        for (unsigned char* p = image.data, *pg = image.data; p != image.data + image.size; p += image.channels, pg += image.channels) {
            *pg = static_cast<uint8_t>((*p + *(p + 1) + *(p + 2)) / 3.0);
            if (image.channels == 4) {
                *(pg + 1) = *(p + 3);
            }
        }

        // Update the channels and allocation type of the image
        if (image.allocation_type != allocationType::SelfAllocated) {
            image.allocation_type = allocationType::SelfAllocated;
        }

        // save the modified image data to the image_manager
        SaveImageData(image);
    }

    void AssetManager::ImageToSepia(Image& image) {
        
        for (unsigned char* p = image.data; p != image.data + image.size; p += image.channels) {
            int r = *p, g = *(p + 1), b = *(p + 2);
            int new_r = static_cast<int>((0.393 * r) + (0.769 * g) + (0.189 * b));
            int new_g = static_cast<int>((0.349 * r) + (0.686 * g) + (0.168 * b));
            int new_b = static_cast<int>((0.272 * r) + (0.534 * g) + (0.131 * b));
            *p = static_cast<uint8_t>(std::min(255, std::max(0, new_r)));
            *(p + 1) = static_cast<uint8_t>(std::min(255, std::max(0, new_g)));
            *(p + 2) = static_cast<uint8_t>(std::min(255, std::max(0, new_b)));
        }

        if (image.allocation_type != allocationType::SelfAllocated) {
            image.allocation_type = allocationType::SelfAllocated;
        }

        SaveImageData(image);
    }


}