#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SHADEREFFECTS_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SHADEREFFECTS_H

#include <glad/glad.h>
#include <glm/glm.hpp>

//include graphics.h


namespace IS {
    class ShaderEffect {
    public:
        enum class shaderEffect {
            no_effect,
            dimmed,
            monochrome,
            static_glitch,
            fragment_light
        };

        static void play_static_glitch();
        static void play_fragment_light();
        static void shader_effect_update(float dt);


        static shaderEffect currentShaderEffect;
        static shaderEffect prevShaderEffect;
        static float shader_effect_timer;
    };
} // end namespace IS
#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SHADEREFFECTS_H