#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
//#include "Asset.h"

namespace IS {

	struct ISGraphics {
		static void init();
		static void update(float delta_time);
		static void draw();
		static void cleanup();

		static void initModels();

		static GLuint placeholder_tex;
		static GLuint initTextures(std::string const& imagePath);

		struct ISModel {
			GLenum primitive_type{};
			Shader shader_program{};
			GLuint vao_ID{};
			GLuint draw_count{};

			// imgui
			std::string name;
			glm::vec3 color{};
			bool drawing{ true };

			ISModel(std::string const& model_name, GLenum primitive) : name(model_name), primitive_type(primitive) {
				PRNG prng;
				for (int i{}; i < 3; ++i) {
					color[i] = prng.generate();
				}
			}

			void setupVAO();
			void setupShaders();
			void transform(float delta_time);
			void draw();

			// for M1 rubrics showcase
			void setupSpecialVAO();
			void drawSpecial();

			// transformation
			glm::vec2 world_position{}; // world coordinates (if world is 1280 pixels, valid world positions: [-640, 640]
			glm::vec2 orientation{};   // x = angle displacement, y = angle speed
			glm::vec2 scaling{};       // *2 twice the size, *1 same size, *0.5 half the size (per axis)
			glm::mat3 mdl_to_ndl_xform = glm::imat3x3{};
		};

		/*static ISModel test_box_model;
		static ISModel test_points_model;
		static ISModel test_lines_model;
		static ISModel test_circle_model;*/

		static std::vector<ISModel> models;
	};

} // end namespace IS
