#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Vector2D.h"
#include "Transform.h"

//#include "Asset.h"

namespace IS {


	class ISGraphics : public ParentSystem {
	public:
		struct ISModel {
			Transform model_TRS{};

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
		};

		//override parent
		std::string getName() override;
		void Initialize() override;
		void Update(float deltaTime) override;
		void Draw() override;
		

		
		void HandleMessage(const Message& message) override;

		static void init();
		static void update(float delta_time);
		static void draw();
		static void cleanup();

		static GLuint initTextures(std::string const& imagePath);
		static void initModels();
		static void setupScreenFBO();
		static void setupQuadVAO();
		static void setupScreenShaders();

		static GLuint placeholder_tex;
		static GLuint fbo_id;
		static GLuint tex_id;
		static GLuint vao_id;
		static Shader shader_pgm;

		static std::vector<ISModel> models;
	};

	//enums added

	enum class ModelType {
		Box,
		Point,
		Line,
		Circle

	};


} // end namespace IS



