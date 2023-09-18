#include "Pch.h"
#include "SceneHierarchyPanel.h"
#include "Guidgets.h"
#include "CoreEngine.h"
#include "Graphics.h"

// Dependencies
#include <imgui.h>
#include <imgui_internal.h>

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
        ImGui::End();

        // Render inspector for selected entity
        ImGui::Begin("Inspector");
        RenderComponentNodes(entity_selected);
        ImGui::End();
    }

    void SceneHierarchyPanel::RenderEntityNode(Entity entity) {
        InsightEngine& engine = InsightEngine::Instance();
        // doing this because entity does not have a name
        auto& sprite = engine.GetComponent<Sprite>(entity);

        ImGuiTreeNodeFlags entity_tree_flags = (entity_selected && (*entity_selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
                                                ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_FramePadding;
        bool opened = ImGui::TreeNodeEx((sprite.name + ' ' + std::to_string(entity)).c_str(), entity_tree_flags);

        if (ImGui::IsItemClicked())
            entity_selected = std::make_shared<Entity>(entity);

        if (opened)
            ImGui::TreePop();
    }

    void SceneHierarchyPanel::RenderComponentNodes(std::shared_ptr<Entity> entity) {
        if (entity == nullptr)
            return;

        // Engine instance
        InsightEngine& engine = InsightEngine::Instance();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;

        // Transform component tree
        if (engine.HasComponent<Transform>(*entity)) {
            bool opened = ImGui::TreeNodeEx((std::to_string(*entity) + "Transform").c_str(), flags, "Transform");

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
            bool opened = ImGui::TreeNodeEx((std::to_string(*entity) + "Sprite").c_str(), flags, "Sprite");

            if (opened) {
                auto& sprite = engine.GetComponent<Sprite>(*entity);
                if (sprite.texture) {
                    ImGui::Text("Texture");
                    ImGuiIO& io = ImGui::GetIO();
                    ImVec2 pos = ImGui::GetCursorPos();
                    ImTextureID texture_id = std::bit_cast<void*>(static_cast<uintptr_t>(sprite.texture));
                    float texture_width  = static_cast<float>(sprite.texture_width);
                    float texture_height = static_cast<float>(sprite.texture_height);
                    float texture_aspect_ratio = texture_width / texture_height;
                    float draw_size = 200.f;
                    ImGui::Image(texture_id, ImVec2(draw_size, draw_size / texture_aspect_ratio));
                    if (ImGui::BeginItemTooltip()) {
                        float region_sz = 32.0f;
                        float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
                        float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
                        float zoom = 4.0f;
                        if (region_x < 0.0f) { region_x = 0.0f; } else if (region_x > texture_width - region_sz) { region_x = texture_width - region_sz; }
                        if (region_y < 0.0f) { region_y = 0.0f; } else if (region_y > texture_height - region_sz) { region_y = texture_height - region_sz; }
                        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
                        ImVec2 uv0 = ImVec2((region_x) / texture_width, (region_y) / texture_height);
                        ImVec2 uv1 = ImVec2((region_x + region_sz) / texture_width, (region_y + region_sz) / texture_height);
                        float zoomed_in_width = region_sz * zoom;
                        float zoomed_in_height = zoomed_in_width / texture_aspect_ratio;
                        ImGui::Image(texture_id, ImVec2(zoomed_in_width, zoomed_in_height));
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
            bool opened = ImGui::TreeNodeEx((std::to_string(*entity) + "RigidBody").c_str(), flags, "RigidBody");

            if (opened) {
                auto& rigidbody = engine.GetComponent<RigidBody>(*entity);
                ImGuiTableFlags table_flags = 0;

                guidgets::RenderControlVec2("Velocity", rigidbody.velocity);
                guidgets::RenderControlVec2("Force", rigidbody.force);

                if (ImGui::BeginTable((std::to_string(*entity) + "RigidBodyTable").c_str(), 2, table_flags)) {
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
                    ImGui::Text("InvMass");
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
            bool opened = ImGui::TreeNodeEx((std::to_string(*entity) + "InputAffector").c_str(), flags, "InputAffector");

            if (opened) {
                //auto& inputaffector = engine.GetComponent<InputAffector>(entity);

                ImGui::TreePop();
            }
        }

        // Aditional actions
        bool actions_opened = ImGui::TreeNodeEx((std::to_string(*entity) + "Actions").c_str(), flags, "Actions");
        if (actions_opened) {

            // Delete entity button
            static bool show = false;
            if (ImGui::Button("DELETE ENTITY"))
                show = true;
            if (show) {
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;
                if (ImGui::Begin("Confirm delete?", &show, window_flags)) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.8f, .1f, .15f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.9f, .2f, .2f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.8f, .1f, .15f, 1.f));

                    ImGuiTableFlags table_flags = ImGuiTableFlags_NoBordersInBody;
                    ImGui::BeginTable("Confirm actions", 2, table_flags, ImVec2(0, 0), 10.f);
                    ImGui::TableNextColumn();
                    if (ImGui::Button("CONFIRM")) {
                        engine.DestroyEntity(*entity);
                        entity_selected = nullptr;
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
