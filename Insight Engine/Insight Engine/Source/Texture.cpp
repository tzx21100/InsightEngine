#include "Pch.h"
//#include "Texture.h"
//
//namespace IS {
//    GLuint Texture::initTextures(Image& image) {
//
//        // Enable blending for transparency
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//
//        int width{ image.width }, height{ image.height }; // channels{ image.channels };
//        unsigned char* image_data = image.data;
//
//        if (!image_data) {
//            IS_CORE_ERROR("Failed to load image: {}", image.file_name.empty() ? "No filepath provided!" : image.file_name);
//            return 0; // Return 0 to indicate failure
//        }
//
//
//        GLuint textureID;
//        glGenTextures(1, &textureID);
//        glBindTexture(GL_TEXTURE_2D, textureID);
//
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
//
//        stbi_image_free(image_data);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        glBindTexture(GL_TEXTURE_2D, 0);
//
//        IS_CORE_INFO("Using Texture: {}", textureID);
//        return textureID;
//    }
//}