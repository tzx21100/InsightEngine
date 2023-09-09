#include "Pch.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb/include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libraries/stb/include/stb_image_write.h"

namespace IS {
    void ISAsset::Initialize() {//call once
        // Example usage:
       // const char* filename = "sky.jpg"; // Replace with your image file path
       // const char* filename2 = "shapes.png"; // Replace with your image file path

       // ISAsset asset(filename);
       // ISAsset asset2(filename2);
       // Image image_manager;
       // asset.ISImageLoad(image_manager);
       // asset2.ISImageLoad(image_manager);
       ///* asset.ISImageCreate(image_manager, false);
       // asset2.ISImageCreate(image_manager, false);*/
       // asset.ISImageSave(image_manager, "sky.jpg");
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
       // asset.ISImageFree(image_manager);
       // std::cout << image_manager;
       // asset2.ISImageFree(image_manager);
       // std::cout << image_manager;


    }

    void ISAsset::Update([[maybe_unused]] float deltaTime) {//every frame
               
    }

    

    std::string ISAsset::getName() {
        return "ISAsset";
    }

    void ISAsset::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            std::cout << "Handling Debug" << std::endl;
        }
    }

    // Constructor
    ISAsset::ISAsset(const char* file_name) : filename(file_name), 
        width(0), height(0), channels(0) {

    }
    // Destructor
    ISAsset::~ISAsset() {

    }

    const ImageData& ISAsset::getImageData() const{
        return image;
    }

    void ISAsset::ISImageLoad(Image & image_manager) {
        // Use STB image library to load the image
        unsigned char* img_data = stbi_load(filename, &width, &height, &channels, 0);

        if (img_data) {
            image.file_name = getFileName(filename);
            image.width = width;
            image.height = height;
            image.channels = channels;
            image.data = img_data;
            image.size = width * height * channels;
            image.allocation_type = allocationType::StbAllocated;

            // Save the image data into the provided Image object
            image_manager.saveImageData(this->getImageData());
        }
        else {
            std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
            
        }
    }

    void ISAsset::ISImageCreate(Image& image_manager, bool zeroed) {
        size_t size = width * height * channels;

        if (zeroed) {
            image.data = new uint8_t[size]();
        }
        else
        {
            image.data = new uint8_t[size];
        }

        if (image.data != nullptr) {
            image.width = width;
            image.height = height;
            image.size = size;
            image.channels = channels;
            image.allocation_type = allocationType::SelfAllocated;
        }
    }

    void ISAsset::ISImageSave(Image& image_manager, const char* file_name) {
        if (stringEndsIn(file_name, ".jpg") || stringEndsIn(file_name, ".JPG") || stringEndsIn(file_name, ".jpeg") || stringEndsIn(file_name, ".JPEG")) {
            stbi_write_jpg(file_name, image.width, image.height, image.channels, image.data, 100);
        }
        else if (stringEndsIn(file_name, ".png") || stringEndsIn(file_name, ".PNG")) {
            stbi_write_png(file_name, image.width, image.height, image.channels, image.data, image.width * image.channels);
        }
        else {
            ON_ERROR(false, "");
        }
    }

    void ISAsset::ISImageFree(Image& image_manager){
        if (image.allocation_type != allocationType::NoAllocation && image.data != nullptr) {
            if (image.allocation_type == allocationType::StbAllocated) {
                stbi_image_free(image.data);
            }
            else {
                delete[] image.data;
            }

            image.data = nullptr;
            image.width = 0;
            image.height = 0;
            image.size = 0;
            image.allocation_type = allocationType::NoAllocation;

            // Remove the image from the Image manager
            image_manager.removeImageData(image);
        }
    }

    void ISAsset::ISImageToGray(Image& image_manager) {
        int channels = image.channels == 4 ? 2 : 1;

        // Check if the current image data is already in grayscale format
        if (channels == 1) {
            return;  // Image is already grayscale, no need to convert
        }

        for (unsigned char* p = image.data, *pg = image.data; p != image.data + image.size; p += image.channels, pg += channels) {
            *pg = static_cast<uint8_t>((*p + *(p + 1) + *(p + 2)) / 3.0);
            if (image.channels == 4) {
                *(pg + 1) = *(p + 3);
            }
        }

        // Update the channels and allocation type of the image
        image.channels = channels;
        image.allocation_type = allocationType::SelfAllocated;

        // Optionally, you can save the modified image data to the image_manager
        image_manager.saveImageData(image);
    }

    void ISAsset::ISImageToSepia(Image& image_manager) {
        
        for (unsigned char* p = image.data; p != image.data + image.size; p += image.channels) {
            int r = *p, g = *(p + 1), b = *(p + 2);
            int newR = static_cast<int>((0.393 * r) + (0.769 * g) + (0.189 * b));
            int newG = static_cast<int>((0.349 * r) + (0.686 * g) + (0.168 * b));
            int newB = static_cast<int>((0.272 * r) + (0.534 * g) + (0.131 * b));
            *p = static_cast<uint8_t>(std::min(255, std::max(0, newR)));
            *(p + 1) = static_cast<uint8_t>(std::min(255, std::max(0, newG)));
            *(p + 2) = static_cast<uint8_t>(std::min(255, std::max(0, newB)));
        }

        if (image.allocation_type != allocationType::SelfAllocated) {
            image.allocation_type = allocationType::SelfAllocated;
        }

        image_manager.saveImageData(image);
    }

    Image::Image() {}

    Image::~Image() {}

    const std::vector<ImageData>& Image::getImages() const
    {
        return images;
    }
    

    void Image::saveImageData(const ImageData& image_data) {
        images.push_back(image_data);
    }

    void Image::removeImageData(const ImageData& image_data) {
        auto it = std::remove_if(images.begin(), images.end(),
            [&image_data](const ImageData& image) {
                return &image == &image_data;
            });

        if (it != images.end()) {
            images.erase(it, images.end());
        }
    }

    // Overload << for ImageData
    std::ostream& operator<<(std::ostream& os, const ImageData& image_data) {
        // Customize how you want to print an ImageData object
        os << "File Name = " << image_data.file_name << ", Width = " << image_data.width << ", Height = " << image_data.height << ", Channels = " << image_data.channels;
        return os;
    }

    // Define the overloaded << operator for Image
    std::ostream& operator<<(std::ostream& os, const Image& image) {
        const std::vector<ImageData>& imageVector = image.getImages();

        os << "[";
        for (size_t i = 0; i < imageVector.size(); ++i) {
            os << imageVector[i];
            if (i < imageVector.size() - 1) {
                os << " | ";
            }
        }
        os << "]";

        return os;
    }


}