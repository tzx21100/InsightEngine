/*!
 * \file Light.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
 * \brief
 * This file defines the Light class, representing a light component used in the graphics system.
 *
 * The Light class encapsulates properties and behavior for a light source in the graphics system.
 * It includes attributes such as position, color, intensity, size, and rendering status. The class
 * inherits from the IComponent interface, allowing it to be part of an entity's components.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/


#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H

#include "Transform.h"
#include <glm/glm.hpp>

namespace IS {

	/*!
	 * \class Light
	 * \brief Represents a light component used in the graphics system.
	 *
	 * The Light class defines properties and behavior for a light source in the graphics system.
	 * It includes attributes such as position, color, intensity, size, and rendering status.
	 * This class inherits from the IComponent interface.
	 */
	class Light : public IComponent {
	public:
		/*!
		 * \brief Retrieves the type of the Light component.
		 * \return A string representing the type of the Light component.
		 */
		static std::string GetType() {
			return "Light";
		}
		
		/*!
		 * \brief Default constructor for the Light class.
		 */
		Light() = default;

		/*!
		 * \brief Parameterized constructor for the Light class.
		 * \param offset The offset of the light from the entity's position.
		 * \param hue The color of the light (RGB values).
		 * \param intensity The intensity of the light (0.0 to 1.0).
		 * \param size The size of the light.
		 * \param render The rendering status of the light.
		 */
		Light(Vector2D offset, Vector3D const& hue, float intensity, float size, bool render)
			: mOffset{ offset }, mHue{ hue }, mIntensity{ intensity }, mSize{ size }, mRender{ render } {};

		/*!
		 * \brief Draws the light and adds its data to the graphics system for rendering.
		 * \param attachedEntID The entity ID to which the light is attached.
		 */
		void draw(float attachedEntID);

		/*!
		 * \brief Updates the position of the light based on the provided position.
		 * \param position The new position to follow.
		 */
		void FollowTransform(Vector2D position);

		/*!
		 * \brief Serializes the Light component data to a Json::Value object.
		 * \return The serialized data in Json::Value format.
		 */
		Json::Value Serialize() override;

		/*!
		 * \brief Deserializes the Light component data from a Json::Value object.
		 * \param data The Json::Value object containing the serialized data.
		 */
		void Deserialize(Json::Value data) override;

		Vector2D mPosition{};
		Vector2D mOffset{};
		Vector3D mHue{ 1.f, 1.f, 1.f };
		float mIntensity{ .5f }; // 0 - 1.f
		float mSize{ 200.f };
		bool mRender{ true };

		static std::vector<glm::vec2> lightPos;
		static std::vector<glm::vec4> lightClr;
	};

}// end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H