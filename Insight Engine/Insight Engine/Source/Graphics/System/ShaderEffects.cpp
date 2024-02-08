#include "Pch.h"
#include "ShaderEffects.h"

namespace IS {
    ShaderEffect::shaderEffect ShaderEffect::currentShaderEffect = shaderEffect::no_effect;
    ShaderEffect::shaderEffect ShaderEffect::prevShaderEffect = shaderEffect::no_effect;

    float ShaderEffect::shader_effect_timer = 0.f;

    void ShaderEffect::play_static_glitch() {
        prevShaderEffect = currentShaderEffect;
        currentShaderEffect = shaderEffect::static_glitch;
    }

    void ShaderEffect::play_fragment_light() {
        shader_effect_timer = 4.2f;
        prevShaderEffect = currentShaderEffect;
        currentShaderEffect = shaderEffect::fragment_light;
    }

    void ShaderEffect::shader_effect_update(float dt) {
        shader_effect_timer -= dt;
        if (shader_effect_timer <= 0.f) {
           currentShaderEffect = prevShaderEffect;
        }
    }
}