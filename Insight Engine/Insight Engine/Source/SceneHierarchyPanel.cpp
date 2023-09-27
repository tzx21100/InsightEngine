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
#include "SceneHierarchyPanel.h"
#include "Guidgets.h"
#include "CoreEngine.h"

// Dependencies
#include <imgui.h>

namespace IS {

    void SceneHierarchyPanel::RenderPanel() {
        InsightEngine& engine = InsightEngine::Instance();

        // Begin creating the scene hierarchy panel
        ImGui::Begin("Hierarchy");

        // Render all entity nodes
        for (auto& [entity, name] : engine.GetEntitiesAlive())
            RenderEntityNode(entity);

        // Deselect entity
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            mSelectedEntity = {};

        ImGuiPopupFlags flags = ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight;
        if (ImGui::BeginPopupContextWindow(0, flags)) {
            // Create Empty Entity
            if (ImGui::MenuItem("Create Empty Entity"))
                engine.CreateEntity("Entity");

            // Create Random Entity
            if (ImGui::MenuItem("Create Random Entity"))
                engine.GenerateRandomEntity();
            ImGui::EndPopup();
        }

        ImGui::End();

        // Render inspector for selected entity
        ImGui::Begin("Inspector");
        if (mSelectedEntity)
            RenderComponentNodes(*mSelectedEntity);
        ImGui::End();
    }

