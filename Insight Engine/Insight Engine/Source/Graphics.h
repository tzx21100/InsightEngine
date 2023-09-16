#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Transform.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Framebuffer.h"

//#include "Asset.h"

namespace IS {
	class ISGraphics : public ParentSystem {
	public:

		//struct Mesh {
		//	GLuint vao_ID{};
		//	GLuint draw_count{};

		//	void setupVAO();
		//	void setupSpecialVAO();
		//};

		//struct ISModel {
		//	Transform model_TRS{};

		//	
		//	Shader shader_program{};
		//	GLuint vao_ID{};
		//	GLuint draw_count{};

		//	// imgui
		//	std::string name;
		//	glm::vec3 color{};
		//	bool drawing{ true };

		//	ISModel(std::string const& model_name, GLenum primitive) : name(model_name), primitive_type(primitive) {
		//		PRNG prng;
		//		for (int i{}; i < 3; ++i) {
		//			color[i] = prng.generate();
		//		}
		//	}

		//	void setupVAO();
		//	void setupShaders();
		//	void transform(float delta_time);
		//	void draw();

		//	// for M1 rubrics showcase
		//	void setupSpecialVAO();
		//	void drawSpecial();
		//};

		//override parent
		void Initialize() override;
		void Update(float deltaTime) override;
		std::string getName() override;
		void HandleMessage(const Message& msg) override;
		void Draw(float delta) override;
		
		static void cleanup();

		static GLuint initTextures(std::string const& imagePath);
		static void initSprites();
		static void setupShaders();
		static GLuint getScreenTexture();

		static GLuint placeholder_tex;
		static std::shared_ptr<Framebuffer> framebuffer;
		static Shader mesh_shader_pgm;
		static Mesh quad_mesh;

		static std::vector<Sprite> sprites;
	};

	//enums added

	/*enum class ModelType {
		Box,
		Point,
		Line,
		Circle
	};*/


} // end namespace IS



