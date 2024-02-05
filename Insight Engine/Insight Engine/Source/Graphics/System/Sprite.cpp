/*!
 * \file Sprite.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
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
#include "Sprite.h"
#include "Light.h"
#include "Graphics/Core/Graphics.h"
#include <glm/gtc/type_ptr.hpp>

namespace IS {
    // set of layers to NOT render
    std::unordered_set<int> Sprite::layersToIgnore{};

    Sprite::Sprite() : name("Quad"), primitive_type(GL_TRIANGLE_STRIP) {
        // Give it a default size of 1 by 1
        setSpriteSize(1, 1);
        setWorldPos(0, 0);
    }

    Sprite::Sprite(std::string const& model_name, GLenum primitive) : name(model_name), primitive_type(primitive) {}

    Sprite::Sprite(const Sprite& other)
        : primitive_type(other.primitive_type),
        model_TRS(other.model_TRS),
        animation_index(other.animation_index),
        img(other.img),
        anims(other.anims),
        name(other.name),
        color(other.color)
    {
        img.texture_id = other.img.texture_id;
    }

    void Sprite::transform() {
        model_TRS.mdl_to_3dcam_to_ndc_xform = model_TRS.Return3DXformMatrix();
    }

    void Sprite::draw_instanced_quads() {
        // set shader
        Shader::setMainQuadShader(ISGraphics::quad_shader_pgm);

        // Bind the instance VBO
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[3].instance_vbo_ID));

        // Upload the quadInstances data to the GPU
        GL_CALL(Sprite::instanceData* buffer = reinterpret_cast<Sprite::instanceData*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)));


        // Copy the instance data from the multiset to the vector
        std::vector<Sprite::instanceData> tempData(ISGraphics::layeredQuadInstances.begin(), ISGraphics::layeredQuadInstances.end());

        if (buffer) {

            // Copy the instance data to the mapped buffer
            if (!tempData.empty()) {
                std::memcpy(buffer, tempData.data(), tempData.size() * sizeof(Sprite::instanceData));

                // Unmap the buffer
                if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE) { // 
                    // Handle the case where unmap was not successful
                    std::cerr << "Failed to unmap the buffer." << std::endl;
                }
            }
        }
        else {
            // Handle the case where mapping the buffer was not successful
            std::cerr << "Failed to map the buffer for writing." << std::endl;
        }

        // bind shader
        GL_CALL(glUseProgram(ISGraphics::main_quad_shader.getHandle()));
        GL_CALL(glBindVertexArray(ISGraphics::meshes[3].vao_ID)); // will change to enums

        // store texture array indices
        std::vector<int> tex_array_index_vect;
        for (auto const& texture : ISGraphics::textures) {
            glBindTextureUnit(texture.texture_index, texture.texture_id);
            tex_array_index_vect.emplace_back(texture.texture_index);
        }

        // upload to uniform variable
        auto tex_arr_uniform = glGetUniformLocation(ISGraphics::main_quad_shader.getHandle(), "uTex2d");
        if (tex_arr_uniform >= 0)
            glUniform1iv(tex_arr_uniform, static_cast<int>(tex_array_index_vect.size()), &tex_array_index_vect[0]);
        else IS_CORE_ERROR({ "uTex2d Uniform not found, shader compilation failed?" });

        // draw instanced quads
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, ISGraphics::meshes[3].draw_count, static_cast<GLsizei>(tempData.size()));

        for (auto const& texture : ISGraphics::textures)
        {
            glBindTextureUnit(texture.texture_index, 0);
        }

        ISGraphics::layeredQuadInstances.clear();
    }

    void Sprite::draw_instanced_glitched_quads() {
        // set to glitching effect shader
        Shader::setMainQuadShader(ISGraphics::glitched_quad_shader_pgm);

        // Bind the instance VBO
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[3].instance_vbo_ID));

        glUnmapBuffer(GL_ARRAY_BUFFER);
        // Upload the quadInstances data to the GPU
        GL_CALL(Sprite::instanceData * buffer = reinterpret_cast<Sprite::instanceData*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)));

        // Copy the instance data from the multiset to the vector
        std::vector<Sprite::instanceData> tempData(ISGraphics::layeredQuadInstances.begin(), ISGraphics::layeredQuadInstances.end());

        if (buffer) {

            // Copy the instance data to the mapped buffer
            if (!tempData.empty()) {
                std::memcpy(buffer, tempData.data(), tempData.size() * sizeof(Sprite::instanceData));

                // Unmap the buffer
                if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE) { // 
                    // Handle the case where unmap was not successful
                    std::cerr << "Failed to unmap the buffer." << std::endl;
                }
            }
        }
        else {
            // Handle the case where mapping the buffer was not successful
            std::cerr << "Failed to map the buffer for writing." << std::endl;
        }

        // bind shader
        GL_CALL(glUseProgram(ISGraphics::main_quad_shader.getHandle()));
        GL_CALL(glBindVertexArray(ISGraphics::meshes[3].vao_ID)); // will change to enums

        // store texture array indices
        std::vector<int> tex_array_index_vect;
        for (auto const& texture : ISGraphics::textures) {
            glBindTextureUnit(texture.texture_index, texture.texture_id);
            tex_array_index_vect.emplace_back(texture.texture_index);
        }

        // upload to uniform variable
        auto tex_arr_uniform = glGetUniformLocation(ISGraphics::main_quad_shader.getHandle(), "uTex2d");
        if (tex_arr_uniform >= 0)
            glUniform1iv(tex_arr_uniform, static_cast<int>(tex_array_index_vect.size()), &tex_array_index_vect[0]);
        else IS_CORE_ERROR({ "uTex2d Uniform not found, shader compilation failed?" });

        // upload to uniform variable for glitch effect animation
        auto globalTimeUniform = glGetUniformLocation(ISGraphics::main_quad_shader.getHandle(), "uGlobalTime");
        if (globalTimeUniform >= 0) {
            glUniform1f(globalTimeUniform, static_cast<float>(InsightEngine::Instance().mElapsedTime));
        }
        else {
            IS_CORE_ERROR("uGlobalTime Uniform not found, shader compilation failed?");
        }

        // draw instanced quads
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, ISGraphics::meshes[3].draw_count, static_cast<GLsizei>(tempData.size()));
        ISGraphics::layeredQuadInstances.clear();
    }
    

    void Sprite::draw_colored_quad(Vector2D const& pos, float rotation, Vector2D const& scale, Vector4D const& color, int layer) {
        // only render if layer not ignored
        if (Sprite::layersToIgnore.find(layer) == Sprite::layersToIgnore.end()) { 
            Transform quadTRS(pos, rotation, scale);

            // get line scaling matrix
            glm::mat4 world_to_NDC_xform = quadTRS.Return3DXformMatrix();
            Sprite::instanceData instData;
            instData.color = glm::vec4(color.x, color.y, color.z, color.w);
            instData.model_to_ndc_xform = world_to_NDC_xform;
            instData.layer = layer;

            // add to multiset to render everything at the end
            ISGraphics::layeredQuadInstances.insert(instData);
        }
    }

    void Sprite::draw_textured_quad(Vector2D const& pos, float rotation, Vector2D const& scale, Image const& texture, float alpha, int layer) {
        // only render if layer not ignored
        if (Sprite::layersToIgnore.find(layer) == Sprite::layersToIgnore.end()) {
            Transform quadTRS(pos, rotation, scale);

            // get line scaling matrix
            glm::mat4 world_to_NDC_xform = quadTRS.Return3DXformMatrix();
            Sprite::instanceData instData;
            instData.color = { 1.f, 1.f, 1.f, alpha };
            instData.tex_index = static_cast<float>(texture.texture_index);
            instData.model_to_ndc_xform = world_to_NDC_xform;
            instData.layer = layer;

            // add to multiset to render everything at the end
            ISGraphics::layeredQuadInstances.insert(instData);
        }
    }

    void Sprite::drawSpritesheetFrame(int rowIndex, int columnIndex, int totalRows, int totalCols, Vector2D const& pos, float rotation, Vector2D const& scale, Image const& texture, float alpha, int layer) {
        // error handling
        if (rowIndex < 0 || rowIndex >= totalRows || columnIndex < 0 || columnIndex >= totalCols) {
            IS_CORE_WARN("Index out of range for drawSpritesheetFrame function");
            return;
        }

        // only render if layer not ignored
        if (Sprite::layersToIgnore.find(layer) == Sprite::layersToIgnore.end()) {
            Transform quadTRS(pos, rotation, scale);

            // get line scaling matrix
            glm::mat4 world_to_NDC_xform = quadTRS.Return3DXformMatrix();
            Sprite::instanceData instData;
            instData.color = { 1.f, 1.f, 1.f, alpha };
            instData.tex_index = static_cast<float>(texture.texture_index);
            instData.model_to_ndc_xform = world_to_NDC_xform;
            instData.layer = layer;

            // set spritesheet variables
            instData.anim_frame_index = { columnIndex, rowIndex };
            instData.anim_frame_dimension = { 1.f / totalCols, 1.f / totalRows };

            // add to multiset to render everything at the end
            ISGraphics::layeredQuadInstances.insert(instData);
        }
    }

    void Sprite::draw_lights() {
        GLuint clr_attach_id = ISGraphics::mShaderFrameBuffer.GetColorAttachment(); // texture ID
        GLuint entt_attach_id = ISGraphics::mShaderFrameBuffer.GetEntityIDAttachment();
       /* Light::shadowLineSegments.emplace_back(glm::vec4(-100.f, 100.f, 100.f, 100.f));
        Light::shadowLineSegments.emplace_back(glm::vec4(100.f, 100.f, 100.f, -100.f));
        Light::shadowLineSegments.emplace_back(glm::vec4(100.f, -100.f, -100.f, -100.f));
        Light::shadowLineSegments.emplace_back(glm::vec4(-100.f, -100.f, -100.f, 100.f));*/


        //std::vector<glm::vec2> lightPos;

        //glm::mat4 camXform = ISGraphics::cameras3D[Camera3D::mActiveCamera].getCameraToNDCXform();

        //for (auto& lineSeg : line) {
        //    glm::vec2 p0 = glm::vec4(lineSeg.x, lineSeg.y, 0.f, 1.f) * camXform;
        //    glm::vec2 p1 = glm::vec4(lineSeg.z, lineSeg.w, 0.f, 1.f) * camXform;
        //    lineSeg.x = p0.x, lineSeg.y = p0.y, lineSeg.z = p1.x, lineSeg.w = p1.y;
        //}

        // Bind the instance VBO
        //GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[3].instance_vbo_ID));

        //glUnmapBuffer(GL_ARRAY_BUFFER);

        //// Upload the quadInstances data to the GPU
        //GL_CALL(Sprite::instanceData* buffer = reinterpret_cast<Sprite::instanceData*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)));

        //if (buffer) {
        //    // Copy the instance data to the mapped buffer
        //    std::memcpy(buffer, ISGraphics::lightInstances.data(), ISGraphics::lightInstances.size() * sizeof(Sprite::instanceData));

        //    // Unmap the buffer
        //    if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE) {
        //        // Handle the case where unmap was not successful
        //        std::cerr << "Failed to unmap the buffer." << std::endl;
        //    }

        //}
        //else {
        //    // Handle the case where mapping the buffer was not successful
        //    std::cerr << "Failed to map the buffer for writing." << std::endl;
        //}        glClear(GL_COLOR_BUFFER_BIT);
        // bind shader
        GL_CALL(glUseProgram(ISGraphics::light_shader_pgm.getHandle()));
        GL_CALL(glBindVertexArray(ISGraphics::meshes[5].vao_ID)); // will change to enums

        GLint tex_arr_uniform{};
        if (!Light::lightClr.empty()) {
            tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "uLightColors");
            if (tex_arr_uniform >= 0)
                glUniform4fv(tex_arr_uniform, static_cast<GLsizei>(Light::lightClr.size()), reinterpret_cast<const GLfloat*>(Light::lightClr.data()));
            else
                IS_CORE_ERROR("uLightColors Uniform not found, shader compilation failed?");
        }

        if (!ISGraphics::lightRadius.empty()) {
            tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "uLightRadius");
            if (tex_arr_uniform >= 0)
                glUniform1fv(tex_arr_uniform, static_cast<GLsizei>(ISGraphics::lightRadius.size()), ISGraphics::lightRadius.data());
            else
                IS_CORE_ERROR({ "uLightRadius Uniform not found, shader compilation failed" });
        }

        if (!Light::lightPos.empty()) {
            tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "uLightWorldPos");
            if (tex_arr_uniform >= 0)
                glUniform2fv(tex_arr_uniform, static_cast<GLsizei>(Light::lightPos.size()), reinterpret_cast<const GLfloat*>(Light::lightPos.data()));
            else
                IS_CORE_ERROR("uLightWorldPos Uniform not found, shader compilation failed?");
        }

        tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "uNoOfWorldLights");
        if (tex_arr_uniform >= 0)
            glUniform1i(tex_arr_uniform, static_cast<int>(Light::lightPos.size()));
        else 
            IS_CORE_ERROR({ "uNoOfWorldLights Uniform not found, shader compilation failed?" });

        if (!Light::shadowLineSegments.empty()) {
            tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "uLineSegments");
            if (tex_arr_uniform >= 0)
                glUniform4fv(tex_arr_uniform, static_cast<GLsizei>(Light::shadowLineSegments.size()), reinterpret_cast<const GLfloat*>(Light::shadowLineSegments.data()));
            else
                IS_CORE_ERROR("uLineSegments Uniform not found, shader compilation failed?");
        }

        tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "uNoOfLineSegments");
        if (tex_arr_uniform >= 0)
            glUniform1i(tex_arr_uniform, static_cast<int>(Light::shadowLineSegments.size()));
        else 
            IS_CORE_ERROR({ "uNoOfLineSegments Uniform not found, shader compilation failed?" });

        
        glm::vec2 resolution{};

        InsightEngine& engine = InsightEngine::Instance();

        if (!engine.mRenderGUI) {
            auto const& [width, height] = engine.GetWindowSize();
            resolution.x = static_cast<float>(width);
            resolution.y = static_cast<float>(height);
        }

