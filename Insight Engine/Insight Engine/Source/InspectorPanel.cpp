/*!
 * \file SceneHierarchyPanel.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for class SceneHierarchyPanel,
 * which encapsulates the functionalities of a scene hierarchy panel
 * akin to other game engines (i.e., Unity/Unreal Engine, etc.), which
 * manages the entities in a scene.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "InspectorPanel.h"
#include "EditorUtils.h"
#include "EditorLayer.h"
#include "FileUtils.h"
#include "Editor.h"

// Dependencies
#include <imgui.h>

namespace IS {

    class SceneHierarchyPanel;

    InspectorPanel::InspectorPanel(std::shared_ptr<SceneHierarchyPanel> scene_hierarchy_panel)
        : mSceneHierarchyPanel(scene_hierarchy_panel) {}

    void InspectorPanel::RenderPanel()
    {
        ImGui::Begin("Inspector");
        if (mSceneHierarchyPanel->mSelectedEntity)
        {
            RenderComponentNodes(*mSceneHierarchyPanel->mSelectedEntity);
            RenderOutline(*mSceneHierarchyPanel->mSelectedEntity);
        }
        ImGui::End(); // end window Inspector
    }

    void InspectorPanel::RenderEntityConfig(Entity entity)
    {
        InsightEngine& engine = InsightEngine::Instance();

        // Edit Entity Name
        std::string& name = engine.GetEntityName(entity);
        char buffer[256]{};
        auto source = name | std::ranges::views::take(name.size());
        std::ranges::copy(source, std::begin(buffer));

        ImGuiIO& io = ImGui::GetIO();
        auto FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;

        if (ImGui::InputText("##Name", buffer, sizeof(buffer), input_text_flags))
            name = std::string(buffer);

        // Prefabs
        ImGui::TextUnformatted("Prefab");

        // Save Prefab
        ImGui::SameLine();
        ImGui::PushFont(FONT_BOLD);
        if (ImGui::Button("Save"))
            engine.SaveAsPrefab(entity, name);

        // Load Prefab
        ImGui::SameLine();
        if (ImGui::Button("Load"))
            mShowPrefabs = true;

        // Render Prefab Combo
        ImGui::PopFont();
        if (mShowPrefabs)
        {
            ImGui::SameLine();
            ImGui::PushItemWidth(100.f);
            bool begin_combo = ImGui::BeginCombo("##Prefabs", name.c_str());
            if (begin_combo)
            {
                auto asset = engine.GetSystem<AssetManager>("Asset");
                for (auto const& [prefab_name, prefab] : asset->mPrefabList)
                {
                    const bool is_selected = (name == prefab_name);
                    if (ImGui::Selectable(prefab_name.c_str(), is_selected))
                    {
                        engine.LoadFromPrefab(prefab);
                        mShowPrefabs = false;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo(); // end combo Prefabs
            }
            ImGui::PopItemWidth();

            if (ImGui::IsAnyMouseDown() && !ImGui::IsItemHovered() && !begin_combo)
                mShowPrefabs = false;
        }

        // Add Component
        ImGui::PushFont(FONT_BOLD);
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");
        ImGui::PopFont();

        ImGui::SameLine();

        // Check whether entity already has the component
        if (ImGui::BeginPopup("AddComponent"))
        {
            SceneHierarchyPanel::RenderAddComponent(entity);
            ImGui::EndPopup();
        }

        // Clone Entity
        ImGui::PushFont(FONT_BOLD);
        if (ImGui::Button("Clone Entity"))
            engine.CopyEntity(entity);
        ImGui::SameLine();

        // Destroy Entity
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.77f, .16f, .04f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.84f, .31f, .25f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.77f, .16f, .04f, 1.f));
        if (ImGui::Button("Destroy Entity"))
        {
            engine.DeleteEntity(entity);
            mSceneHierarchyPanel->ResetSelection();
        }
        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::Spacing();

    } // end RenderEntityConfig()

    void InspectorPanel::RenderOutline(Entity entity)
    {
        auto& engine = InsightEngine::Instance();
        if (engine.HasComponent<Sprite>(entity) && engine.HasComponent<RigidBody>(entity))
        {
            auto& sprite = engine.GetComponent<Sprite>(entity);
            auto& body = engine.GetComponent<RigidBody>(entity);
            ISGraphics::DrawOutLine(body, sprite, { 1.f, 1.f, 0.f }, 3.f);
        }
    }

    void InspectorPanel::RenderComponentNodes(Entity entity)
    {
        ImGui::PushID(entity);

        // Make everything rounded
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
        ImGuiIO& io = ImGui::GetIO();
        auto FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];

        // Entity configurations
        RenderEntityConfig(entity);

        // Sprite Component
        RenderComponent<Sprite>("Sprite", entity, [FONT_BOLD, entity](Sprite& sprite)
        {
            auto& engine = InsightEngine::Instance();
            auto const editor = engine.GetSystem<Editor>("Editor");
            auto const editor_layer = editor->GetEditorLayer();

            // Render color
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Color");
            ImGui::PopFont();
            ImGui::ColorEdit3("##Color", &sprite.color[0]);
            ImGui::Spacing();

            // Render Texture and its dimension data
            // Header
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Texture");
            ImGui::PopFont();

            // Parameters
            ImTextureID texture_id;
            float texture_width;
            float texture_height;

            // Use placeholder if no texture attached
            if (sprite.img.texture_id)
            {
                texture_id = EditorUtils::ConvertTextureID(sprite.img.texture_id);
                texture_width = static_cast<float>(sprite.img.width);
                texture_height = static_cast<float>(sprite.img.height);
            }
            else
            {
                texture_id = editor_layer->GetIcon("TexturePlaceholder");
                texture_width = texture_height = 512.f;
            }

            const float texture_aspect_ratio = texture_width / texture_height;
            const float draw_size = 40.f;
            ImGuiIO& io = ImGui::GetIO();
            ImVec2 pos = ImGui::GetCursorPos();
            ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
            ImVec4 tint_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            ImVec4 border_color = ImGui::GetStyleColorVec4(ImGuiCol_Border);

            // Render Texture as Image
            ImGui::BeginGroup();
            ImGui::Image(texture_id, ImVec2(draw_size * texture_aspect_ratio, draw_size), uv_min, uv_max, tint_color, border_color);

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
                {
                    std::filesystem::path path = static_cast<wchar_t*>(payload->Data);
                    auto asset = engine.GetSystem<AssetManager>("Asset");
                    IS_CORE_DEBUG("Image : {} ", path.string());
                    sprite.img = *asset->GetImage(path.string());
                }
                ImGui::EndDragDropTarget();
            }
            ImGui::EndGroup();

            // Texture Tooltip
            if (ImGui::BeginItemTooltip())
            {
                float region_size = draw_size;
                float region_x = io.MousePos.x - pos.x - region_size * 0.5f;
                float region_y = io.MousePos.y - pos.y - region_size * 0.5f;
                float zoom = 4.f;
                if (region_x < 0.0f) { region_x = 0.0f; } else if (region_x > texture_width - region_size) { region_x = texture_width - region_size; }
                if (region_y < 0.0f) { region_y = 0.0f; } else if (region_y > texture_height - region_size) { region_y = texture_height - region_size; }
                ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                ImGui::Text("Max: (%.2f, %.2f)", region_x + region_size, region_y + region_size);
                ImVec2 uv0 = ImVec2((region_x) / texture_width, (region_y) / texture_height);
                ImVec2 uv1 = ImVec2((region_x + region_size) / texture_width, (region_y + region_size) / texture_height);
                float zoomed_in_height = region_size * zoom;
                float zoomed_in_width = zoomed_in_height * texture_aspect_ratio;
                ImGui::Image(texture_id, ImVec2(zoomed_in_width, zoomed_in_height)); // still WIP
                ImGui::EndTooltip();
            }

            // Texture Dimensions
            ImGuiTableFlags table_flags = 0;
            ImGui::BeginTable("Texture", 2, table_flags);
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Width");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::Text("%dpx", sprite.img.width);
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Height");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::Text("%dpx", sprite.img.height);
            ImGui::EndTable();

            //// Render Animation stuff
            //if (!sprite.anims.empty())
            //{
            //    ImGui::PushFont(FONT_BOLD);
            //    ImGui::TextUnformatted("Animation");
            //    ImGui::PopFont();

            //    for (Animation& animation : sprite.anims)
            //    {
            //        if (ImGui::BeginTable("Animation", 2))
            //        {
            //            ImGui::TableNextColumn();
            //            ImGui::PushFont(FONT_BOLD);
            //            ImGui::TextUnformatted("Columns");
            //            ImGui::PopFont();
            //            ImGui::TableNextColumn();
            //            ImGui::SliderInt("##Columns", &animation.x_frames, 1, 20);

            //            ImGui::EndTable();
            //        }
            //    }
            //}

        }); // end render Sprite Component

        // Transform Component
        RenderComponent<Transform>("Transform", entity, [FONT_BOLD](Transform& transform)
        {
            // Render Translation
            Vector2D position = { transform.world_position.x, transform.world_position.y };
            EditorUtils::RenderControlVec2("Translation", position);

            // Render Rotation
            ImGuiTableFlags table_flags = ImGuiTableFlags_PreciseWidths;
            if (ImGui::BeginTable("TransformRotation", 2, table_flags))
            {
                ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
                ImGui::TableSetupColumn("TransformRotation", column_flags, 100.f);
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Rotation");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                float rotation = transform.rotation * (PI / 180.f);
                ImGui::SliderAngle("##Rotation", &rotation, 0.f);
                transform.rotation = rotation / (PI / 180.f);
                ImGui::EndTable();
            }

            // Render Scale
            Vector2D scale = { transform.scaling.x, transform.scaling.y };
            EditorUtils::RenderControlVec2("Scale", scale, 95.f, 120.f);

            transform.setScaling(scale.x, scale.y);
            transform.setWorldPosition(position.x, position.y);

        }); // end render Transform Component

        // Rigidbody Component
        RenderComponent<RigidBody>("Rigidbody", entity, [entity, FONT_BOLD](RigidBody& rigidbody)
        {
            ImGuiTableFlags table_flags = ImGuiTableFlags_PreciseWidths;

            EditorUtils::RenderControlVec2("Velocity", rigidbody.mVelocity);
            EditorUtils::RenderControlVec2("Force", rigidbody.mForce);

            // Render Other Component Attributes
            if (ImGui::BeginTable(("RigidbodyTable" + std::to_string(entity)).c_str(), 2, table_flags))
            {
                // Set Table and its flags
                ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
                ImGui::TableSetupColumn("RigidbodyTable", column_flags, 100.f);

                // Angular Velocity
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Angular Velocity");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::DragFloat("##AngularVelocity", &rigidbody.mAngularVelocity, 1.f, 0.f, 0.f, "%.2f");
                ImGui::PopItemWidth();

                // Body Type
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Body Type");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                EditorUtils::RenderComboBoxEnum<BodyType>("##Body Type", rigidbody.mBodyType, { "Static", "Dynamic", "Kinematic" });

                // Body Shape
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Body Shape");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                EditorUtils::RenderComboBoxEnum<BodyShape>("##Body Shape", rigidbody.mBodyShape, { "Box", "Circle", "Line" });

                // Mass
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Mass");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::DragFloat("##Mass", &rigidbody.mMass, 1.f, 0.f, 0.f, "%.2f");
                ImGui::PopItemWidth();

                // Inverse Mass
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Inverse Mass");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::DragFloat("##InvMass", &rigidbody.mInvMass, 1.f, 0.f, 0.f, "%.2f");
                ImGui::PopItemWidth();

                // Inertia
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Inertia");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::DragFloat("##Inertia", &rigidbody.mInertia, 1.f, 0.f, 0.f, "%.2f");
                ImGui::PopItemWidth();

                // Restitution
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Restitution");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::Text("%.2f", rigidbody.mRestitution);
                ImGui::PopItemWidth();

                // Density
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Density");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::Text("%.2f", rigidbody.mDensity);
                ImGui::PopItemWidth();

                ImGui::EndTable(); // end table RigidbodyTable
            }
        }); // end render Rigidbody Component

        // Script Component
        RenderComponent<ScriptComponent>("Script", entity, [FONT_BOLD](ScriptComponent& script)
        {
            // Display Sprite Name
            if (ImGui::BeginTable("ScriptTable", 2))
            {
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Script Name:");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::Text("%s", script.mScriptName.c_str());
                ImGui::EndTable();
            }

            // Modify Script
            bool modify_button = ImGui::Button("Modify");
            if (ImGui::BeginItemTooltip())
            {
                ImGui::SetTooltip("Browse and replace the existing script");
                ImGui::EndTooltip();
            }

            // Clicking the modfiy button
            if (modify_button)
            {
                if (std::filesystem::path filepath(FileUtils::OpenAndGetScript()); !filepath.empty())
                    script.mScriptName = filepath.stem().string();
            }

            ImGui::SameLine();

            // Edit Script
            bool edit_button = ImGui::Button("Edit");
            if (ImGui::BeginItemTooltip())
            {
                ImGui::SetTooltip("Open and edit script in default application");
                ImGui::EndTooltip();
            }

            // Clicking the edit button
            if (edit_button)
                InsightEngine::Instance().OpenGameScript(script.mScriptName + ".cs");

        }); // end render Script Component

        ImGui::PopStyleVar(); // end style rounding
        ImGui::PopID();

    } // end RenderComponentNodes()

    template <typename Component, typename RenderFunc>
    void InspectorPanel::RenderComponent(std::string const& label, Entity entity, RenderFunc render)
    {
        // Engine instance
        InsightEngine& engine = InsightEngine::Instance();
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        // Render Component Tree
        if (engine.HasComponent<Component>(entity))
        {
            bool opened = ImGui::TreeNodeEx(std::bit_cast<void*>(typeid(Component).hash_code()), tree_flags, label.c_str());

            // Display Component Config
            ImGui::SameLine(ImGui::GetWindowWidth() - 20.f);

            if (ImGui::Button("..."))
                ImGui::OpenPopup("ComponentConfig");

            // Set Remove Component
            bool remove_component = false;
            if (ImGui::BeginPopup("ComponentConfig"))
            {
                if (ImGui::MenuItem("Remove Component"))
                    remove_component = true;
                ImGui::EndPopup();
            }

            // Render Component (unique for each component)
            if (opened)
            {
                auto& component = engine.GetComponent<Component>(entity);
                render(component);

                ImGui::TreePop();
            }

            // Remove Component
            if (remove_component)
                engine.RemoveComponent<Component>(entity);
        }

    } // end RenderComponent()

} // end namespace IS
