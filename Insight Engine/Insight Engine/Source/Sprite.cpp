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
        float sin_angle = sinf(angle_rad);
        float cos_angle = cosf(angle_rad);

        glm::mat3 world_to_NDC_xform = { (model_TRS.scaling.x) * cos_angle, (model_TRS.scaling.x) * -sin_angle, 0,   // column 1
                                         (model_TRS.scaling.y) * sin_angle, (model_TRS.scaling.y) * cos_angle,  0,   // column 2
                                         model_TRS.world_position.x,              model_TRS.world_position.y,               1 }; // column 3


        float map_scale_x = 0.00078125f; // 1/1280 (DEPENDANT ON WORLD SIZE)
        float map_scale_y = 0.00138889f; // 1/720

        glm::mat3 map_scale_xform = { map_scale_x, 0,         0,   // column 1
                                      0,         map_scale_y, 0,   // column 2
                                      0,         0,           1 }; // column 3

        model_TRS.mdl_to_ndl_xform = map_scale_xform * world_to_NDC_xform;
	}

    void Sprite::drawSpecial(Mesh mesh_used, Shader shader, GLuint texture_id) {
        shader.use();

        glBindVertexArray(mesh_used.vao_ID);

        //shader_program.SetUniform("uSize", test_model.size);
        shader.setUniform("uColor", color);
        shader.setUniform("uModel_to_NDC", model_TRS.mdl_to_ndl_xform);

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

        switch (primitive_type) {
        case GL_TRIANGLES:
            
            glDrawElements(primitive_type, mesh_used.draw_count, GL_UNSIGNED_INT, NULL);
            break;
        case GL_POINTS:
            glPointSize(5.f);
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
}