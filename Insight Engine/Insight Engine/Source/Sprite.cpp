/*!
 * \file Sprite.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This source  file defines the Sprite class, which represents a 2D sprite with transformation, rendering properties,
 * and animation capabilities.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#include "Pch.h"

namespace IS {

	void Sprite::transform() {
        // convert angle to radians
        float angle_rad = glm::radians(model_TRS.rotation);

        // to scale to world coordinates
        float map_scale_x = 2.f / WIDTH; 
        float map_scale_y = 2.f / HEIGHT;

        float sin_angle = sinf(angle_rad);
        float cos_angle = cosf(angle_rad);
        float model_scale_x = model_TRS.scaling.x / 2.f;
        float model_scale_y = model_TRS.scaling.y / 2.f;

        glm::mat3 world_to_NDC_xform = { (map_scale_x * model_scale_x * cos_angle),  (map_scale_y * model_scale_x * sin_angle),  0.f,   // column 1
                                         (map_scale_x * model_scale_y * -sin_angle), (map_scale_y * model_scale_y * cos_angle),  0.f,   // column 2
                                         (map_scale_x * model_TRS.world_position.x), (map_scale_y * model_TRS.world_position.y), 1.f }; // column 3

        // save matrix
        model_TRS.mdl_to_ndc_xform = world_to_NDC_xform;
	}

    void Sprite::drawSprite(const Mesh& mesh_used, Shader shader, GLuint texture_id) {
        // use sprite shader
        shader.use();
        
        // bind vao
        glBindVertexArray(mesh_used.vao_ID);

        // set uniforms
        shader.setUniform("uColor", color);
        shader.setUniform("uModel_to_NDC", model_TRS.mdl_to_ndc_xform);

        // Bind the texture to the uniform sampler2D
        if (glIsTexture(texture_id)) {
            GLuint textureUniformLoc = glGetUniformLocation(shader.getHandle(), "uTex2d");
            if (textureUniformLoc != -1) {
                glUniform1i(textureUniformLoc, 0); // Bind to texture unit 0
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_id);
            }
            shader.setUniform("uTexture", 1);
        }
        else shader.setUniform("uTexture", 0);

        // set animation uniforms to default value
        shader.setUniform("uFrameDim", glm::vec2(1.f, 1.f));
        shader.setUniform("uFrameIndex", glm::vec2(0.f, 0.f));

        // for each mesh type
        switch (primitive_type) {
        case GL_TRIANGLE_STRIP:
            glDrawArrays(primitive_type, 0, mesh_used.draw_count);
            break;
        case GL_POINTS:
            glPointSize(8.f);
            glDrawArrays(primitive_type, 0, mesh_used.draw_count);
            glPointSize(1.f);
            break;
        case GL_LINES:
            glLineWidth(3.f);
            glDrawArrays(primitive_type, 0, mesh_used.draw_count);
            glLineWidth(1.f);
            break;
        case GL_TRIANGLE_FAN:
            glDrawArrays(primitive_type, 0, mesh_used.draw_count);
            break;
        }

        // unbind texture and vao
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader.unUse();
    }

    void Sprite::drawAnimation(const Mesh& mesh_used, Shader shader, Animation const& anim, GLuint texture_id) {
        // use sprite shader
        shader.use();

        // bind vao
        glBindVertexArray(mesh_used.vao_ID);
        
        // set uniforms
        shader.setUniform("uColor", color);
        shader.setUniform("uModel_to_NDC", model_TRS.mdl_to_ndc_xform);

        // set spritesheet texture
        GLuint textureUniformLoc = glGetUniformLocation(shader.getHandle(), "uTex2d");
        if (textureUniformLoc != -1) {
            glUniform1i(textureUniformLoc, 0); // Bind to texture unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_id);
        }
        shader.setUniform("uTexture", 1);

        // animation uniforms
        shader.setUniform("uFrameDim", glm::vec2(anim.getFrameWidth(), anim.getFrameHeight()));
        shader.setUniform("uFrameIndex", glm::vec2(anim.x_frame_index, anim.y_frame_index));

        // transformation matrix uniform
        shader.setUniform("uModel_to_NDC", model_TRS.mdl_to_ndc_xform);

        // draw quad
        glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh_used.draw_count);

        // unbind texture and vao
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader.unUse();
    }

    void Sprite::drawLine(Vector2D const& p0, Vector2D const& p1) {
        // ensure line mesh is initialized
        if (ISGraphics::meshes.empty()) {
            std::cerr << "No mesh to draw line!" << std::endl;
            return;
        }
        Mesh mesh_used = ISGraphics::meshes[2];
        float delta_x = p1.x - p0.x;
        float delta_y = p1.y - p0.y;

        // Translation
        Vector2D midpoint = (p0 + p1) / 2.f;

        // Rotation
        float angle = atan2f(delta_y, delta_x);
        angle += PI; // atan returns [-PI, PI]

        // Scaling
        float length = ISVector2DDistance(p0, p1);

        // get line scaling matrix
        glm::mat3 world_to_NDC_xform = lineTransform(midpoint, angle, length);

        // render
        Shader shader = ISGraphics::mesh_shader_pgm;
        shader.use();

        // bind line mesh vao
        glBindVertexArray(mesh_used.vao_ID);

        // set uniforms
        shader.setUniform("uColor", glm::vec3(1.f, 1.f, 1.f));
        shader.setUniform("uModel_to_NDC", world_to_NDC_xform);
        shader.setUniform("uTexture", 0);
        shader.setUniform("uFrameDim", glm::vec2(1.f, 1.f));
        shader.setUniform("uFrameIndex", glm::vec2(0.f, 0.f));
       
        // draw line
        glDrawArrays(GL_LINES, 0, mesh_used.draw_count);
        glLineWidth(1.f);

        // unbind texture and vao
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader.unUse();
    }

    glm::mat3 Sprite::lineTransform(Vector2D const& midpoint_translate, float rotate_angle_rad, float length_scale) {
        // similar to transform
        float map_scale_x = 2.f / WIDTH;
        float map_scale_y = 2.f / HEIGHT;

        float sin_angle = sinf(rotate_angle_rad);
        float cos_angle = cosf(rotate_angle_rad);

        float model_scale_x = length_scale / 2.f;
        float model_scale_y = 1.f; // no Y-scaling for line

        glm::mat3 world_to_NDC_xform = { (map_scale_x * model_scale_x * cos_angle),  (map_scale_y * model_scale_x * sin_angle),  0.f,   // column 1
                                         (map_scale_x * model_scale_y * -sin_angle), (map_scale_y * model_scale_y * cos_angle),  0.f,   // column 2
                                         (map_scale_x * midpoint_translate.x),       (map_scale_y * midpoint_translate.y),       1.f }; // column 3


        return world_to_NDC_xform;
    }
}