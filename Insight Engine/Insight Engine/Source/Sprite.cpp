#include "Pch.h"

namespace IS {

	void Sprite::transform() {

        float angle_rad = glm::radians(model_TRS.rotation);

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

    void Sprite::drawAnimation(const Mesh& mesh_used, Shader shader, Animation const& anim, GLuint texture_id) {
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
        shader.setUniform("uFrameDim", glm::vec2(anim.getFrameWidth(), anim.getFrameHeight()));
        shader.setUniform("uFrameIndex", glm::vec2(anim.x_frame_index, anim.y_frame_index));

        shader.setUniform("uModel_to_NDC", model_TRS.mdl_to_ndc_xform);


       glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh_used.draw_count);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader.unUse();
    }

    void Sprite::drawLine(Vector2D const& p0, Vector2D const& p1) {
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

        glm::mat3 world_to_NDC_xform = lineTransform(midpoint, angle, length);

        // render
        Shader shader = ISGraphics::mesh_shader_pgm;
        shader.use();

        glBindVertexArray(mesh_used.vao_ID);

        shader.setUniform("uColor", glm::vec3(1.f, 1.f, 1.f));
        shader.setUniform("uModel_to_NDC", world_to_NDC_xform);
        shader.setUniform("uTexture", 0);
        shader.setUniform("uFrameDim", glm::vec2(1.f, 1.f));
        shader.setUniform("uFrameIndex", glm::vec2(0.f, 0.f));
       
        //glLineWidth(0.5f);
        glDrawArrays(GL_LINES, 0, mesh_used.draw_count);
        glLineWidth(1.f);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader.unUse();
    }

    glm::mat3 Sprite::lineTransform(Vector2D const& midpoint_translate, float rotate_angle_rad, float length_scale) {
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