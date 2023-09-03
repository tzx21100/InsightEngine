#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glslshader.h>

#define WIDTH 1280
#define HEIGHT 720

struct ISGraphics {
	static void init();
	static void update();
	static void draw();
	static void cleanup();

	struct ISModel {
		GLenum primititiveType{};
		GLSLShader shader_program{};
		GLuint vao_ID{};
		GLuint draw_count{};

		// imgui
		std::string name{ "Model" };
		float size{ .1f };
		bool drawing{ true };
		float color[3]{ 1.f, 0.f, 0.f };

		ISModel(std::string const& model_name) : name(model_name) {}
		void setupVAO();
		void setupShaders();
		void draw();
	};

	static ISModel test_model;
};
