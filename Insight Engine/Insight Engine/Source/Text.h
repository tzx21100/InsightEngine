/*!
 * \file Text.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This header file defines the Text class, which provides functionality for rendering text in OpenGL.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_TEXT_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_TEXT_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <vector>

namespace IS {
    /*!
     * \brief The Text class provides functionality for rendering text in OpenGL.
     *
     * The Text class encapsulates methods for rendering text using FreeType and OpenGL. It allows you to
     * display text with various fonts, sizes, and colors in an OpenGL application.
     */
    class Text {
    public:
        /*!
         * \brief Renders an animated text.
         *
         * This function renders an animated text with two strings, transitioning between them based on time.
         *
         * \param str1 The first text string.
         * \param str2 The second text string.
         * \param dt The time delta for animation.
         * \param shader The shader to use for rendering.
         */
        static void drawTextAnimation(std::string const& str1, std::string const& str2, float dt, Text& font1, Text& font2);

        /*!
         * \brief Initializes the Text class with a font file and shader.
         *
         * This function initializes the Text class, loading a font file and configuring the shader for text rendering.
         *
         * \param filepath The filepath to the font file.
         * \param text_shader The shader for text rendering.
         */
        void initText(std::string const& filepath);

        /*!
         * \brief Renders text at a specified position, size, and color.
         *
         * This function renders text at a specified position, size, and color using the provided shader.
         *
         * \param shader The shader to use for rendering.
         * \param text The text to render.
         * \param x The X-coordinate of the text's position.
         * \param y The Y-coordinate of the text's position.
         * \param scale The scale of the text.
         * \param color The color of the text.
         */
        void renderText(std::string text, float x, float y, float scale, glm::vec3 color);

        /*!
         * \brief Renders text using instanced rendering.
         *
         * This function renders text using instanced rendering for improved performance.
         *
         * \param length The length of the text to render.
         * \param shader The shader to use for rendering.
         */
        void textRenderCall(int length);

        /// Holds all state information relevant to a character as loaded using FreeType
        struct Character {
            int TextureID;          // ID handle of the glyph texture
            glm::ivec2   Size;      // Size of glyph
            glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
            unsigned int Advance;   // Horizontal offset to advance to next glyph
        };

        Shader shader;
        std::map<GLchar, Text::Character> Characters; // A map of character information loaded from FreeType.
        unsigned int text_vao;                        // VAO for text rendering.
        unsigned int text_vbo;                        // VBO for text rendering.
        GLuint textureArray;                          // Texture array for storing glyph textures.
        std::vector<glm::mat4> transforms;            // Transforms for text rendering.
        std::vector<int> letterMap;                   // Letter map for text rendering.
        const int ARRAY_LIMIT = 100;         // The maximum limit for text rendering.
        const int base_size = 256;                    // The base size for text rendering.
    };
}

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_TEXT_H