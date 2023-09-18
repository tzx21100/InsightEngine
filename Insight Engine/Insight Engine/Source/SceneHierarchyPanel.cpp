#include "Pch.h"
#include "SceneHierarchyPanel.h"
#include "Guidgets.h"
#include "CoreEngine.h"
#include "Graphics.h"

// Dependencies
#include <imgui.h>

namespace IS {

    void SceneHierarchyPanel::RenderPanel() {
        // Data sources
        InsightEngine& engine = InsightEngine::Instance();
        std::shared_ptr<ParentSystem> graphics = engine.GetSystemPointer().at("Graphics");

        // Begin creating the scene hierarchy panel
        ImGui::Begin("Hierarchy");
        for (auto& entity : graphics->mEntities) {
            RenderEntityNode(entity);
        }

        // Deselect entity
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            selected_entity = {};

        ImGui::End();

        // Render inspector for selected entity
        ImGui::Begin("Inspector");
        RenderComponentNodes(selected_entity);
        ImGui::End();
    }

    void SceneHierarchyPanel::RenderEntityNode(Entity entity) {
        InsightEngine& engine = InsightEngine::Instance();
        // doing this because entity does not have a name
        auto& sprite = engine.GetComponent<Sprite>(entity);

        ImGuiTreeNodeFlags tree_flags = (selected_entity && (*selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0);
        tree_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((sprite.name + ' ' + std::to_string(entity)).c_str(), tree_flags);

        if (ImGui::IsItemClicked())
            selected_entity = std::make_shared<Entity>(entity);

        if (opened)
            ImGui::TreePop();
    }

    void SceneHierarchyPanel::RenderComponentNodes(std::shared_ptr<Entity> entity) {
        if (!entity)
            return;

        // Engine instance
        InsightEngine& engine = InsightEngine::Instance();
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        // Transform component tree
        if (engine.HasComponent<Transform>(*entity)) {
            bool opened = ImGui::TreeNodeEx(std::bit_cast<void*>(typeid(Transform).hash_code()), tree_flags, "Transform");

            if (opened) {
                auto& transform = engine.GetComponent<Transform>(*entity);

                Vector2D position = { transform.world_position.x, transform.world_position.y };
                Vector2D orientation = { transform.orientation.x, transform.orientation.y };
                Vector2D scale = { transform.scaling.x, transform.scaling.y };
                guidgets::RenderControlVec2("Position", position);
                guidgets::RenderControlVec2("Orientation", orientation);
                guidgets::RenderControlVec2("Scale", scale, 95.f, 120.f);
                transform.world_position = { position.x, position.y };
                transform.orientation = { orientation.x, orientation.y };
                transform.scaling = { scale.x, scale.y };

                ImGui::TreePop();
            }
        }

        // Sprite component tree
        if (engine.HasComponent<Sprite>(*entity)) {
            bool opened = ImGui::TreeNodeEx(std::bit_cast<void*>(typeid(Sprite).hash_code()), tree_flags, "Sprite");

            if (opened) {
                auto& sprite = engine.GetComponent<Sprite>(*entity);
                if (sprite.texture) {
                    ImGui::Text("Texture");
                    ImTextureID texture_id = std::bit_cast<void*>(static_cast<uintptr_t>(sprite.texture));
                    const float texture_width = static_cast<float>(sprite.texture_width);
                    const float texture_height = static_cast<float>(sprite.texture_height);
                    const float texture_aspect_ratio = texture_width / texture_height;
                    const float draw_size = 200.f;
                    ImGuiIO& io = ImGui::GetIO();
                    ImVec2 pos = ImGui::GetCursorPos();
                    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
                    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
                    ImVec4 tint_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    ImVec4 border_color = ImGui::GetStyleColorVec4(ImGuiCol_Border);
                    ImGui::Image(texture_id, ImVec2(draw_size, draw_size / texture_aspect_ratio), uv_min, uv_max, tint_color, border_color);
                    if (ImGui::BeginItemTooltip()) {
                        float region_size = 32.0f;
                        float region_x = io.MousePos.x - pos.x - region_size * 0.5f;
                        float region_y = io.MousePos.y - pos.y - region_size * 0.5f;
                        float zoom = 4.0f;
                        if (region_x < 0.0f) { region_x = 0.0f; } else if (region_x > texture_width - region_size) { region_x = texture_width - region_size; }
                        if (region_y < 0.0f) { region_y = 0.0f; } else if (region_y > texture_height - region_size) { region_y = texture_height - region_size; }
                        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_size, region_y + region_size);
                        ImVec2 uv0 = ImVec2((region_x) / texture_width, (region_y) / texture_height);
                        ImVec2 uv1 = ImVec2((region_x + region_size) / texture_width, (region_y + region_size) / texture_height);
                        float zoomed_in_width = region_size * zoom;
                        float zoomed_in_height = zoomed_in_width / texture_aspect_ratio;
                        ImGui::Image(texture_id, ImVec2(zoomed_in_width, zoomed_in_height)); // still WIP
                        ImGui::EndTooltip();
                    }

                    ImGuiTableFlags table_flags = 0;
                    ImGui::BeginTable("Texture", 2, table_flags);
                    ImGui::TableNextColumn();
                    ImGui::Text("Width");
                    ImGui::TableNextColumn();
                    ImGui::Text("%dpx", sprite.texture_width);
                    ImGui::TableNextColumn();
                    ImGui::Text("Height");
                    ImGui::TableNextColumn();
                    ImGui::Text("%dpx", sprite.texture_height);
                    ImGui::EndTable();

                } else {
                    ImGui::Text("Color");
                    ImGui::SameLine();
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
                    ImGui::ColorEdit3(("##Color" + std::to_string(*entity)).c_str(), &sprite.color[0]);
                    ImGui::PopStyleVar();
                }
                ImGui::TreePop();
            }
        }

        // Rigidbody component tree
        if (engine.HasComponent<RigidBody>(*entity)) {
            bool opened = ImGui::TreeNodeEx(std::bit_cast<void*>(typeid(RigidBody).hash_code()), tree_flags, "Rigidbody");

            if (opened) {
                auto& rigidbody = engine.GetComponent<RigidBody>(*entity);
                ImGuiTableFlags table_flags = 0;

                guidgets::RenderControlVec2("Velocity", rigidbody.velocity);
                guidgets::RenderControlVec2("Force", rigidbody.force);

                if (ImGui::BeginTable(("RigidbodyTable" + std::to_string(*entity)).c_str(), 2, table_flags)) {
                    ImGui::TableNextColumn();
                    ImGui::Text("Body Type");
                    ImGui::TableNextColumn();
                    guidgets::RenderComboBoxBodyType("##Body Type", rigidbody.bodyType);

                    ImGui::TableNextColumn();
                    ImGui::Text("Body Shape");
                    ImGui::TableNextColumn();
                    guidgets::RenderComboBoxBodyShape("##Body Shape", rigidbody.bodyShape);

                    ImGui::TableNextColumn();
                    ImGui::Text("Mass");
                    ImGui::TableNextColumn();
                    ImGui::DragFloat(("##Mass" + std::to_string(*entity)).c_str(), &rigidbody.mass, 1.f, 0.f, 0.f, "%.2f");

                    ImGui::TableNextColumn();
                    ImGui::Text("Invariant Mass");
                    ImGui::TableNextColumn();
                    ImGui::DragFloat(("##InvMass" + std::to_string(*entity)).c_str(), &rigidbody.InvMass, 1.f, 0.f, 0.f, "%.2f");

                    ImGui::TableNextColumn();
                    ImGui::Text("Restitution");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.2f", rigidbody.restitution);

                    ImGui::TableNextColumn();
                    ImGui::Text("Density");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.2f", rigidbody.density);

                    ImGui::TableNextColumn();
                    ImGui::Text("Vertex Count");
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", rigidbody.vertices.size());

                    ImGui::EndTable();
                    ImGui::Separator();
                }

                ImGui::TreePop();
            }
        }

        // InputAffector component tree
        if (engine.HasComponent<InputAffector>(*entity)) {
            bool opened = ImGui::TreeNodeEx(std::bit_cast<void*>(typeid(InputAffector).hash_code()), tree_flags, "InputAffector");

            if (opened) {
                //auto& inputaffector = engine.GetComponent<InputAffector>(entity);
                ImGui::Text("(Empty)");
                ImGui::TreePop();
            }
        }

        // Aditional actions
        bool actions_opened = ImGui::TreeNodeEx(std::bit_cast<void*>(typeid("Actions").hash_code()), tree_flags, "Actions");
        if (actions_opened) {

            // Delete entity button
            static bool show = false;
            if (ImGui::Button("DELETE ENTITY"))
                show = true;
            if (show) {
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
                                                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
                if (ImGui::Begin("Confirm delete?", &show, window_flags)) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.8f, .1f, .15f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.9f, .2f, .2f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.8f, .1f, .15f, 1.f));

                    ImGuiTableFlags table_flags = ImGuiTableFlags_NoBordersInBody;
                    ImGui::BeginTable("Confirm actions", 2, table_flags, ImVec2(0, 0), 10.f);
                    ImGui::TableNextColumn();
                    if (ImGui::Button("CONFIRM")) {
                        engine.DestroyEntity(*entity);
                        selected_entity = {};
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

            if (ImGui::Button("CLONE ENTITY")) {
                // WIP
            }

            ImGui::TreePop();
        }
    }

} // end namespace IS
