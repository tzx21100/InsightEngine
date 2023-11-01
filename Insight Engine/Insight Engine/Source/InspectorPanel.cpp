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
        }
        ImGui::End(); // end window Inspector
    }

    void InspectorPanel::RenderEntityConfig(Entity entity)
    {
        InsightEngine& engine = InsightEngine::Instance();
        ImGuiIO& io = ImGui::GetIO();
        auto const FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];

        if (ImGui::BeginTable("Details", 2))
        {
            ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
            ImGui::TableSetupColumn("Label", column_flags, 50.f);

            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Name");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

            // Edit Entity Name
            std::string& name = engine.GetEntityName(entity);
            char buffer[256]{};
            auto source = name | std::ranges::views::take(name.size());
            std::ranges::copy(source, std::begin(buffer));
            ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText("##Name", buffer, sizeof(buffer), input_text_flags))
                name = std::string(buffer);

            ImGui::TableNextColumn(); // skip column

            ImGui::TableNextColumn();
            // Save Prefab
            ImGui::PushFont(FONT_BOLD);
            if (ImGui::Button("Save As Prefab"))
                engine.SaveAsPrefab(entity, name);

            // Load Prefab
            ImGui::SameLine();
            if (ImGui::Button("Load Prefab"))
                mShowPrefabs = true;
            ImGui::PopFont();

            // Prefab combo
            if (mShowPrefabs)
            {
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Prefabs");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(100.f);
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

                if (ImGui::IsAnyMouseDown() && !ImGui::IsItemHovered() && !begin_combo)
                    mShowPrefabs = false;
            }

            ImGui::TableNextColumn(); // skip column

            // Add Component
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("AddComponent");
            ImGui::PopFont();

            // Check whether entity already has the component
            if (ImGui::BeginPopup("AddComponent"))
            {
                SceneHierarchyPanel::RenderAddComponent(entity);
                ImGui::EndPopup();
            }

            ImGui::TableNextColumn(); // skip column

            // Clone Entity
            ImGui::TableNextColumn();
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

            ImGui::EndTable();
        }

        ImGui::Spacing();

    } // end RenderEntityConfig()

    void InspectorPanel::RenderComponentNodes(Entity entity)
    {
        ImGui::PushID(entity);

        // Make everything rounded
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
        ImGuiIO& io = ImGui::GetIO();
        auto FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];

        // Entity configurations
        RenderEntityConfig(entity);

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

        // Sprite Component
        RenderComponent<Sprite>("Sprite", entity, [io, FONT_BOLD, entity](Sprite& sprite)
        {
            auto& engine = InsightEngine::Instance();
            auto const editor = engine.GetSystem<Editor>("Editor");
            auto const editor_layer = editor->GetEditorLayer();
            std::filesystem::path filepath(sprite.img.mFileName);
            ImTextureID texture_id;
            float texture_width;
            float texture_height;
            bool has_texture = sprite.img.texture_id;
            bool no_dimensions = sprite.img.width == 0 || sprite.img.height == 0;
            bool missing_filename = !filepath.has_filename() && has_texture;
            bool has_animation = !sprite.anims.empty();
            std::string const& filename = missing_filename ? "Missing filename!" : filepath.filename().string();

            if (ImGui::BeginTable("Sprite Table", 2))
            {
                ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, 100.f);

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Layer");
                ImGui::PopFont();
                
                ImGui::TableNextColumn();
                Sprite::DrawLayer layer = static_cast<Sprite::DrawLayer>(sprite.layer);
                EditorUtils::RenderComboBoxEnum<Sprite::DrawLayer>("##Layer", layer, { "Background", "Default", "Foreground", "UI" });
                sprite.layer = layer;

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Color");
                ImGui::PopFont();

                ImGui::TableNextColumn();
                ImGui::ColorEdit3("##Color", &sprite.color[0]);
                ImGui::Spacing();

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Texture");
                ImGui::PopFont();

                ImGui::TableNextColumn();
                ImGui::TextColored(missing_filename ? ImVec4(1.f, .675f, .11f, 1.f) : ImVec4(1.f, 1.f, 1.f, 1.f), filename.c_str());

                ImGui::EndTable();
            }

            //placeholder for animations for now YIMING TAKE NOTE!!! JUST FOR DEMO!!!
            /*if (sprite.img.mFileName == "Assets/Textures/Player idle 1R12C.png") {
                sprite.anims.clear();
                Animation a; a.initAnimation(1, 12, 3);
                sprite.anims.emplace_back(a);
                sprite.animation_index = 0;
            }*/

            // Use placeholder if width or height of texture
            if (no_dimensions)
            {
                texture_id = editor_layer->GetIcon("TexturePlaceholder");
                texture_width = texture_height = 512.f;
            }
            else
            {
                texture_id = EditorUtils::ConvertTextureID(sprite.img.texture_id);
                texture_width = static_cast<float>(sprite.img.width);
                texture_height = static_cast<float>(sprite.img.height);
            }

            const float texture_aspect_ratio = texture_width / texture_height;
            const float draw_size = 40.f;
            ImVec2 pos = ImGui::GetCursorPos();
            ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
            ImVec4 tint_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            ImVec4 border_color = ImGui::GetStyleColorVec4(ImGuiCol_Border);

            // Render Texture as Image
            ImGuiWindowFlags child_window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding;
            ImGuiStyle& style = ImGui::GetStyle();
            if (ImGui::BeginChild("Texture Preview", { ImGui::GetContentRegionAvail().x, draw_size + 2 * (style.FramePadding.y + style.ScrollbarSize) }, false, child_window_flags))
            {
                ImGui::Image(texture_id, ImVec2(draw_size * texture_aspect_ratio, draw_size), uv_min, uv_max, tint_color, border_color);

                // Accept file drop
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMPORTED_TEXTURE"))
                    {
                        std::filesystem::path path = static_cast<wchar_t*>(payload->Data);
                        auto asset = engine.GetSystem<AssetManager>("Asset");
                        IS_CORE_DEBUG("Image : {} ", path.string());
                        sprite.img = *asset->GetImage(path.string());
                    }
                    ImGui::EndDragDropTarget();
                }

                if (has_texture && no_dimensions)
                {
                    ImGui::SameLine();
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::TextColored({ 1.f, .675f, .11f, 1.f }, "Texture attached missing dimensions!");
                    ImGui::PopFont();
                }

                // Texture Tooltip
                if (!no_dimensions && ImGui::BeginItemTooltip())
                {
                    float region_size = draw_size;
                    float zoom = 2.f;
                    float zoomed_in_height = region_size * zoom;
                    float zoomed_in_width = zoomed_in_height * texture_aspect_ratio;
                    ImGui::Image(texture_id, ImVec2(zoomed_in_width, zoomed_in_height)); // still WIP

                    ImGui::EndTooltip();
                }
            }
            ImGui::EndChild(); // end child window Texture Preview

            // Texture Dimensions
            ImGuiTableFlags table_flags = 0;
            if (ImGui::BeginTable("Texture", 2, table_flags))
            {
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Width");
                ImGui::PopFont();

                ImGui::TableNextColumn();
                ImGui::Text("%d px", sprite.img.width);
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Height");
                ImGui::PopFont();

                ImGui::TableNextColumn();
                ImGui::Text("%d px", sprite.img.height);

                ImGui::EndTable(); // end table Texture
            }

            // Render animation details
            if (has_animation)
            {
                ImGuiTreeNodeFlags animation_tree_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
                bool animation_opened = ImGui::TreeNodeEx("Animation", animation_tree_flags);
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(.8f, .8f, .8f, .8f), "(%zu)", sprite.anims.size());
                if (animation_opened)
                {
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::TextUnformatted("Active Index");
                    ImGui::PopFont();
                    ImGui::SameLine();
                    ImGui::Text("%d", sprite.animation_index);

                    if (ImGui::BeginTable("Animations Table", 1))
                    {
                        int i{};
                        for (Animation& animation : sprite.anims)
                        {
                            ImGui::TableNextColumn();
                            ImGui::PushID(i);
                            if (ImGui::BeginTable("Animation Data", 2))
                            {
                                ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
                                ImGui::PushFont(FONT_BOLD);
                                ImGui::TableSetupColumn("Index", column_flags, 100.f);
                                ImGui::PopFont();
                                ImGui::TableSetupColumn(std::to_string(i).c_str());
                                ImGui::TableHeadersRow();

                                ImGui::TableNextColumn();
                                ImGui::PushFont(FONT_BOLD);
                                ImGui::TextUnformatted("Duration");
                                ImGui::PopFont();
                                ImGui::TableNextColumn();
                                ImGui::SliderFloat("##Duration", &animation.animation_duration, 1.f, 10.f, "%.2f sec");

                                ImGui::TableNextColumn();
                                ImGui::PushFont(FONT_BOLD);
                                ImGui::TextUnformatted("Frame Duration");
                                ImGui::PopFont();
                                ImGui::TableNextColumn();
                                ImGui::SliderFloat("##Frame Duration", &animation.time_per_frame, 0.01f, 1.f, "%.2f sec");

                                ImGui::TableNextColumn();
                                ImGui::PushFont(FONT_BOLD);
                                ImGui::TextUnformatted("Columns");
                                ImGui::PopFont();
                                ImGui::TableNextColumn();
                                ImGui::SliderInt("##Columns", &animation.x_frames, 1, 20);

                                ImGui::TableNextColumn();
                                ImGui::PushFont(FONT_BOLD);
                                ImGui::TextUnformatted("Rows");
                                ImGui::PopFont();
                                ImGui::TableNextColumn();
                                ImGui::SliderInt("##Rows", &animation.y_frames, 1, 20);

                                ImGui::EndTable(); // end table Animation Data
                            }
                            Vector2D temp = { animation.frame_dimension.x, animation.frame_dimension.y };
                            EditorUtils::RenderControlVec2("Frame Size", temp);
                            animation.frame_dimension = { temp.x, temp.y };

                            ImGui::PopID();
                            ++i;
                        }
                        ImGui::EndTable(); // end table Animations
                    }

                    ImGui::TreePop(); // end tree Animation
                }
            }

        }); // end render Sprite Component

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
