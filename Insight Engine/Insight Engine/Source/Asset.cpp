#include "Pch.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb/include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libraries/stb/include/stb_image_write.h"

namespace IS {
    void ISAsset::Initialize() {//call once
        ////Example
        //Image img_sky, img_shapes;

        //ISAsset::ISImageLoad(&img_sky, "sky.jpg");
        //ON_ERROR_EXIT(img_sky.data == nullptr, "Error in loading the image");
        //ISAsset::ISImageLoad(&img_shapes, "shapes.png");
        //ON_ERROR_EXIT(img_shapes.data == nullptr, "Error in loading the image");

        // // Convert the images to gray
        // Image img_sky_gray, img_shapes_gray;
        // ISAsset::ISImageToGray(&img_sky, &img_sky_gray);
        // ISAsset::ISImageToGray(&img_shapes, &img_shapes_gray);
        // 
        // // Convert the images to sepia
        // Image img_sky_sepia, img_shapes_sepia;
        // ISAsset::ISImageToSepia(&img_sky, &img_sky_sepia);
        // ISAsset::ISImageToSepia(&img_shapes, &img_shapes_sepia);
        // 
        //// Save images
        //ISAsset::ISImageSave(&img_sky_gray, "sky_gray.jpg");
        //ISAsset::ISImageSave(&img_sky_sepia, "sky_sepia.jpg");
        //ISAsset::ISImageSave(&img_shapes_gray, "Shapes_gray.png");
        //ISAsset::ISImageSave(&img_shapes_sepia, "Shapes_sepia.png");
        // 
        //// Release memory
        //ISAsset::ISImageFree(&img_sky);
        //ISAsset::ISImageFree(&img_sky_gray);
        //ISAsset::ISImageFree(&img_sky_sepia); 
        //ISAsset::ISImageFree(&img_shapes);
        //ISAsset::ISImageFree(&img_shapes_gray);
        //ISAsset::ISImageFree(&img_shapes_sepia);

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
    ISAsset::ISAsset() {

    }

    // Destructor
    ISAsset::~ISAsset() {

    }

    void ISAsset::ISImageLoad(Image* img, const char* file_name){ //supports png/jpg
        if ((img->data = stbi_load(file_name, &img->width, &img->height, &img->channels, 0)) != nullptr) {
            img->size = img->width * img->height * img->channels;
            img->allocation_type = allocationType::StbAllocated;
        }
    }

    void ISAsset::ISImageCreate(Image* img, int width, int height, int channels, bool zeroed){
        size_t size = width * height * channels;

        if (zeroed) {
            img->data = new uint8_t[size]();
        }
        else
        {
            img->data = new uint8_t[size];
        }

        if (img->data != nullptr) {
            img->width = width;
            img->height = height;
            img->size = size;
            img->channels = channels;
            img->allocation_type = allocationType::SelfAllocated;
        }
    }

    void ISAsset::ISImageSave(const Image* img, const char* file_name){
        if (stringEndsIn(file_name, ".jpg") || stringEndsIn(file_name, ".JPG") || stringEndsIn(file_name, ".jpeg") || stringEndsIn(file_name, ".JPEG")) {
            stbi_write_jpg(file_name, img->width, img->height, img->channels, img->data, 100);
        }
        else if (stringEndsIn(file_name, ".png") || stringEndsIn(file_name, ".PNG")) {
            stbi_write_png(file_name, img->width, img->height, img->channels, img->data, img->width * img->channels);
        }
        else {
            ON_ERROR_EXIT(false, "");
        }

    }

    void ISAsset::ISImageFree(Image* img){
        if (img->allocation_type != allocationType::NoAllocation && img->data != nullptr) {
            if (img->allocation_type == allocationType::StbAllocated) {
                stbi_image_free(img->data);
            }
            else {
                delete[] img->data;
            }

            img->data = nullptr;
            img->width = 0;
            img->height = 0;
            img->size = 0;
            img->allocation_type = allocationType::NoAllocation;
        }
    }

    void ISAsset::ISImageToGray(const Image* original, Image* gray) {
        int channels = original->channels == 4 ? 2 : 1;
        ISImageCreate(gray, original->width, original->height, channels, false);
        ON_ERROR_EXIT(gray->data == nullptr, "Error creating gray img");

        for (unsigned char *p = original->data, *pg = gray->data; p != original->data + original->size; p += original->channels, pg += gray->channels) {
            *pg = static_cast<uint8_t>((*p + *(p + 1) + *(p + 2)) / 3.0);
            if (original->channels == 4) {
                *(pg + 1) = *(p + 3);
            }
        }
    }

    void ISAsset::ISImageToSepia(const Image* original, Image* sepia) {
        ISImageCreate(sepia, original->width, original->height, original->channels, false);
        ON_ERROR_EXIT(sepia->data == nullptr, "Error creating sepia img");

        for (unsigned char *p = original->data, *pg = sepia->data; p != original->data + original->size; p += original->channels, pg += sepia->channels) {
            *pg = static_cast<uint8_t>(std::min(0.393 * *p + 0.769 * *(p + 1) + 0.189 * *(p + 2), 255.0)); // red
            *(pg + 1) = static_cast<uint8_t>(std::min(0.349 * *p + 0.686 * *(p + 1) + 0.168 * *(p + 2), 255.0)); // green
            *(pg + 2) = static_cast<uint8_t>(std::min(0.272 * *p + 0.534 * *(p + 1) + 0.131 * *(p + 2), 255.0)); // blue
            if (original->channels == 4) {
                *(pg + 3) = *(p + 3);
            }
        }
    }
}