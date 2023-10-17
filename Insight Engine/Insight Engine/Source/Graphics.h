/*!
 * \file Graphics.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This source file defines the implementation for class ISGraphics which
 * encapsulates the functionalities of a Graphics Engine.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                      guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_GRAPHICS_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_GRAPHICS_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Transform.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Framebuffer.h"
#include "Animation.h"

namespace IS {
	/*!
	 * \brief The ISGraphics class encapsulates the functionalities of a Graphics Engine.
	 *
	 * ISGraphics is a subclass of ParentSystem and handles the rendering of graphics in the application.
	 */
	class ISGraphics : public ParentSystem {
	public:

		/*!
		 * \brief Initializes the ISGraphics system.
		 */
		void Initialize() override;

		/*!
		* \brief Updates the ISGraphics system.
		* \param delta_time The time difference since the last update.
		*/
		void Update(float deltaTime) override;

		/*!
		 * \brief Gets the name of the ISGraphics system.
		 * \return The name of the ISGraphics system.
		 */
		std::string GetName() override;

		/*!
		 * \brief Handles a message received by the ISGraphics system.
		 * \param msg The message to be handled.
		 */
		void HandleMessage(const Message& msg) override;

		/*!
		* \brief Draws the graphics based on the current state of the application.
		* \param delta_time The time difference since the last draw call.
		*/
		void Draw(float delta) override;
		
		/*!
		 * \brief Cleans up resources used by the ISGraphics system.
		 */
		static void cleanup();

		/*!
		* \brief Initializes textures for rendering.
		* \param filepath The file path to the texture image.
		* \param image An Image struct to store texture information.
		*/
		static void initTextures(const std::string& filepath, Image& image);

		/*!
		 * \brief Gets the screen texture ID.
		 * \return The texture ID of the screen.
		 */
		static GLuint GetScreenTexture();

		/*!
		 * \brief Resizes the framebuffer to the specified width and height.
		 * \param width The new width of the framebuffer.
		 * \param height The new height of the framebuffer.
		 */
		static void ResizeFramebuffer(GLuint width, GLuint height);

		static std::pair<GLuint, GLuint> GetFramebufferSize() { return mFramebuffer->GetSize(); }

		// Static objects
		static std::vector<Sprite::instanceData> quadInstances;
		// Sprites (models) to render
		static std::vector<Sprite> sprites;
		// Animation objects
		static Animation idle_ani;
		static Animation walking_ani;
		static Animation ice_cream_truck_ani;
		// Shaders
		static Shader mesh_shader_pgm;
		static Shader text_shader_pgm;
		// Mesh vector
		static std::vector<Mesh> meshes;
		// Text objects
		static GLuint font_texture;
		static Text cascadia_text;
		// Frame Buffer
		static std::shared_ptr<Framebuffer> mFramebuffer;
	};
} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_GRAPHICS_H
