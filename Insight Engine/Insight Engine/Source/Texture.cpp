#include "Pch.h"
#include "Texture.h"

namespace IS {
	void Texture::init_texture(std::string const& tex_file_name) {
        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        int tex_width{}, tex_height{}, tex_channels{};
        unsigned char* image_data = stbi_load(tex_file_name.c_str(), &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha);

        if (!image_data) {
            std::cerr << "Failed to load the texture image: " << tex_file_name << std::endl;
            return ;
        }

        GLuint texture_ID;
        glGenTextures(1, &texture_ID);
        glBindTexture(GL_TEXTURE_2D, texture_ID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

        stbi_image_free(image_data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        tex_ID = texture_ID;
        width = tex_width;
        height = tex_height;
        channels = tex_channels;
	}


}