    void SceneHierarchyPanel::RenderEntityNode(Entity entity) {
        InsightEngine& engine = InsightEngine::Instance();

        ImGuiTreeNodeFlags tree_flags = (mSelectedEntity && (*mSelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0);
        tree_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((engine.GetEntityName(entity) + ' ' + std::to_string(entity)).c_str(), tree_flags);

        if (ImGui::IsItemClicked())
            mSelectedEntity = std::make_shared<Entity>(entity);

        if (ImGui::BeginPopupContextItem()) {
            // Clone Entity
            if (ImGui::MenuItem("Clone Entity"))
                engine.CopyEntity(entity);

            // Delete Entity
            if (ImGui::MenuItem("Delete Entity")) {
                engine.DeleteEntity(entity);
                if (mSelectedEntity && *mSelectedEntity == entity)
                    mSelectedEntity = {};
            }

            ImGui::EndPopup();
        }

        if (opened)
            ImGui::TreePop();
    }

    void SceneHierarchyPanel::RenderComponentNodes(Entity entity) {
        // Make everything rounded
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
        ImGuiIO& io = ImGui::GetIO();
        auto font_bold = io.Fonts->Fonts[0];

        // Entity configurations
        RenderEntityConfig(entity);

        // Sprite Component
        RenderComponent<Sprite>("Sprite", entity, [&](Sprite& sprite) {
            if (sprite.texture) {
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Texture");
                ImGui::PopFont();
                ImTextureID texture_id = std::bit_cast<void*>(static_cast<uintptr_t>(sprite.texture));
                const float texture_width = static_cast<float>(sprite.texture_width);
                const float texture_height = static_cast<float>(sprite.texture_height);
                const float texture_aspect_ratio = texture_width / texture_height;
                const float draw_size = 40.f;
                ImGuiIO& io = ImGui::GetIO();
                ImVec2 pos = ImGui::GetCursorPos();
                ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
                ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
                ImVec4 tint_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                ImVec4 border_color = ImGui::GetStyleColorVec4(ImGuiCol_Border);
                ImGui::Image(texture_id, ImVec2(draw_size * texture_aspect_ratio, draw_size), uv_min, uv_max, tint_color, border_color);
                if (ImGui::BeginItemTooltip()) {
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

                ImGuiTableFlags table_flags = 0;
                ImGui::BeginTable("Texture", 2, table_flags);
                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Width");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::Text("%dpx", sprite.texture_width);
                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Height");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::Text("%dpx", sprite.texture_height);
                ImGui::EndTable();

            } else {
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Color");
                ImGui::PopFont();
                ImGui::SameLine();
                ImGui::ColorEdit3(("##Color" + std::to_string(entity)).c_str(), &sprite.color[0]);
            }
        });

        // Transform Component
        RenderComponent<Transform>("Transform", entity, [&](Transform& transform) {
            Vector2D position = { transform.world_position.x, transform.world_position.y };
            Vector2D scale = { transform.scaling.x, transform.scaling.y };
            guidgets::RenderControlVec2("Translation", position);
            ImGuiTableFlags table_flags = ImGuiTableFlags_PreciseWidths;
            if (ImGui::BeginTable("TransformRotation", 2, table_flags)) {
                ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
                ImGui::TableSetupColumn("TransformRotation", column_flags, 100.f);
                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Rotation");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                float rotation = transform.rotation * (PI / 180.f);
                ImGui::SliderAngle("##Rotation", &rotation, 0.f);
                transform.rotation = rotation / (PI / 180.f);
                ImGui::EndTable();
            }
            guidgets::RenderControlVec2("Scale", scale, 95.f, 120.f);
            transform.world_position = { position.x, position.y };
            transform.scaling = { scale.x, scale.y };
        });

        // Rigidbody Component
        RenderComponent<RigidBody>("Rigidbody", entity, [&](RigidBody& rigidbody) {
            ImGuiTableFlags table_flags = ImGuiTableFlags_PreciseWidths;

            guidgets::RenderControlVec2("Velocity", rigidbody.mVelocity);
            guidgets::RenderControlVec2("Force", rigidbody.mForce);

            if (ImGui::BeginTable(("RigidbodyTable" + std::to_string(entity)).c_str(), 2, table_flags)) {
                ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
                ImGui::TableSetupColumn("RigidbodyTable", column_flags, 100.f);
                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Angular Velocity");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::DragFloat(("##AngularVelocity" + std::to_string(entity)).c_str(), &rigidbody.mAngularVelocity, 1.f, 0.f, 0.f, "%.2f");
                ImGui::PopItemWidth();

                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Body Type");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                guidgets::RenderComboBoxEnum<BodyType>("##Body Type", rigidbody.mBodyType, { "Static", "Dynamic", "Kinematic" });
                ImGui::PopItemWidth();

                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Body Shape");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                guidgets::RenderComboBoxEnum<Shape>("##Body Shape", rigidbody.mBodyShape, { "Box", "Circle", "Line" });
                ImGui::PopItemWidth();

                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Mass");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::DragFloat(("##Mass" + std::to_string(entity)).c_str(), &rigidbody.mMass, 1.f, 0.f, 0.f, "%.2f");
                ImGui::PopItemWidth();

                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Invariant Mass");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::DragFloat(("##InvMass" + std::to_string(entity)).c_str(), &rigidbody.mInvMass, 1.f, 0.f, 0.f, "%.2f");
                ImGui::PopItemWidth();

                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Restitution");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::Text("%.2f", rigidbody.mRestitution);
                ImGui::PopItemWidth();

                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Density");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::Text("%.2f", rigidbody.mDensity);
                ImGui::PopItemWidth();

                ImGui::EndTable();
            }
        });

        // Input Affector Component
        RenderComponent<InputAffector>("Input Affector", entity, []([[maybe_unused]] InputAffector& input_affector) {
            ImGui::TextUnformatted("(Empty)");
        });

        ImGui::PopStyleVar();
    }

    void SceneHierarchyPanel::RenderEntityConfig(Entity entity) {
        InsightEngine& engine = InsightEngine::Instance();

        // Edit Entity Name
        std::string& name = engine.GetEntityName(entity);
        char buffer[std::numeric_limits<char8_t>::max() + 1]{};
        auto source = name | std::ranges::views::take(name.size());
        std::ranges::copy(source, std::begin(buffer));

        ImGuiIO& io = ImGui::GetIO();
        auto font_bold = io.Fonts->Fonts[0];
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;

        if (ImGui::InputText("##Name", buffer, sizeof(buffer), input_text_flags))
            name = std::string(buffer);

        // Prefabs
        ImGui::TextUnformatted("Prefab");

        // Save Prefab
        ImGui::SameLine();
        ImGui::PushFont(font_bold);
        if (ImGui::Button("Save"))
            engine.SaveAsPrefab(entity, name);

        // Load Prefab
        ImGui::SameLine();
        if (ImGui::Button("Load"))
            show_prefabs = true;

        ImGui::PopFont();
        if (show_prefabs) {
            ImGui::SameLine();
            ImGui::PushItemWidth(100.f);
            bool begin_combo = ImGui::BeginCombo("##Prefabs", name.c_str());
            if (begin_combo) {
                auto asset = engine.GetSystem<AssetManager>("Asset");
                for (auto const& [prefab_name, prefab] : asset->mPrefabList) {
                    const bool is_selected = (name == prefab_name);
                    if (ImGui::Selectable(prefab_name.c_str(), is_selected)) {
                        engine.LoadFromPrefab(prefab);
                        show_prefabs = false;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();

            if (ImGui::IsMouseDown(0) && !ImGui::IsItemHovered() && !begin_combo)
                show_prefabs = false;
        }

        // Add Component
        ImGui::PushFont(font_bold);
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");
        ImGui::PopFont();

        // Check whether entity already has the component
        if (ImGui::BeginPopup("AddComponent")) {
            if (engine.HasComponent<Sprite>(entity) && engine.HasComponent<Transform>(entity) &&
                engine.HasComponent<RigidBody>(entity) && engine.HasComponent<InputAffector>(entity)) {
                if (ImGui::MenuItem("Limit Reached!"))
                    ImGui::CloseCurrentPopup();
            } else {
                if (!engine.HasComponent<Sprite>(entity)) {
                    if (ImGui::MenuItem("Sprite")) {
                        engine.AddComponent<Sprite>(entity, Sprite());
                        ImGui::CloseCurrentPopup();
                    }
                }
                if (!engine.HasComponent<Transform>(entity)) {
                    if (ImGui::MenuItem("Transform")) {
                        engine.AddComponent<Transform>(entity, Transform());
                        ImGui::CloseCurrentPopup();
                    }
                }
                if (!engine.HasComponent<RigidBody>(entity)) {
                    if (ImGui::MenuItem("Rigidbody")) {
                        engine.AddComponent<RigidBody>(entity, RigidBody());
                        ImGui::CloseCurrentPopup();
                    }
                }
                if (!engine.HasComponent<InputAffector>(entity)) {
                    if (ImGui::MenuItem("InputAffector")) {
                        engine.AddComponent<InputAffector>(entity, InputAffector());
                        ImGui::CloseCurrentPopup();
                    }
                }
            }

            ImGui::EndPopup();
        }
        ImGui::SameLine();

        // Clone Entity
        ImGui::PushFont(font_bold);
        if (ImGui::Button("Clone Entity"))
            engine.CopyEntity(entity);
        ImGui::SameLine();

        // Destroy Entity
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.77f, .16f, .04f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.84f, .31f, .25f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.77f, .16f, .04f, 1.f));
        if (ImGui::Button("Destroy Entity")) {
            engine.DeleteEntity(entity);
            mSelectedEntity = {};
        }
        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::Spacing();
    }

    void SceneHierarchyPanel::RenderConfirmDelete(Entity entity, bool& show) {
        InsightEngine& engine = InsightEngine::Instance();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;

        if (ImGui::Begin("Confirm delete?", &show, window_flags)) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.8f, .1f, .15f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.9f, .2f, .2f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.8f, .1f, .15f, 1.f));

            ImGuiTableFlags table_flags = ImGuiTableFlags_NoBordersInBody;
            ImGui::BeginTable("Confirm actions", 2, table_flags, ImVec2(0, 0), 10.f);
            ImGui::TableNextColumn();
            if (ImGui::Button("CONFIRM")) {
                engine.DeleteEntity(entity);
                if (mSelectedEntity && *mSelectedEntity == entity)
                    mSelectedEntity = {};
                show = false;
            }
            ImGui::PopStyleColor(3);
            ImGui::TableNextColumn();
            if (ImGui::Button("CANCEL"))
                show = false;
            ImGui::EndTable();
            ImGui::End();
        }
    }

    void SceneHierarchyPanel::ResetSelection() { mSelectedEntity = {}; }

    template <typename Component, typename RenderFunc>
    static void SceneHierarchyPanel::RenderComponent(std::string const& label, Entity entity, RenderFunc render) {
        // Engine instance
        InsightEngine& engine = InsightEngine::Instance();
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        // Render Component Tree
        if (engine.HasComponent<Component>(entity)) {
            bool opened = ImGui::TreeNodeEx(std::bit_cast<void*>(typeid(Component).hash_code()), tree_flags, label.c_str());

            ImGui::SameLine(ImGui::GetWindowWidth() - 20.f);
            if (ImGui::Button("..."))
                ImGui::OpenPopup("ComponentConfig");

            bool remove_component = false;
            if (ImGui::BeginPopup("ComponentConfig")) {
                if (ImGui::MenuItem("Remove Component"))
                    remove_component = true;
                ImGui::EndPopup();
            }

            // Unique for each component
            if (opened) {
                auto& component = engine.GetComponent<Component>(entity);
                render(component);

                ImGui::TreePop();
            }

            if (remove_component)
                engine.RemoveComponent<Component>(entity);
        }
    }

} // end namespace IS
