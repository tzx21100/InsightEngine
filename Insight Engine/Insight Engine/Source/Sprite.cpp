/*!
 * \file Sprite.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
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
    // set of layers to NOT render
    std::unordered_set<int> Sprite::layersToIgnore{};

    Sprite::Sprite() : name("Quad"), primitive_type(GL_TRIANGLE_STRIP), color(.6f, .6f, .6f, 1.f) {
        // Give it a default size of 1 by 1
        setSpriteSize(1, 1);
        setWorldPos(0, 0);
    }

    Sprite::Sprite(std::string const& model_name, GLenum primitive) : name(model_name), primitive_type(primitive), color(.6f, .6f, .6f, 1.f) {}

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
        model_TRS.mdl_to_ndc_xform = model_TRS.ReturnXformMatrix();
        model_TRS.mdl_to_3dcam_to_ndc_xform = model_TRS.Return3DXformMatrix();
    }

    void Sprite::draw_instanced_quads() {
        // Bind the instance VBO
        glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[0].instance_vbo_ID);
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
        
        // bind shader
        glUseProgram(ISGraphics::inst_quad_shader_pgm.getHandle());
        glBindVertexArray(ISGraphics::meshes[0].vao_ID); // will change to enums

        // store texture array indices
        std::vector<int> tex_array_index_vect;
        for (auto const& texture : ISGraphics::textures) {
            glBindTextureUnit(texture.texture_index, texture.texture_id);
            tex_array_index_vect.emplace_back(texture.texture_index);
        }

        // upload to uniform variable
        auto tex_arr_uniform = glGetUniformLocation(ISGraphics::inst_quad_shader_pgm.getHandle(), "uTex2d");
        if (tex_arr_uniform >= 0) 
            glUniform1iv(tex_arr_uniform, static_cast<int>(tex_array_index_vect.size()), &tex_array_index_vect[0]);
        else std::cout << "uTex2d Uniform not found" << std::endl;

        // draw instanced quads
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, ISGraphics::meshes[0].draw_count, static_cast<GLsizei>(tempData.size()));
    }

    void Sprite::draw_instanced_3D_quads() {
        // Bind the instance VBO
        glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[3].instance_vbo_ID);
        // Upload the quadInstances data to the GPU
        Sprite::instanceData3D* buffer = reinterpret_cast<Sprite::instanceData3D*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
        std::vector<Sprite::instanceData3D> tempData;

        if (buffer) {
            // Copy the instance data from the multiset to the vector
            for (const auto& data : ISGraphics::layered3DQuadInstances) {
                tempData.push_back(data);
            }

            // Copy the instance data to the mapped buffer
            std::memcpy(buffer, tempData.data(), tempData.size() * sizeof(Sprite::instanceData3D));

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
        glUseProgram(ISGraphics::inst_3d_quad_shader_pgm.getHandle());
        glBindVertexArray(ISGraphics::meshes[3].vao_ID); // will change to enums

        // store texture array indices
        std::vector<int> tex_array_index_vect;
        for (auto const& texture : ISGraphics::textures) {
            glBindTextureUnit(texture.texture_index, texture.texture_id);
            tex_array_index_vect.emplace_back(texture.texture_index);
        }

        // upload to uniform variable
        auto tex_arr_uniform = glGetUniformLocation(ISGraphics::inst_3d_quad_shader_pgm.getHandle(), "uTex2d");
        if (tex_arr_uniform >= 0)
            glUniform1iv(tex_arr_uniform, static_cast<int>(tex_array_index_vect.size()), &tex_array_index_vect[0]);
        else std::cout << "uTex2d Uniform not found" << std::endl;

        // draw instanced quads
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, ISGraphics::meshes[3].draw_count, static_cast<GLsizei>(tempData.size()));
    }

    // draw light quad here similar to old non-layered instance quad drawing
    

    void Sprite::draw_colored_quad(Vector2D const& pos, float rotation, Vector2D const& scale, Vector4D const& color, int layer) {
        if (Sprite::layersToIgnore.find(layer) == Sprite::layersToIgnore.end()) {
            Transform quadTRS(pos, rotation, scale);

            // get line scaling matrix
            glm::mat4 world_to_NDC_xform = quadTRS.Return3DXformMatrix();
            Sprite::instanceData3D instData;
            instData.color = glm::vec4(color.x, color.y, color.z, color.w);
            instData.model_to_ndc_xform = world_to_NDC_xform;
            instData.layer = layer;

            ISGraphics::layered3DQuadInstances.insert(instData);
        }
    }

    void Sprite::draw_textured_quad(Vector2D const& pos, float rotation, Vector2D const& scale, Image const& texture, int layer) {
        if (Sprite::layersToIgnore.find(layer) == Sprite::layersToIgnore.end()) {
            Transform quadTRS(pos, rotation, scale);

            // get line scaling matrix
            glm::mat4 world_to_NDC_xform = quadTRS.Return3DXformMatrix();
            Sprite::instanceData3D instData;
            instData.tex_index = static_cast<float>(texture.texture_index);
            instData.model_to_ndc_xform = world_to_NDC_xform;
            instData.layer = layer;

            ISGraphics::layered3DQuadInstances.insert(instData);
        }
    }

    void Sprite::draw_lights() {
        // Bind the instance VBO
        glBindBuffer(GL_ARRAY_BUFFER, ISGraphics::meshes[3].instance_vbo_ID);
        // Upload the quadInstances data to the GPU
        Sprite::instanceData3D* buffer = reinterpret_cast<Sprite::instanceData3D*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

        if (buffer) {
            // Copy the instance data to the mapped buffer
            std::memcpy(buffer, ISGraphics::lightInstances.data(), ISGraphics::lightInstances.size() * sizeof(Sprite::instanceData3D));

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
        glUseProgram(ISGraphics::light_shader_pgm.getHandle());
        glBindVertexArray(ISGraphics::meshes[3].vao_ID); // will change to enums

        // draw instanced quads

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, ISGraphics::meshes[3].draw_count, static_cast<GLsizei>(ISGraphics::lightInstances.size()));
        ISGraphics::lightInstances.clear();
    }

    void Sprite::draw_picked_entity_border() {
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
        glUseProgram(ISGraphics::inst_non_quad_shader_pgm.getHandle());
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
        glUseProgram(ISGraphics::inst_non_quad_shader_pgm.getHandle());
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

        IS_CORE_DEBUG(" SAVED FILENAME {}", img.mFileName);

        // Serializing imgui-related properties
        spriteData["SpriteName"] = name;
        spriteData["SpriteColorX"] = color.x;
        spriteData["SpriteColorY"] = color.y;
        spriteData["SpriteColorZ"] = color.z;

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
        }

        animation_index = data["SpriteCurrentTexIndex"].asInt();

        // Deserializing imgui-related properties
        name = data["SpriteName"].asString();
        color.x = data["SpriteColorX"].asFloat();
        color.y = data["SpriteColorY"].asFloat();
        color.z = data["SpriteColorZ"].asFloat();

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

    void Sprite::drawLine(Vector2D const& p0, Vector2D const& p1, std::tuple<float, float, float> const& clr, float thickness) {
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
        Transform lineTRS(midpoint, glm::degrees(angle), { length, 0.f });

        // get line scaling matrix
        glm::mat3 world_to_NDC_xform = ISMtx33ToGlmMat3(lineTRS.ReturnXformMatrix());

        // render
        // Shader shader = ISGraphics::mesh_shader_pgm;
        Shader shader{};
        shader.use();

        // bind line mesh vao
        glBindVertexArray(mesh_used.vao_ID);

        // set uniforms
        auto& [r, g, b] = clr;
        glm::vec3 uniform_color{ r, g, b };
        shader.setUniform("uColor", uniform_color);
        shader.setUniform("uModel_to_NDC", world_to_NDC_xform);
        shader.setUniform("uTexture", 0);
        shader.setUniform("uFrameDim", glm::vec2(1.f, 1.f));
        shader.setUniform("uFrameIndex", glm::vec2(0.f, 0.f));

        // draw line
        glDrawArrays(GL_LINES, 0, mesh_used.draw_count);
        glLineWidth(thickness);

        // unbind texture and vao
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader.unUse();
    }
}