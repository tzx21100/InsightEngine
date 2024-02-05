#include "Pch.h"
#include "ShaderEffects.h"

namespace IS {
    ShaderEffect::shaderEffect ShaderEffect::currentShaderEffect = shaderEffect::no_effect;
    float ShaderEffect::shader_effect_timer = 0.f;

    void ShaderEffect::play_static_glitch() {
        currentShaderEffect = shaderEffect::static_glitch;
    }

    void ShaderEffect::play_fragment_light() {
        shader_effect_timer = 4.2f;
        currentShaderEffect = shaderEffect::fragment_light;
    }

    void ShaderEffect::shader_effect_update(float dt) {
        shader_effect_timer -= dt;
        if (shader_effect_timer <= 0.f) {
            currentShaderEffect = shaderEffect::no_effect;
        }
    }
}