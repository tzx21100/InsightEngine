#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>

namespace IS {
	class Text {
    public:
        void init_text(const char* font_file_path, int font_pixel_size);
        void free_text();
        static GLuint create_font_texture(FT_GlyphSlot glyph);
        GLuint render_text(std::string const& text);

        FT_Library ft;
        FT_Face face;
	};

}