/*!
 * \file Graphics.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
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

#include "Graphics/System/Shader.h"
#include "Graphics/System/Transform.h"
#include "Graphics/System/Sprite.h"
#include "Graphics/System/Mesh.h"
#include "Graphics/System/Text.h"
#include "Graphics/System/Animation.h"
#include "Graphics/System/Camera.h"
#include "Graphics/System/Camera3D.h"
#include "Graphics/System/Layering.h"

#include "Graphics/Buffers/Framebuffer.h"

namespace IS {
	/*!
	 * \brief Clears OpenGL errors.
	 */
	void ClearOpenGLError();

	/*!
	 * \brief Checks if any OpenGL errors occurred.
	 * \return True if there is an error, false otherwise.
	 */
	bool GotError();

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
		* \brief Updates the ISGraphics system.
		* \param delta_time The time difference since the last update.
		*/
		void Update(float deltaTime) override;

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

		/**
		 * Set the line width for OpenGL rendering using glLineWidth.
		 *
		 * @param lWidth - The desired line width to set in OpenGL.
		 */
		static void setLineWidth(float lWidth) { glLineWidth(lWidth); }

		
		static void deleteTexture(Image& image);


		/// Static objects ///

		// Frame Buffer
		static std::shared_ptr<Framebuffer> mFramebuffer;

		// Meshes vector
		static std::vector<Mesh> meshes;

		// Shaders
		static Shader fb_shader_pgm;
		static Shader main_quad_shader;
		static Shader quad_shader_pgm;
		static Shader glitched_quad_shader_pgm;
		static Shader non_quad_shader_pgm;
		static Shader quad_border_shader_pgm;
		static Shader light_shader_pgm;

		// Texture vector
		static std::vector<Image> textures;

		// instance data containers
		static std::multiset<Sprite::instanceData, Layering::GfxLayerComparator> layeredQuadInstances;
		static std::vector<Sprite::nonQuadInstanceData> lineInstances;
		static std::vector<Sprite::nonQuadInstanceData> circleInstances;
		static std::vector<Sprite::instanceData> lightInstances;

		// Editor and entity camera
		static Camera3D cameras3D[2];

		// Text Objects
		static Text Times_New_Roman_font;
		static Text Brush_Script_font;
		static Text North_Forest_font;

		// Flags
		static bool mGlitched;
		static bool mLightsOn;

		// Layers
		static std::vector<Layering>mLayers;

		static void EnableLayer(Layering layer) {
			layer.mLayerActive = true;
		}

		static void DisableLayer(Layering layer) {
			layer.mLayerActive = false;
		}

		static void ChangeLayerPosition(int layer, int layer2){
			//swap positions in the vector
			std::swap(mLayers[layer], mLayers[layer2]);
		}

		static void AddLayer() {
			Layering new_layer;
			new_layer.mLayerActive = true;
			mLayers.emplace_back(new_layer); 
		}

		static void RemoveLayer(int layerID) {
			mLayers.erase(mLayers.begin()+layerID);
		}

		static void AddEntityToLayer(int layer,Entity entity) {
			mLayers[layer].AddEntity(entity);
			auto& spr = InsightEngine::Instance().GetComponent<Sprite>(entity);
			spr.layer = layer;
		}

		static void RemoveEntityFromLayer(int layer, Entity entity) {
			mLayers[layer].RemoveEntity(entity);
		}

		static void RemoveEntityFromLayer(Entity entity) {
			for (auto& layer : mLayers) {
				layer.RemoveEntity(entity);
			}
		}

		static void ChangeEntityLayer(int layer, int new_layer, Entity entity) {
			RemoveEntityFromLayer(layer, entity);
			AddEntityToLayer(new_layer, entity);
		}


	};

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_GRAPHICS_H