#if defined(USING_IMGUI)

        auto const& editor_layer = engine.GetEditorLayer();
        if (engine.mRenderGUI)
        {
            // Use the viewport size from the editor layer
            resolution.x = editor_layer->GetViewportSize().x;
            resolution.y = editor_layer->GetViewportSize().y;
        }
#endif

        tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "uResolution");
        if (tex_arr_uniform >= 0)
            glUniform2fv(tex_arr_uniform, 1, &resolution.x);
        else
            IS_CORE_ERROR("uResolution Uniform not found, shader compilation failed?");

        tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "uInverseVP");
        if (tex_arr_uniform >= 0)
            glUniformMatrix4fv(tex_arr_uniform, 1, GL_FALSE, glm::value_ptr(ISGraphics::cameras3D[Camera3D::mActiveCamera].getInverseCameraToNDCXform()));
        else
            IS_CORE_ERROR({ "uInverseVP Uniform not found, shader compilation failed?" });
        
        GL_CALL(glBindTextureUnit(0, clr_attach_id));
        GL_CALL(glBindTextureUnit(1, entt_attach_id));

        tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "bg_tex");
        if (tex_arr_uniform >= 0)
            glUniform1i(tex_arr_uniform, 0);
        else
            IS_CORE_ERROR({ "bg_tex Uniform not found, shader compilation failed?" });

        tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "id_tex");
        if (tex_arr_uniform >= 0)
            glUniform1i(tex_arr_uniform, 1);
        else
            IS_CORE_ERROR({ "id_tex Uniform not found, shader compilation failed?" });

        tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "type_of_light");
        if (tex_arr_uniform >= 0)
            glUniform1i(tex_arr_uniform, ISGraphics::mLightType);
        else
            IS_CORE_ERROR({ "id_tex Uniform not found, shader compilation failed?" });

        tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "ilovetime");
        if (tex_arr_uniform >= 0)
            glUniform1f(tex_arr_uniform, static_cast<float>(glfwGetTime()));
        else
            IS_CORE_ERROR({ "id_tex Uniform not found, shader compilation failed?" });
        //glm::mat3 transform
        //{
        //    1.f, 0.f, 0.f,
        //    0.f, 1.f, 0.f,
        //    0.f, 0.f, 1.f,
        //};
        //tex_arr_uniform = glGetUniformLocation(ISGraphics::light_shader_pgm.getHandle(), "model_to_NDC_xform");
        //if (tex_arr_uniform >= 0)
        //    glUniformMatrix3fv(tex_arr_uniform, 1, GL_FALSE, glm::value_ptr(transform));
        //else
        //    IS_CORE_ERROR({ "id_tex Uniform not found, shader compilation failed?" });

        // draw instanced quads
        // GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
        // GL_CALL(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, ISGraphics::meshes[3].draw_count, 1));
        GL_CALL(glDrawArrays(GL_TRIANGLES, 0, ISGraphics::meshes[5].draw_count));
        GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        ISGraphics::lightRadius.clear();
        ISGraphics::lightInstances.clear();
        Light::lightPos.clear();
        Light::lightClr.clear();
        Light::shadowLineSegments.clear();
    }

    void Sprite::draw_picked_entity_border() {
    #ifdef USING_IMGUI
        InsightEngine& engine = InsightEngine::Instance();

        // No entity selected, do nothing
        if (!engine.GetEditorLayer()->IsAnyEntitySelected())
            return;

        Entity entity = engine.GetEditorLayer()->GetSelectedEntity();
        auto& sprite = engine.GetComponent<Sprite>(entity);

        // use sprite shader
        Shader shader = ISGraphics::quad_border_shader_pgm;
        shader.use();

        // bind vao
        glBindVertexArray(ISGraphics::meshes[4].vao_ID);

        shader.setUniform("model_to_ndc_xform", sprite.model_TRS.mdl_to_3dcam_to_ndc_xform);
        glDrawArrays(GL_LINE_LOOP, 0, ISGraphics::meshes[4].draw_count);
    #endif // USING_IMGUI
    }

    void Sprite::drawDebugLine(Vector2D const& p0, Vector2D const& p1, std::tuple<float, float, float> const& color, float lineLength, float angleInDegrees) {
        // Translation
        Vector2D midpoint = (p0 + p1) / 2.f;

        // Scaling
        if (lineLength < 0.f) {
            lineLength = ISVector2DDistance(p0, p1); // will not save
        }

        // Rotation
        float delta_x = p1.x - p0.x;
        float delta_y = p1.y - p0.y;
        if (angleInDegrees < 0.f) angleInDegrees = glm::degrees(atan2f(delta_y, delta_x));

        Transform lineTRS(midpoint, angleInDegrees, { lineLength, 0.f });

        // get line scaling matrix
        glm::mat4 world_to_NDC_xform = lineTRS.Return3DXformMatrix();
        Sprite::nonQuadInstanceData lineData;
        lineData.color = glm::vec4(std::get<0>(color), std::get<1>(color), std::get<2>(color), 1.f);
        lineData.model_to_ndc_xform = world_to_NDC_xform;

        // adds to vector of lines to draw
        ISGraphics::lineInstances.emplace_back(lineData);
    }

    void Sprite::draw_instanced_lines() {
        // Bind the instance VBO
        glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[1].instance_vbo_ID);
        // Upload the quadInstances data to the GPU
        Sprite::nonQuadInstanceData* buffer = reinterpret_cast<Sprite::nonQuadInstanceData*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

        if (buffer) {
            // Copy the instance data to the mapped buffer
            std::memcpy(buffer, ISGraphics::lineInstances.data(), ISGraphics::lineInstances.size() * sizeof(Sprite::nonQuadInstanceData));

            // Unmap the buffer
            if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE) {
                // Handle the case where unmap was not successful
                std::cerr << "Failed to unmap the buffer." << std::endl;
            }
        }
        else {
            // Handle the case where mapping the buffer was not successful
            std::cerr << "Failed to map the buffer for writing." << std::endl;
        }

        // bind shader
        glUseProgram(ISGraphics::non_quad_shader_pgm.getHandle());
        glBindVertexArray(ISGraphics::meshes[1].vao_ID);

        // draw instanced lines
        glDrawArraysInstanced(GL_LINES, 0, ISGraphics::meshes[1].draw_count, static_cast<GLsizei>(ISGraphics::lineInstances.size()));
        
        // clear vector of instance data
        ISGraphics::lineInstances.clear();
    }

    void Sprite::drawDebugCircle(Vector2D const& worldPos, Vector2D const& scale, std::tuple<float, float, float> const& color) {
        // TRS (0 rotation for circles)
        Transform CircleTRS(worldPos, 0.f, scale);
        glm::mat4 world_to_NDC_xform = CircleTRS.Return3DXformMatrix();

        // set up instance data
        Sprite::nonQuadInstanceData circleData;
        circleData.color = glm::vec4(std::get<0>(color), std::get<1>(color), std::get<2>(color), 1.f);
        circleData.model_to_ndc_xform = world_to_NDC_xform;

        // adds to vector of circles to draw
        ISGraphics::circleInstances.emplace_back(circleData);
    }

    void Sprite::draw_instanced_circles() {
        // Bind the instance VBO
        glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[2].instance_vbo_ID);
        // Upload the quadInstances data to the GPU
        Sprite::nonQuadInstanceData* buffer = reinterpret_cast<Sprite::nonQuadInstanceData*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

        if (buffer) {
            // Copy the instance data to the mapped buffer
            std::memcpy(buffer, ISGraphics::circleInstances.data(), ISGraphics::circleInstances.size() * sizeof(Sprite::nonQuadInstanceData));

            // Unmap the buffer
            if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE) {
                // Handle the case where unmap was not successful
                std::cerr << "Failed to unmap the buffer." << std::endl;
            }
        }
        else {
            // Handle the case where mapping the buffer was not successful
            std::cerr << "Failed to map the buffer for writing." << std::endl;
        }

        // bind shader
        glUseProgram(ISGraphics::non_quad_shader_pgm.getHandle());
        glBindVertexArray(ISGraphics::meshes[2].vao_ID);

        // draw instanced circles
        glDrawArraysInstanced(GL_LINE_LOOP, 0, ISGraphics::meshes[2].draw_count, static_cast<GLsizei>(ISGraphics::circleInstances.size()));
       
        // clear vector of instance data
        ISGraphics::circleInstances.clear();
    }

    void Sprite::AddAnimation(std::string const& anim_name, int columns, int rows, float animation_time) {
        // similar to init, but adding into a sprite directly
        Animation animation;
        animation.name = anim_name;
        animation.initAnimation(rows, columns, animation_time);
        anims.emplace_back(animation);
    }

    void Sprite::removeAnimation(int index) {
        if (index < 0 || index >= anims.size()) {
            IS_CORE_WARN("Invalid animation index selected.");
            return;
        }
        anims.erase(anims.begin() + index);
    }

    void Sprite::removeAllAnimations() {
        anims.clear();
    }

    void Sprite::drawAnimation(const Mesh& mesh_used, Shader shader, Animation const& animation, GLuint texture_ID) {
        // use sprite shader
        shader.use();

        // bind vao
        glBindVertexArray(mesh_used.vao_ID);

        // set uniforms
        shader.setUniform("uColor", color);
        shader.setUniform("uModel_to_NDC", ISMtx33ToGlmMat3(model_TRS.mdl_to_ndc_xform));

        // set spritesheet texture
        GLuint textureUniformLoc = glGetUniformLocation(shader.getHandle(), "uTex2d");
        if (textureUniformLoc != -1) {
            glUniform1i(textureUniformLoc, 0); // Bind to texture unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_ID);
        }
        shader.setUniform("uTexture", 1);

        // animation uniforms
        shader.setUniform("uFrameDim", glm::vec2(animation.getFrameWidth(), animation.getFrameHeight()));
        shader.setUniform("uFrameIndex", glm::vec2(animation.frame_index.x, animation.frame_index.y));

        // transformation matrix uniform
        shader.setUniform("uModel_to_NDC", ISMtx33ToGlmMat3(model_TRS.mdl_to_ndc_xform));

        // draw quad
        glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh_used.draw_count);

        // unbind texture and vao
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader.unUse();
    }

    void Sprite::toggleLayer(DrawLayer layer) {
        auto found = layersToIgnore.find(layer);

        // if layer specificed is not ignored, add to ignore set
        if (found == layersToIgnore.end()) layersToIgnore.insert(layer);

        // else, stop ignoring
        else layersToIgnore.erase(found);
    }

    Json::Value Sprite::Serialize() {
        Json::Value spriteData;

        // Serializing primitive_type
        spriteData["SpritePrimitiveType"] = primitive_type;

        // Serializing model_TRS (using the details of the Transform class)
        spriteData["SpriteTransformWorldPositionX"] = model_TRS.world_position.x;
        spriteData["SpriteTransformWorldPositionY"] = model_TRS.world_position.y;
        spriteData["SpriteTransformRotation"] = model_TRS.rotation;
        spriteData["SpriteTransformScalingX"] = model_TRS.scaling.x;
        spriteData["SpriteTransformScalingY"] = model_TRS.scaling.y;

        // Note: Not serializing mdl_to_ndc_xform as it's a matrix and the specific serialization might depend on further needs

        // Serializing texture-related properties
        spriteData["SpriteFilename"] = img.mFileName;
        spriteData["SpriteTexture"] = img.texture_id;
        spriteData["SpriteTextureWidth"] = img.width;
        spriteData["SpriteTextureHeight"] = img.height;
        spriteData["SpriteCurrentTexIndex"] = animation_index;
        spriteData["SpriteTextureIndex"] = img.texture_index;

        IS_CORE_DEBUG("SAVED FILENAME {}", img.mFileName);

        // Serializing imgui-related properties
        spriteData["SpriteName"] = name;
        spriteData["SpriteColorX"] = color.x;
        spriteData["SpriteColorY"] = color.y;
        spriteData["SpriteColorZ"] = color.z;
        spriteData["SpriteColorA"] = color.a;

        spriteData["AnimationIndex"] = animation_index;
        spriteData["Layer"] = layer;

        Json::Value animationsJson(Json::arrayValue);
        for (const Animation& animation : anims) {
            Json::Value animationData;

            animationData["Name"] = animation.name;
            animationData["frame_dimension_x"] = animation.frame_dimension.x;
            animationData["frame_dimension_y"] = animation.frame_dimension.y;
            animationData["frame_index_x"] = animation.frame_index.x;
            animationData["frame_index_y"] = animation.frame_index.y;
            animationData["x_frames"] = animation.x_frames;
            animationData["y_frames"] = animation.y_frames;
            animationData["animation_duration"] = animation.animation_duration;
            animationData["time_per_frame"] = animation.time_per_frame;
            animationData["frame_timer"] = animation.frame_timer;

            animationsJson.append(animationData);
        }
        spriteData["animations"] = animationsJson;

        return spriteData;
    }

    void Sprite::Deserialize(Json::Value data) {
        // Deserializing primitive_type
        primitive_type = data["SpritePrimitiveType"].asUInt();

        // Deserializing model_TRS
        model_TRS.world_position.x = data["SpriteTransformWorldPositionX"].asFloat();
        model_TRS.world_position.y = data["SpriteTransformWorldPositionY"].asFloat();
        model_TRS.rotation = data["SpriteTransformRotation"].asFloat();
        model_TRS.scaling.x = data["SpriteTransformScalingX"].asFloat();
        model_TRS.scaling.y = data["SpriteTransformScalingY"].asFloat();

        // Note: Not deserializing mdl_to_ndc_xform as it's a matrix and the specific deserialization might depend on further needs

        // Deserializing texture-related properties
        img.mFileName = data["SpriteFilename"].asString();
        img.texture_id = static_cast<uint8_t>(data["SpriteTexture"].asUInt());
        img.width = data["SpriteTextureWidth"].asInt();
        img.height = data["SpriteTextureHeight"].asInt();
        img.texture_index = data["SpriteTextureIndex"].asInt();
        auto system = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        auto image = system->GetImage(img.mFileName);
        if (image != nullptr) {
            img.texture_index = system->GetImage(img.mFileName)->texture_index;
            img.texture_id = system->GetImage(img.mFileName)->texture_id;
        }
        

        animation_index = data["SpriteCurrentTexIndex"].asInt();

        // Deserializing imgui-related properties
        name = data["SpriteName"].asString();
        color.x = data["SpriteColorX"].asFloat();
        color.y = data["SpriteColorY"].asFloat();
        color.z = data["SpriteColorZ"].asFloat();
        color.a = data["SpriteColorA"].asFloat();

        animation_index = data["AnimationIndex"].asInt();
        layer = data["Layer"].asInt();

        // Deserializing the vector of animations
        anims.clear(); // Clear existing animations
        Json::Value animationsJson = data["animations"];
        for (const Json::Value& animationData : animationsJson) {
            Animation animation;

            animation.name = animationData["Name"].asString();
            animation.frame_dimension.x = animationData["frame_dimension_x"].asFloat();
            animation.frame_dimension.y = animationData["frame_dimension_y"].asFloat();
            animation.frame_index.x = animationData["frame_index_x"].asFloat();
            animation.frame_index.y = animationData["frame_index_y"].asFloat();
            animation.x_frames = animationData["x_frames"].asInt();
            animation.y_frames = animationData["y_frames"].asInt();
            animation.animation_duration = animationData["animation_duration"].asFloat();
            animation.time_per_frame = animationData["time_per_frame"].asFloat();
            animation.frame_timer = animationData["frame_timer"].asFloat();

            anims.push_back(animation);
        }

    }


    /// BELOW FUNCTIONS ARE UNUSED AFTER ADDING INSTANCING ///
    //void Sprite::drawSprite(const Mesh& mesh_used, Shader shader, GLuint texture_ID) {
    //    // use sprite shader
    //    shader.use();

    //    // bind vao
    //    glBindVertexArray(mesh_used.vao_ID);

    //    // set uniforms
    //    shader.setUniform("uColor", color);
    //    shader.setUniform("uModel_to_NDC", ISMtx33ToGlmMat3(model_TRS.mdl_to_ndc_xform));

    //    // Bind the texture to the uniform sampler2D
    //    if (glIsTexture(texture_ID)) {
    //        GLuint textureUniformLoc = glGetUniformLocation(shader.getHandle(), "uTex2d");
    //        if (textureUniformLoc != -1) {
    //            glUniform1i(textureUniformLoc, 0); // Bind to texture unit 0
    //            glActiveTexture(GL_TEXTURE0);
    //            glBindTexture(GL_TEXTURE_2D, texture_ID);
    //        }
    //        shader.setUniform("uTexture", 1);
    //    }
    //    else shader.setUniform("uTexture", 0);

    //    // set animation uniforms to default value
    //    shader.setUniform("uFrameDim", glm::vec2(1.f, 1.f));
    //    shader.setUniform("uFrameIndex", glm::vec2(0.f, 0.f));

    //    // for each mesh type
    //    switch (primitive_type) {
    //    case GL_TRIANGLE_STRIP:
    //        glDrawArrays(primitive_type, 0, mesh_used.draw_count);
    //        break;
    //    case GL_POINTS:
    //        glPointSize(8.f);
    //        glDrawArrays(primitive_type, 0, mesh_used.draw_count);
    //        glPointSize(1.f);
    //        break;
    //    case GL_LINES:
    //        //glLineWidth(3.f);
    //        //glDrawArrays(primitive_type, 0, mesh_used.draw_count);
    //        //glLineWidth(1.f);
    //        break;
    //    case GL_TRIANGLE_FAN:
    //        glDrawArrays(primitive_type, 0, mesh_used.draw_count);
    //        break;
    //    }

    //    // unbind texture and vao
    //    glBindTexture(GL_TEXTURE_2D, 0);
    //    glBindVertexArray(0);
    //    shader.unUse();
    //}

    //void Sprite::drawLine(Vector2D const& p0, Vector2D const& p1, std::tuple<float, float, float> const& clr, float thickness) {
    //    // ensure line mesh is initialized
    //    if (ISGraphics::meshes.empty()) {
    //        std::cerr << "No mesh to draw line!" << std::endl;
    //        return;
    //    }
    //    Mesh mesh_used = ISGraphics::meshes[2];
    //    float delta_x = p1.x - p0.x;
    //    float delta_y = p1.y - p0.y;

    //    // Translation
    //    Vector2D midpoint = (p0 + p1) / 2.f;

    //    // Rotation
    //    float angle = atan2f(delta_y, delta_x);
    //    angle += PI; // atan returns [-PI, PI]

    //    // Scaling
    //    float length = ISVector2DDistance(p0, p1);
    //    Transform lineTRS(midpoint, glm::degrees(angle), { length, 0.f });

    //    // get line scaling matrix
    //    glm::mat3 world_to_NDC_xform = ISMtx33ToGlmMat3(lineTRS.ReturnXformMatrix());

    //    // render
    //    // Shader shader = ISGraphics::mesh_shader_pgm;
    //    Shader shader{};
    //    shader.use();

    //    // bind line mesh vao
    //    glBindVertexArray(mesh_used.vao_ID);

    //    // set uniforms
    //    auto& [r, g, b] = clr;
    //    glm::vec3 uniform_color{ r, g, b };
    //    shader.setUniform("uColor", uniform_color);
    //    shader.setUniform("uModel_to_NDC", world_to_NDC_xform);
    //    shader.setUniform("uTexture", 0);
    //    shader.setUniform("uFrameDim", glm::vec2(1.f, 1.f));
    //    shader.setUniform("uFrameIndex", glm::vec2(0.f, 0.f));

    //    // draw line
    //    glDrawArrays(GL_LINES, 0, mesh_used.draw_count);
    //    glLineWidth(thickness);

    //    // unbind texture and vao
    //    glBindTexture(GL_TEXTURE_2D, 0);
    //    glBindVertexArray(0);
    //    shader.unUse();
    //}

    //void Sprite::draw_instanced_quads() {
    //    // Bind the instance VBO
    //    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[0].instance_vbo_ID));
    //    // Upload the quadInstances data to the GPU
    //    GL_CALL(Sprite::instanceData* buffer = reinterpret_cast<Sprite::instanceData*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)));
    //    std::vector<Sprite::instanceData> tempData;

    //    if (buffer) {
    //        // Copy the instance data from the multiset to the vector
    //        for (const auto& data : ISGraphics::layeredQuadInstances) {
    //            tempData.push_back(data);
    //        }

    //        // Copy the instance data to the mapped buffer
    //        std::memcpy(buffer, tempData.data(), tempData.size() * sizeof(Sprite::instanceData));

    //        // Unmap the buffer
    //        if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE) {
    //            // Handle the case where unmap was not successful
    //            IS_CORE_ERROR("Failed to unmap the buffer.");
    //        }
    //    }
    //    else {
    //        // Handle the case where mapping the buffer was not successful
    //        IS_CORE_ERROR("Failed to map the buffer for writing.");
    //    }
    //    
    //    // bind shader
    //    GL_CALL(glUseProgram(ISGraphics::inst_quad_shader_pgm.getHandle()));
    //    GL_CALL(glBindVertexArray(ISGraphics::meshes[0].vao_ID)); // will change to enums

    //    // store texture array indices
    //    std::vector<int> tex_array_index_vect;
    //    for (auto const& texture : ISGraphics::textures) {
    //        GL_CALL(glBindTextureUnit(texture.texture_index, texture.texture_id));
    //        tex_array_index_vect.emplace_back(texture.texture_index);
    //    }

    //    // upload to uniform variable
    //    GL_CALL(auto tex_arr_uniform = glGetUniformLocation(ISGraphics::inst_quad_shader_pgm.getHandle(), "uTex2d"));
    //    if (tex_arr_uniform >= 0)
    //    {
    //        GL_CALL(glUniform1iv(tex_arr_uniform, static_cast<int>(tex_array_index_vect.size()), &tex_array_index_vect[0]));
    //    }
    //    else
    //    {
    //        IS_CORE_ERROR("uTex2d Uniform not found");
    //    }

    //    // draw instanced quads
    //    GL_CALL(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, ISGraphics::meshes[0].draw_count, static_cast<GLsizei>(tempData.size())));
    //}
}