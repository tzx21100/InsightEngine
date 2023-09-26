#pragma once
#include <glad/glad.h>

namespace IS {
	class Mesh {
	public:
		GLuint vao_ID{};
		GLuint animation_vao_ID{};
		GLuint vbo_ID{};
		GLuint draw_count{};

		struct Vertex {
			glm::vec2 position;
			glm::vec2 texCoord;
		};

		/*std::array<Vertex, 4> og_vertices {
			Vertex{glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
			Vertex{glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
			Vertex{glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			Vertex{glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
		};
		std::array<Vertex, 4> animation_vertices{};*/

		static void initMeshes(std::vector<Mesh>& meshes);
		static void cleanupMeshes(std::vector<Mesh>& meshes);
		void setupQuadVAO();
		void setupNonQuadVAO(GLenum mesh_primitive_type);
	};
	//static Mesh quad_mesh, points_mesh, lines_mesh, circle_mesh;
}

