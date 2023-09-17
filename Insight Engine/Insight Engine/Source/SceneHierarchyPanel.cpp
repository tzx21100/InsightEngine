#include "Pch.h"
#include "SceneHierarchyPanel.h"
#include "Widgets.h"
#include "CoreEngine.h"
#include "Graphics.h"

// Dependencies
#include <imgui.h>
#include <imgui_internal.h>

namespace IS {

    void SceneHierarchyPanel::RenderPanel() {
        // Data sources
        InsightEngine& engine = InsightEngine::Instance();
        std::shared_ptr<ISGraphics> graphics = std::dynamic_pointer_cast<ISGraphics>(engine.GetSystemPointer().at("Graphics"));

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

        ImGuiTreeNodeFlags entity_tree_flags = ((entity_selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_FramePadding;
        bool opened = ImGui::TreeNodeEx((void*)(uintptr_t)entity, entity_tree_flags, (sprite.name + ' ' + std::to_string(entity)).c_str());

        if (ImGui::IsItemClicked())
            entity_selected = entity;

        if (opened)
            ImGui::TreePop();
    }

    void SceneHierarchyPanel::RenderComponentNodes(Entity entity) {
        // Engine instance
        InsightEngine& engine = InsightEngine::Instance();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;

        // Transform component tree
        if (engine.HasComponent<Transform>(entity)) {
            bool opened = ImGui::TreeNodeEx((std::to_string(entity) + "Transform").c_str(), flags, "Transform");

            if (opened) {
                auto& transform = engine.GetComponent<Transform>(entity);

                Vector2D position = { transform.world_position.x, transform.world_position.y };
                Vector2D orientation = { transform.orientation.x, transform.orientation.y };
                Vector2D scale = { transform.scaling.x, transform.scaling.y };
                RenderControlVec2("Position", position);
                RenderControlVec2("Orientation", orientation);
                RenderControlVec2("Scale", scale, 95.f, 120.f);
                transform.world_position = { position.x, position.y };
                transform.orientation = { orientation.x, orientation.y };
                transform.scaling = { scale.x, scale.y };

                ImGui::TreePop();
            }
        }

        // Sprite component tree
        if (engine.HasComponent<Sprite>(entity)) {
            bool opened = ImGui::TreeNodeEx((std::to_string(entity) + "Sprite").c_str(), flags, "Sprite");

            if (opened) {
                auto& sprite = engine.GetComponent<Sprite>(entity);
                if (sprite.texture) { // temp
                    // needa function to get image width and height
                    ImGuiIO& io = ImGui::GetIO();
                    ImVec2 pos = ImGui::GetCursorPos();
                    ImTextureID my_tex_id = (void*)(uintptr_t)sprite.texture;
                    float my_tex_w = 760.f;
                    float my_tex_h = 120.f;
                    ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h));
                    if (ImGui::BeginItemTooltip()) {
                        float region_sz = 32.0f;
                        float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
                        float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
                        float zoom = 4.0f;
                        if (region_x < 0.0f) { region_x = 0.0f; } else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
                        if (region_y < 0.0f) { region_y = 0.0f; } else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
                        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
                        ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                        ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                        ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom));
                        ImGui::EndTooltip();
                    }
                } else {
                    ImGui::Text("%-6s", "Color");
                    ImGui::SameLine();
                    ImGui::ColorEdit3(("##Color" + std::to_string(entity)).c_str(), &sprite.color[0]);
                }
                ImGui::TreePop();
            }
        }

        // Rigidbody component tree
        if (engine.HasComponent<RigidBody>(entity)) {
            bool opened = ImGui::TreeNodeEx((std::to_string(entity) + "RigidBody").c_str(), flags, "RigidBody");

            if (opened) {
                auto& rigidbody = engine.GetComponent<RigidBody>(entity);
                ImGuiTableFlags table_flags = ImGuiTableFlags_BordersInner;

                if (ImGui::BeginTable((std::to_string(entity) + "RigidBodyTable1").c_str(), 2, table_flags)) {
                    ImGui::TableNextColumn();
                    ImGui::Text("Body Type: ");
                    ImGui::TableNextColumn();
                    switch (rigidbody.bodyType) {
                    case BodyType::Static:
                        ImGui::Text("Static");
                        break;
                    case BodyType::Dynamic:
                        ImGui::Text("Dynamic");
                        break;
                    case BodyType::Kinematic:
                        ImGui::Text("Kinematic");
                        break;
                    default:
                        break;
                    }
                    ImGui::TableNextColumn();
                    ImGui::Text("Body Shape: ");
                    ImGui::TableNextColumn();
                    switch (rigidbody.bodyShape) {
                    case Shape::Box:
                        ImGui::Text("Box");
                        break;
                    case Shape::Circle:
                        ImGui::Text("Circle");
                        break;
                    case Shape::Line:
                        ImGui::Text("Line");
                        break;
                    default:
                        break;
                    }

                    ImGui::EndTable();
                    ImGui::Separator();
                }

                RenderControlVec2("Velocity", rigidbody.velocity);
                RenderControlVec2("Force", rigidbody.force);

                if (ImGui::BeginTable((std::to_string(entity) + "RigidBodyTable2").c_str(), 2, table_flags)) {
                    ImGui::TableNextColumn();
                    ImGui::Text("Density");
                    ImGui::TableNextColumn();
                    ImGui::DragFloat(("##Density" + std::to_string(entity)).c_str(), &rigidbody.density);

                    ImGui::TableNextColumn();
                    ImGui::Text("Mass");
                    ImGui::TableNextColumn();
                    ImGui::DragFloat(("##Mass" + std::to_string(entity)).c_str(), &rigidbody.mass);

                    ImGui::TableNextColumn();
                    ImGui::Text("InvMass");
                    ImGui::TableNextColumn();
                    ImGui::DragFloat(("##InvMass" + std::to_string(entity)).c_str(), &rigidbody.InvMass);

                    ImGui::TableNextColumn();
                    ImGui::Text("Restitution");
                    ImGui::TableNextColumn();
                    ImGui::DragFloat(("##Restitution" + std::to_string(entity)).c_str(), &rigidbody.restitution);

                    ImGui::TableNextColumn();
                    ImGui::Text("Area");
                    ImGui::TableNextColumn();
                    ImGui::DragFloat(("##Area" + std::to_string(entity)).c_str(), &rigidbody.area);

                    ImGui::TableNextColumn();
                    ImGui::Text("Vertex Count:");
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", rigidbody.vertices.size());

                    ImGui::TableNextColumn();
                    ImGui::Text("Transform Update");
                    ImGui::TableNextColumn();
                    ImGui::Checkbox("##Transform Update", &rigidbody.transformUpdateRequired);

                    ImGui::EndTable();
                    ImGui::Separator();
                }

                ImGui::TreePop();
            }
        }

        if (engine.HasComponent<InputAffector>(entity)) {
            bool opened = ImGui::TreeNodeEx((std::to_string(entity) + "InputAffector").c_str(), flags, "InputAffector");

            if (opened) {
                //auto& inputaffector = engine.GetComponent<InputAffector>(entity);

                ImGui::TreePop();
            }
        }
    }

} // end namespace IS
