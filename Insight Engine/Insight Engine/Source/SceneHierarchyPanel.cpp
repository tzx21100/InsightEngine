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

        // Create random entity
        ImGuiPopupFlags flags = ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight;
        if (ImGui::BeginPopupContextWindow(0, flags)) {
            if (ImGui::MenuItem("Create Random Entity"))
                engine.GenerateRandomEntity();
            ImGui::EndPopup();
        }

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

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Clone Entity")) {
                Entity e = engine.CreateEntity();
                engine.AddComponent<Sprite>(e, sprite);
                if (engine.HasComponent<Transform>(entity)) {
                    auto& transform = engine.GetComponent<Transform>(entity);
                    engine.AddComponent<Transform>(e, transform);
                }
                if (engine.HasComponent<RigidBody>(entity)) {
                    auto& rigidbody = engine.GetComponent<RigidBody>(entity);
                    engine.AddComponent<RigidBody>(e, rigidbody);
                }
                if (engine.HasComponent<InputAffector>(entity)) {
                    auto& input_affector = engine.GetComponent<InputAffector>(entity);
                    engine.AddComponent<InputAffector>(e, input_affector);
                }
            }

            ImGui::EndPopup();
        }

        if (opened)
            ImGui::TreePop();
    }

    void SceneHierarchyPanel::RenderComponentNodes(std::shared_ptr<Entity> entity) {
        // No entity selected
        if (!entity)
            return;

        // Sprite Component
        RenderComponent<Sprite>("Sprite", *entity, [&](auto& sprite) {
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
        });

        // Transform Component
        RenderComponent<Transform>("Transform", *entity, [](auto& transform) {
            Vector2D position = { transform.world_position.x, transform.world_position.y };
            Vector2D orientation = { transform.orientation.x, transform.orientation.y };
            Vector2D scale = { transform.scaling.x, transform.scaling.y };
            guidgets::RenderControlVec2("Position", position);
            guidgets::RenderControlVec2("Orientation", orientation);
            guidgets::RenderControlVec2("Scale", scale, 95.f, 120.f);
            transform.world_position = { position.x, position.y };
            transform.orientation = { orientation.x, orientation.y };
            transform.scaling = { scale.x, scale.y };
        });

        // Rigidbody Component
        RenderComponent<RigidBody>("Rigidbody", *entity, [&](auto& rigidbody) {
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
        });

        // Input Affector
        RenderComponent<InputAffector>("Input Affector", *entity, []([[maybe_unused]]auto& input_affector) {
            ImGui::Text("(Empty)");
        });

        // Aditional actions (temp)
        InsightEngine& engine = InsightEngine::Instance();
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        bool actions_opened = ImGui::TreeNodeEx(std::bit_cast<void*>(typeid("Configuration").hash_code()), tree_flags, "Configuration");
        if (actions_opened) {

            // Destroy entity
            if (ImGui::Button("DESTROY ENTITY")) {
                engine.DestroyEntity(*entity);
                selected_entity = {};
            }

            //// Confirm deletion
            //static bool show = false;
            //if (ImGui::Button("DESTROY ENTITY"))
            //    show = true;
            //if (show) {
            //    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
            //                                    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
            //    if (ImGui::Begin("Confirm delete?", &show, window_flags)) {
            //        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.8f, .1f, .15f, 1.f));
            //        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.9f, .2f, .2f, 1.f));
            //        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.8f, .1f, .15f, 1.f));

            //        ImGuiTableFlags table_flags = ImGuiTableFlags_NoBordersInBody;
            //        ImGui::BeginTable("Confirm actions", 2, table_flags, ImVec2(0, 0), 10.f);
            //        ImGui::TableNextColumn();
            //        if (ImGui::Button("CONFIRM")) {
            //            engine.DestroyEntity(*entity);
            //            selected_entity = {};
            //            show = false;
            //        }
            //        ImGui::PopStyleColor(3);
            //        ImGui::TableNextColumn();
            //        if (ImGui::Button("CANCEL"))
            //            show = false;
            //        ImGui::EndTable();
            //        ImGui::End();
            //    }
            //}

            // Clone entity
            if (ImGui::Button("CLONE ENTITY")) {
                Entity e = engine.CreateEntity();
                if (engine.HasComponent<Sprite>(*entity)) {
                    auto& sprite = engine.GetComponent<Sprite>(*entity);
                    engine.AddComponent<Sprite>(e, sprite);
                }
                if (engine.HasComponent<Transform>(*entity)) {
                    auto& transform = engine.GetComponent<Transform>(*entity);
                    engine.AddComponent<Transform>(e, transform);
                }
                if (engine.HasComponent<RigidBody>(*entity)) {
                    auto& rigidbody = engine.GetComponent<RigidBody>(*entity);
                    engine.AddComponent<RigidBody>(e, rigidbody);
                }
                if (engine.HasComponent<InputAffector>(*entity)) {
                    auto& input_affector = engine.GetComponent<InputAffector>(*entity);
                    engine.AddComponent<InputAffector>(e, input_affector);
                }
            }

            ImGui::TreePop();
        }
    }

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
