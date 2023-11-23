#include "Pch.h"
#include "Light.h"
#include "Transform.h"

namespace IS {


	void Light::draw()
	{
		Sprite::instanceData3D lightData;

		lightData.color = { mHue.x, mHue.y, mHue.z, mIntensity };

		Transform lightXform(mPosition, 0.f, { mSize, mSize });
		lightData.model_to_ndc_xform = lightXform.Return3DXformMatrix();

		ISGraphics::lightInstances.emplace_back(lightData);
	}

	//Vector2D mOffset{};
	//Vector3D mColor{ 1.f, 1.f, 1.f };
	//float mIntensity{ .5f }; // 0 - 1.f
	//float mSize{ 200.f };
	void Light::FollowTransform(Vector2D position)
	{
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
		return data;
	}

	void Light::Deserialize(Json::Value data)
	{
		mOffset = { data["LightOffset"]["X"].asFloat(), data["LightOffset"]["Y"].asFloat() };
		mHue = { data["LightHue"]["R"].asFloat(), data["LightHue"]["G"].asFloat(), data["LightHue"]["B"].asFloat() };
		mIntensity = data["LightIntensity"].asFloat();
		mSize = data["LightSize"].asFloat();
	}
}