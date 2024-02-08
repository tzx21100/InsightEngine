/*!
 * \file Animation.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 09-02-2024
 * \brief
 * This header file defines the ShaderEffect class, which manages shader generated visual effects.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                      guard
 ----------------------------------------------------------------------------- */
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
        static float shader_effect_timer;
    };
} // end namespace IS
#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SHADEREFFECTS_H