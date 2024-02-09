/*!
 * \file ShaderEffects.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2451
 * \date 25-11-2023
 * \brief
 * This source file defines the ShaderEffect class, which manages shader generated visual effects.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#include "Pch.h"
#include "ShaderEffects.h"

namespace IS {
    // Static member initialization
    ShaderEffect::shaderEffect ShaderEffect::currentShaderEffect = shaderEffect::no_effect;
    ShaderEffect::shaderEffect ShaderEffect::prevShaderEffect = shaderEffect::no_effect;

    float ShaderEffect::shader_effect_timer = 0.f;

    // Static method implementation
    void ShaderEffect::play_static_glitch() {
        prevShaderEffect = currentShaderEffect;
        currentShaderEffect = shaderEffect::static_glitch;
    }

    // fragment collection visual effect
    void ShaderEffect::play_fragment_light() {
        shader_effect_timer = 4.2f;
        prevShaderEffect = currentShaderEffect;
        currentShaderEffect = shaderEffect::fragment_light;
    }

    // updating the shader effect
    void ShaderEffect::shader_effect_update(float dt) {
        shader_effect_timer -= dt;
        if (shader_effect_timer <= 0.f) {
           currentShaderEffect = prevShaderEffect;
        }
    }
}