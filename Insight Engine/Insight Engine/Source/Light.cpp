#include "Pch.h"
#include "Light.h"

namespace IS {
	void Light::init(glm::vec2 pos, glm::vec3 clr, float inten, float sz) {
		position = pos;
		color = clr;
		intensity = inten;
		size = sz;
	}

	void Light::draw() {
		Sprite::instanceData3D lightData;

		lightData.color = { color, intensity };

		Transform lightXform({ position.x, position.y }, 0.f, { size, size });
		lightData.model_to_ndc_xform = lightXform.Return3DXformMatrix();

		ISGraphics::lightInstances.emplace_back(lightData);
	}
}