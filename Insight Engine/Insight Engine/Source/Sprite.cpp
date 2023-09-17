#include "Pch.h"

namespace IS {
	void Sprite::transform(float delta_time) {
        // xform
        if (name == "Circle") model_TRS.scaling.y = model_TRS.scaling.x; // special case for circle sprite

        // angle updates
        model_TRS.orientation.x += model_TRS.orientation.y * delta_time;
        model_TRS.orientation.x = fmod(model_TRS.orientation.x, 360.f);
        if (model_TRS.orientation.x < 0.f) model_TRS.orientation.x += 360.f;

        float angle_rad = glm::radians(model_TRS.orientation.x);

        float map_scale_x = 2.f / WIDTH; 
        float map_scale_y = 2.f / HEIGHT;

        float sin_angle = sinf(angle_rad);
        float cos_angle = cosf(angle_rad);

        float model_scale_x = model_TRS.scaling.x / 2.f;
        float model_scale_y = model_TRS.scaling.y / 2.f;

        glm::mat3 world_to_NDC_xform = { (map_scale_x * model_scale_x * cos_angle),  (map_scale_y * model_scale_x * sin_angle),  0.f,   // column 1
                                         (map_scale_x * model_scale_y * -sin_angle), (map_scale_y * model_scale_y * cos_angle),  0.f,   // column 2
                                         (map_scale_x * model_TRS.world_position.x), (map_scale_y * model_TRS.world_position.y), 1.f }; // column 3
        

        model_TRS.mdl_to_ndc_xform = world_to_NDC_xform;
	}

    void Sprite::drawSpecial(const Mesh& mesh_used, Shader shader, GLuint texture_id) {
        shader.use();

        glBindVertexArray(mesh_used.vao_ID);

        //shader_program.SetUniform("uSize", test_model.size);
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

        shader.setUniform("uFrameDim", glm::vec2(1.f, 1.f));
        shader.setUniform("uFrameIndex", glm::vec2(0.f, 0.f));

        switch (primitive_type) {
        case GL_TRIANGLE_STRIP:
            glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh_used.draw_count);
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

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader.unUse();
    }

    void Sprite::drawAnimation(const Mesh& mesh_used, Shader shader, Animation const& animPointer, GLuint texture_id) {
        shader.use();

        glBindVertexArray(mesh_used.vao_ID);

        //shader_program.SetUniform("uSize", test_model.size);
        shader.setUniform("uColor", color);
        shader.setUniform("uModel_to_NDC", model_TRS.mdl_to_ndc_xform);

        GLuint textureUniformLoc = glGetUniformLocation(shader.getHandle(), "uTex2d");
        if (textureUniformLoc != -1) {
            glUniform1i(textureUniformLoc, 0); // Bind to texture unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_id);
        }
        shader.setUniform("uTexture", 1);

        // animation uniforms
        shader.setUniform("uFrameDim", glm::vec2(animPointer.getFrameWidth(), animPointer.getFrameHeight()));
        shader.setUniform("uFrameIndex", glm::vec2(animPointer.x_frame_index, animPointer.y_frame_index));

        shader.setUniform("uModel_to_NDC", model_TRS.mdl_to_ndc_xform);


       glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh_used.draw_count);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader.unUse();
    }
}