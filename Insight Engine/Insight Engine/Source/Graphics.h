#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Transform.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Framebuffer.h"
#include "Texture.h"
#include "Animation.h"

//#include "Asset.h"

namespace IS {
	class ISGraphics : public ParentSystem {
	public:

		//override parent
		void Initialize() override;
		void Update(float deltaTime) override;
		std::string GetName() override;
		void HandleMessage(const Message& msg) override;
		void Draw(float delta) override;
		
		static void cleanup();

		static GLuint initTextures(Image& image);
		static void initMeshes();
		static GLuint getScreenTexture();
		static void resizeFramebuffer(GLuint w, GLuint h);


		static Animation idle_ani;
		static Animation walking_ani;
		static Animation ice_cream_truck_ani;
	#ifdef USING_IMGUI
		static std::shared_ptr<Framebuffer> framebuffer;
	#endif // USING_IMGUI
		static Shader mesh_shader_pgm;
		static Shader text_shader_pgm;
		static Text cascadia_text;

		static std::vector<Mesh> meshes;
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



