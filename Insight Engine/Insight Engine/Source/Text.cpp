#include "Pch.h"
#include <iostream>

namespace IS {
    void Text::init_text(const char* font_file_path, int font_pixel_size) {
        if (FT_Init_FreeType(&ft)) {
            std::cout << "Error: Could not initialize FreeType." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (FT_New_Face(ft, font_file_path, 0, &face)) {
            std::cout << "Error: Could not load font." << std::endl;
            exit(EXIT_FAILURE);
        }

        FT_Set_Pixel_Sizes(face, 0, font_pixel_size);
    }

    void Text::free_text() {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    GLuint Text::create_font_texture(FT_GlyphSlot glyph) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        return textureID;
    }

    GLuint Text::render_text(std::string const& text) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width = 0; // Total width of the rendered text
        int maxHeight = 0; // Maximum height of the rendered text

        // Iterate through each character in the string
        for (char c : text) {
            // Load the glyph for the character
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "Error: Failed to load glyph for character: " << c << std::endl;
                continue;
            }

            // Calculate the dimensions for this character
            int charWidth = face->glyph->bitmap.width;
            int charHeight = face->glyph->bitmap.rows;

            // Update the total width and maximum height
            width += charWidth;
            maxHeight = std::max(maxHeight, charHeight);

            // Copy the glyph's bitmap data to the texture at the current position
            glTexSubImage2D(GL_TEXTURE_2D, 0, width - charWidth, 0, charWidth, charHeight, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        }

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);

        return textureID;
    }
}