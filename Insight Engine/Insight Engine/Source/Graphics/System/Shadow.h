#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SHADOW_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SHADOW_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Mesh.h"

namespace IS {
    class Shadow {
    public:
        enum shadowAttribs {
            pos_attrib,
            color_attrib,
            x_form_row1_attrib, // shader only able to take in max 4 values at a time
            x_form_row2_attrib, // so we will have to break a 3x3 matrix into 3 vec3
            x_form_row3_attrib,
            x_form_row4_attrib,
            TL_coord_attrib,
            TR_coord_attrib,
            BR_coord_attrib,
            BL_coord_attrib
        };

        struct shadowInstanceData {
            glm::vec4 color{};
            float tex_index{ -1 };
            glm::mat4 model_to_ndc_xform{};
            glm::vec2 anim_frame_dimension{ 1.f, 1.f }; // default UV size
            glm::vec2 anim_frame_index{ 0.f, 0.f };
            float entID{}; // initialize with invalid entity id
            int layer{};
            glm::vec2 TL_norm_coords{};
            glm::vec2 TR_norm_coords{};
            glm::vec2 BR_norm_coords{};
            glm::vec2 BL_norm_coords{};
        };

        static Mesh shadowMesh;

        static void setupShadowQuadMesh();

    };
} // end namespace IS
#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SHADOW_H
