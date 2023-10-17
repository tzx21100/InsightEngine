/*!
 * \file Text.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This source file defines the Text class, which provides functionality for rendering text in OpenGL.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#include "Pch.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used: nameless struct/union
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

namespace IS {
    std::map<GLchar, Text::Character> Text::Characters;
    unsigned int Text::text_vao, Text::text_vbo;
    GLuint Text::textureArray;
    std::vector<glm::mat4> Text::transforms;
    std::vector<int> Text::letterMap;

    void Text::drawTextAnimation(std::string const& str1, std::string const& str2, float dt, Shader shader) {
        // set static timer and condition
        static float text_timer = 0.f;
        static bool drawFirst = true;

        text_timer += dt;
        if (text_timer > 3.f) {
            text_timer = 0.f;
            drawFirst = !drawFirst;
        }

        // draw either text strings
        if (drawFirst) renderText(shader, str1, -125.f, 400.f, 10.f, glm::vec3(0.529f, 0.808f, 0.922f));
        else renderText(shader, str2, -135.f, 400.f, 10.f, glm::vec3(0.255f, 0.412f, 0.882f));
    }

    void Text::initText(std::string const& filepath, Shader text_shader) {
        // OpenGL state
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // compile and setup the shader
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT));
        text_shader.use();
        glUniformMatrix4fv(glGetUniformLocation(text_shader.getHandle(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // FreeType
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return;
        }

        // load font as face
        FT_Face face;
        if (FT_New_Face(ft, filepath.c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return;
        }
        else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, base_size, base_size);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glGenTextures(1, &textureArray);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, base_size, base_size, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                //if (c == 36) continue; // unable to load '$' for some reason
                // Load character glyph 
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph for character " << static_cast<int>(c) << std::endl;
                    continue;
                }

                glTexSubImage3D(
                    GL_TEXTURE_2D_ARRAY,
                    0, 0, 0, int(c),
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows, 1,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );

                // now store character for later use
                Character character = {
                    int(c),
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Character>(c, character));
            }
            // set texture options
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
            
        }
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        for (int i = 0; i < ARRAY_LIMIT; i++) {
            letterMap.push_back(0);
            transforms.push_back(glm::mat4(1.0f));
        }

        GLfloat vertex_data[] = { // vertices for VAO
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
        };

        // configure VAO/VBO for text texture quads
        glGenVertexArrays(1, &text_vao);
        glGenBuffers(1, &text_vbo);
        glBindVertexArray(text_vao);
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // render line of text
    void Text::renderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color) {
        // Scale the text to the correct size.
        scale = scale * 48.f / (base_size * 16);

        // Store the initial X position.
        float copyX = x;

        // Activate the shader and set the text color.
        shader.use();
        glUniform3f(glGetUniformLocation(shader.getHandle(), "textColor"), color.x, color.y, color.z);

        // Bind textures and buffers.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
        glBindVertexArray(text_vao);
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo);

        int char_index = 0;

        // Iterate through all characters in the text.
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) { // for each char in string
            Character ch = Characters[*c];

            if (*c == '\n') { // if new line
                y -= static_cast<float>(((ch.Size.y)) * 1.3 * scale); // Indent y and reset X.
                x = copyX;
            }
            else if (*c == ' ') { // if whitespace
                x += (ch.Advance >> 6) * scale; // add empty space in texture
            }
            else {
                // Calculate the position and transformation for the character
                float xpos = x + ch.Bearing.x * scale + (WIDTH / 2.f);
                float ypos = y - (base_size - ch.Bearing.y) * scale + (HEIGHT / 2.f);

                // Store the transformation matrix and character texture ID
                transforms[char_index] = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(base_size * scale, base_size * scale, 0));
                letterMap[char_index] = ch.TextureID;
                
                // Render the character as a quad
                x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
                char_index++;

                // Perform a render call if the character limit is reached
                if (char_index == ARRAY_LIMIT - 1) {
                    textRenderCall(char_index, shader.getHandle());
                    char_index = 0;
                }
            }
        }
        // Perform the final render call for any remaining characters
        textRenderCall(char_index, shader.getHandle());

        // Unbind buffers and textures
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    void Text::textRenderCall(int length, GLuint shader) {
        if (length != 0) {
            // Set the transformation matrices and character texture IDs
            glUniformMatrix4fv(glGetUniformLocation(shader, "transforms"), length, GL_FALSE, &transforms[0][0][0]);
            glUniform1iv(glGetUniformLocation(shader, "letterMap"), length, &letterMap[0]);

            // Render the characters using instanced rendering
            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, length);
        }
    }
}