/*!
 * \file Text.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
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
#include "Shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <glm/glm.hpp>
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
         * \param widthScalar * width for text's position. // 0 to 0.99~ [at 1 the first char will be placed to the right, outside of VP]
         * \param heightScalar * height for text's position. // 0 to 0.99~
         * \param scale The scale of the text.
         * \param color The color of the text.
         */
        void renderText(std::string text, float widthScalar, float heightScalar, float scale, glm::vec3 color);

        /*!
         * \brief Renders text using instanced rendering.
         *
         * This function renders text using instanced rendering for improved performance.
         *
         * \param length The length of the text to render.
         * \param shader The shader to use for rendering.
         */
        void textRenderCall(int length);

        /**
         * @brief Add a text rendering call to the list of render calls.
         *
         * This function adds a text rendering call to the list of render calls, specifying the text content,
         * width and height scaling factors, scale, and color.
         *
         * @param text The text content to be rendered.
         * @param widthScalar The width scaling factor for the text rendering.
         * @param heightScalar The height scaling factor for the text rendering.
         * @param scale The overall scale factor for the text rendering.
         * @param color The color of the rendered text, specified as a glm::vec3 (RGB).
         */
        void addTextRenderCall(std::string text, float widthScalar, float heightScalar, float scale, glm::vec3 color);
        
        /**
         * @brief Render all the queued text rendering calls.
         *
         * This function iterates through the list of text rendering calls and renders each of them.
         * After rendering, it clears the list of render calls.
         */
        static void renderAllText(std::unordered_map<std::string, Text>& textMap);

        /// Holds all state information relevant to a character as loaded using FreeType
        struct Character {
            int TextureID;          // ID handle of the glyph texture
            glm::ivec2   Size;      // Size of glyph
            glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
            unsigned int Advance;   // Horizontal offset to advance to next glyph
        };

        // Member variables
        static Shader textShader;
        std::map<GLchar, Text::Character> Characters; // A map of character information loaded from FreeType.
        unsigned int text_vao{};                        // VAO for text rendering.
        unsigned int text_vbo{};                        // VBO for text rendering.
        GLuint textureArray{};                          // Texture array for storing glyph textures.
        std::vector<glm::mat4> transforms{};            // Transforms for text rendering.
        std::vector<int> letterMap{};                   // Letter map for text rendering.
        const int ARRAY_LIMIT = 128;         // The maximum limit for text rendering.
        const int base_size = 256;                    // The base size for text rendering.

    private:
        /**
         * @brief The TextRenderCall struct represents a single rendering call for text.
         *
         * The TextRenderCall struct encapsulates information needed for rendering a text string, including the text content,
         * width and height scaling factors, scale, and color.
         */
        struct TextRenderCall {
            std::string text;
            float widthScalar;
            float heightScalar;
            float scale;
            glm::vec3 color;
        };

    public:
        /**
         * @brief Static vector to store text rendering calls.
         *
         * The static vector `renderCalls` is used to store instances of TextRenderCall, representing text rendering calls.
         * It holds information such as text content, scaling factors, scale, and color for each rendering call.
         * The vector is static to ensure that it is shared among all instances of the Text class.
         */
       std::vector<TextRenderCall> renderCalls;
    };
}

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_TEXT_H