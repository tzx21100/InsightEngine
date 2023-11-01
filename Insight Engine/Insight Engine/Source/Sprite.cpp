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
    int Sprite::texture_count = 0;

    void Sprite::transform() {
        model_TRS.mdl_to_ndc_xform = model_TRS.ReturnXformMatrix();
    }

    void Sprite::drawSprite(const Mesh& mesh_used, Shader shader, GLuint texture_ID) {
        // use sprite shader
        shader.use();

        // bind vao
        glBindVertexArray(mesh_used.vao_ID);

        // set uniforms
        shader.setUniform("uColor", color);
        shader.setUniform("uModel_to_NDC", ISMtx33ToGlmMat3(model_TRS.mdl_to_ndc_xform));

        // Bind the texture to the uniform sampler2D
        if (glIsTexture(texture_ID)) {
            GLuint textureUniformLoc = glGetUniformLocation(shader.getHandle(), "uTex2d");
            if (textureUniformLoc != -1) {
                glUniform1i(textureUniformLoc, 0); // Bind to texture unit 0
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_ID);
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
            //glLineWidth(3.f);
            //glDrawArrays(primitive_type, 0, mesh_used.draw_count);
            //glLineWidth(1.f);
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

    void Sprite::addAnimation(Animation const& anim) {
        anims.emplace_back(anim);
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

    //void Sprite::drawLine(Vector2D const& p0, Vector2D const& p1, std::tuple<float, float, float> const& color, float thickness) {
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
    //    Shader shader = ISGraphics::mesh_shader_pgm;
    //    shader.use();

    //    // bind line mesh vao
    //    glBindVertexArray(mesh_used.vao_ID);

    //    // set uniforms
    //    auto [r, g, b] = color;
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

    void Sprite::draw_instanced_quads() {
        // Bind the instance VBO
        glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[4].instance_vbo_ID);
        // Upload the quadInstances data to the GPU
        Sprite::instanceData* buffer = reinterpret_cast<Sprite::instanceData*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
        std::vector<Sprite::instanceData> tempData;

        if (buffer) {
            // Copy the instance data from the multiset to the vector
            for (const auto& data : ISGraphics::layeredQuadInstances) {
                tempData.push_back(data);
            }

            // Copy the instance data to the mapped buffer
            std::memcpy(buffer, tempData.data(), tempData.size() * sizeof(Sprite::instanceData));

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

        glUseProgram(ISGraphics::inst_quad_shader_pgm.getHandle());
        glBindVertexArray(ISGraphics::meshes[4].vao_ID);


        std::vector<int> tex_array_index_vect;
        for (auto const& texture : ISGraphics::textures) {
            glBindTextureUnit(texture.texture_index, texture.texture_id);
            tex_array_index_vect.emplace_back(texture.texture_index);
        }

        auto tex_arr_uniform = glGetUniformLocation(ISGraphics::inst_quad_shader_pgm.getHandle(), "uTex2d");
        if (tex_arr_uniform >= 0)
            glUniform1iv(tex_arr_uniform, static_cast<int>(tex_array_index_vect.size()), &tex_array_index_vect[0]);
        else
            std::cout << "uTex2d Uniform not found" << std::endl;


        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, ISGraphics::meshes[4].draw_count, static_cast<GLsizei>(tempData.size()));
    }

    //glm::mat3 Sprite::lineTransform(Vector2D const& midpoint_translate, float rotate_angle_rad, float length_scale) {
    //    // similar to transform
    //    auto [width, height] = InsightEngine::Instance().GetWindowSize();
    //    float map_scale_x = 2.f / width;
    //    float map_scale_y = 2.f / height;

    //    float sin_angle = sinf(rotate_angle_rad);
    //    float cos_angle = cosf(rotate_angle_rad);

    //    float model_scale_x = length_scale / 2.f;
    //    float model_scale_y = 1.f; // no Y-scaling for line

    //    glm::mat3 world_to_NDC_xform = { (map_scale_x * model_scale_x * cos_angle),  (map_scale_y * model_scale_x * sin_angle),  0.f,   // column 1
    //                                     (map_scale_x * model_scale_y * -sin_angle), (map_scale_y * model_scale_y * cos_angle),  0.f,   // column 2
    //                                     (map_scale_x * midpoint_translate.x),       (map_scale_y * midpoint_translate.y),       1.f }; // column 3


    //    return world_to_NDC_xform;
    //}

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
        spriteData["SpriteTexture"] = img.texture_id;
        spriteData["SpriteTextureWidth"] = img.width;
        spriteData["SpriteTextureHeight"] = img.height;
        spriteData["SpriteCurrentTexIndex"] = animation_index;
        spriteData["SpriteTextureIndex"] = img.texture_index;

        // Serializing imgui-related properties
        spriteData["SpriteName"] = name;
        spriteData["SpriteColorX"] = color.x;
        spriteData["SpriteColorY"] = color.y;
        spriteData["SpriteColorZ"] = color.z;
        spriteData["SpriteDrawing"] = drawing;

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
        img.texture_id = static_cast<uint8_t>(data["SpriteTexture"].asUInt());
        img.width = data["SpriteTextureWidth"].asInt();
        img.height = data["SpriteTextureHeight"].asInt();
        img.texture_index = data["SpriteTextureIndex"].asInt();
        animation_index = data["SpriteCurrentTexIndex"].asInt();

        // Deserializing imgui-related properties
        name = data["SpriteName"].asString();
        color.x = data["SpriteColorX"].asFloat();
        color.y = data["SpriteColorY"].asFloat();
        color.z = data["SpriteColorZ"].asFloat();
        drawing = data["SpriteDrawing"].asBool();

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
        glm::mat3 world_to_NDC_xform = ISMtx33ToGlmMat3(lineTRS.ReturnXformMatrix());
        Sprite::nonQuadInstanceData lineData;
        lineData.color = glm::vec3(std::get<0>(color), std::get<1>(color), std::get<2>(color));
        lineData.model_to_ndc_xform = world_to_NDC_xform;

        ISGraphics::lineInstances.emplace_back(lineData);
    }

    void Sprite::draw_instanced_lines() {
        // Bind the instance VBO
        glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[5].instance_vbo_ID);
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

        glUseProgram(ISGraphics::inst_non_quad_shader_pgm.getHandle());
        glBindVertexArray(ISGraphics::meshes[5].vao_ID);

        glDrawArraysInstanced(GL_LINES, 0, ISGraphics::meshes[5].draw_count, static_cast<GLsizei>(ISGraphics::lineInstances.size()));
        ISGraphics::lineInstances.clear();
    }

    void Sprite::drawDebugCircle(Vector2D const& worldPos, Vector2D const& scale, std::tuple<float, float, float> const& color) {
        Transform CircleTRS(worldPos, 0.f, scale);
        glm::mat3 world_to_NDC_xform = ISMtx33ToGlmMat3(CircleTRS.ReturnXformMatrix());

        Sprite::nonQuadInstanceData circleData;
        circleData.color = glm::vec3(std::get<0>(color), std::get<1>(color), std::get<2>(color));
        circleData.model_to_ndc_xform = world_to_NDC_xform;

        ISGraphics::circleInstances.emplace_back(circleData);
    }

    void Sprite::draw_instanced_circles() {
        // Bind the instance VBO
        glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[6].instance_vbo_ID);
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

        glUseProgram(ISGraphics::inst_non_quad_shader_pgm.getHandle());
        glBindVertexArray(ISGraphics::meshes[6].vao_ID);

        glDrawArraysInstanced(GL_LINE_LOOP, 0, ISGraphics::meshes[6].draw_count, static_cast<GLsizei>(ISGraphics::circleInstances.size()));
        ISGraphics::circleInstances.clear();
    }
}