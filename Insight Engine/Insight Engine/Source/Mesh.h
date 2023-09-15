#pragma once
#include <GL/glew.h>

namespace IS {
	class Mesh {
	public:
		GLuint vao_ID{};
		GLuint draw_count{};

		void setupQuadVAO();
		void setupNonQuadVAO(GLenum mesh_primitive_type);
		//static void init4Meshes();
		//static void cleanup4Meshes();
	};
	//static Mesh quad_mesh, points_mesh, lines_mesh, circle_mesh;
}

