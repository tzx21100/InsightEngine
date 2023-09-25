#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <vector>


namespace IS {
	class Text {
    public:
        static void drawTextAnimation(std::string const& str1, std::string const& str2, float dt, Shader shader);

        static const unsigned int ARRAY_LIMIT = 100;
        static const int base_size = 256;

        static void initText(std::string const& filepath, Shader text_shader);
        static void renderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
        static void textRenderCall(int length, GLuint shader);

        /// Holds all state information relevant to a character as loaded using FreeType
        struct Character {
            int TextureID; // ID handle of the glyph texture
            glm::ivec2   Size;      // Size of glyph
            glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
            unsigned int Advance;   // Horizontal offset to advance to next glyph
        };

        static std::map<GLchar, Text::Character> Characters;
        static unsigned int text_vao, text_vbo;
        static GLuint textureArray;
        static std::vector<glm::mat4> transforms;
        static std::vector<int> letterMap;
	};

}