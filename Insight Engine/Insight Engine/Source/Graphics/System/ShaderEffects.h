/*!
 * \file Animation.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2451
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
        /*!
         * \enum shaderEffect
         * \brief Enumerates the types of shader effects that can be applied.
         *
         * This enumeration lists the various shader effects supported by the ShaderEffect class,
         * allowing for easy identification and switching between effects.
         */
        enum class shaderEffect {
            no_effect,
            dimmed,
            monochrome,
            static_glitch,
            fragment_light
        };

        
        /*!
         * \brief Applies a static glitch effect to the current rendering scene.
         *
         * This static method activates the static glitch shader effect, modifying the rendering
         * to simulate a glitchy visual appearance. The effect is applied by setting the currentShaderEffect
         * to static_glitch. This method is intended for use when a visual disruption or glitch-like
         * animation is desired in the scene.
         */
        static void play_static_glitch();

        /*!
         * \brief Applies a fragment-based lighting effect to enhance the scene's lighting dynamically.
         *
         * This static method enables the fragment lighting shader effect, aiming to improve the lighting
         * quality by applying more sophisticated lighting calculations on a per-fragment basis. It sets
         * the currentShaderEffect to fragment_light, suitable for scenes requiring enhanced lighting
         * details or atmospheric effects.
         */
        static void play_fragment_light();

        /*!
         * \brief Updates the active shader effect based on elapsed time.
         *
         * This static method is responsible for updating the parameters or intensity of the currently
         * active shader effect, utilizing the delta time to ensure smooth transitions and animations.
         * The method adapts the effect's behavior over time, allowing for dynamic visual experiences.
         *
         * \param dt The delta time since the last update, used to adjust effect parameters over time.
         */
        static void shader_effect_update(float dt);

        static shaderEffect currentShaderEffect;
        static shaderEffect prevShaderEffect;
        static float shader_effect_timer;
    };
} // end namespace IS
#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SHADEREFFECTS_H