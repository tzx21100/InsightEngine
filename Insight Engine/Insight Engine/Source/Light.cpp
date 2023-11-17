#include "Pch.h"
#include "Light.h"

namespace IS {
	void Light::draw() {
		Sprite::instanceData3D lightData;

		lightData.color = { color, intensity };
	}
}