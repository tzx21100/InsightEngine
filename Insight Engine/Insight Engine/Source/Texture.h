#pragma once

#include <GL/glew.h>
#include <string>
#include <stb_image.h>

namespace IS {
	class Texture {
	public:
		int width{};
		int height{};
		int channels{};

		GLuint tex_ID{};

		void init_texture(std::string const& tex_file_name);
	};
}