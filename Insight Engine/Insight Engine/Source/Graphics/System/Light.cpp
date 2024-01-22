/*!
 * \file Light.cpp
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

#include "Pch.h"
#include "Light.h"
#include "Transform.h"
#include "Sprite.h"
#include "Graphics/Core/Graphics.h"

namespace IS {
	std::vector<glm::vec2> Light::lightPos;

	void Light::draw(float attachedEntID)
	{
		if (mRender) { // if draw light flag is true
			Sprite::instanceData lightData;

			lightData.color = { mHue.x, mHue.y, mHue.z, mIntensity };

			Transform lightXform(mPosition, 0.f, { mSize, mSize });
			lightData.model_to_ndc_xform = lightXform.Return3DXformMatrix();
			lightData.entID = attachedEntID; // to allow mousepicking past light

			lightPos.emplace_back(mPosition.x, mPosition.y);

			ISGraphics::lightInstances.emplace_back(lightData);
		}
	}

	void Light::FollowTransform(Vector2D position)
	{
		// set position to offset of attached entity
		mPosition = position + mOffset;
	}

	Json::Value Light::Serialize()
	{
		Json::Value data;

		Json::Value offset;
		offset["X"] = mOffset.x;
		offset["Y"] = mOffset.y;

		Json::Value color;
		color["R"] = mHue.x;
		color["G"] = mHue.y;
		color["B"] = mHue.z;

		data["LightOffset"] = offset;
		data["LightHue"] = color;
		data["LightIntensity"] = mIntensity;
		data["LightSize"] = mSize;
		data["LightRender"] = mRender;
		return data;
	}

	void Light::Deserialize(Json::Value data)
	{
		mOffset = { data["LightOffset"]["X"].asFloat(), data["LightOffset"]["Y"].asFloat() };
		mHue = { data["LightHue"]["R"].asFloat(), data["LightHue"]["G"].asFloat(), data["LightHue"]["B"].asFloat() };
		mIntensity = data["LightIntensity"].asFloat();
		mSize = data["LightSize"].asFloat();
		mRender = data["LightRender"].asBool();
	}
}