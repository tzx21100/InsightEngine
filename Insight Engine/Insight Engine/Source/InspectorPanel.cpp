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
#include "GameGui.h"

// Dependencies
#include <imgui.h>
#include <IconsLucide.h>

namespace IS {

    class SceneHierarchyPanel;

    InspectorPanel::InspectorPanel(std::shared_ptr<SceneHierarchyPanel> scene_hierarchy_panel)
        : mSceneHierarchyPanel(scene_hierarchy_panel) {}

    void InspectorPanel::RenderPanel()
    {
        ImGui::Begin(ICON_LC_INFO "  Inspector");
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

            // Edit Entity Name
            std::string& name = engine.GetEntityName(entity);
            char buffer[256]{};
            auto source = name | std::ranges::views::take(name.size());
            std::ranges::copy(source, std::begin(buffer));
            ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText("##Name", buffer, sizeof(buffer), input_text_flags))
                name = std::string(buffer);

            ImGui::SameLine();

            // Save Prefab
            if (ImGui::Button(ICON_LC_FOLDER_UP))
                engine.SaveAsPrefab(entity, name);
            ImGui::SetItemTooltip("Save as Prefab");

            // Load Prefab
            ImGui::SameLine();
            if (ImGui::Button(ICON_LC_FOLDER_DOWN))
                mShowPrefabs = true;
            ImGui::SetItemTooltip("Load Prefab");

            ImGui::SameLine();

            if (ImGui::Button(ICON_LC_MORE_VERTICAL))
                ImGui::OpenPopup("Entity Options");

            if (ImGui::BeginPopup("Entity Options"))
            {
                if (ImGui::BeginMenu("Add Component"))
                {
                    SceneHierarchyPanel::RenderAddComponent(entity);

                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Clone Entity"))
                {
                    engine.CopyEntity(entity);
                }
                if (ImGui::MenuItem("Delete Entity"))
                {
                    engine.DeleteEntity(entity);
                    mSceneHierarchyPanel->ResetSelection();
                }

                ImGui::EndPopup();
            }

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
                            engine.LoadFromPrefab(prefab, entity);
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
        RenderComponent<Transform>(ICON_LC_MOVE "  Transform", entity, [FONT_BOLD](Transform& transform)
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
        RenderComponent<Sprite>(ICON_LC_IMAGE "  Sprite", entity, [io, FONT_BOLD, entity, this](Sprite& sprite)
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
                ImGui::TextUnformatted("Rendering");
                ImGui::PopFont();

                ImGui::TableNextColumn();
                ImGui::Checkbox("##Rendering", &sprite.toRender);

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

            if (has_texture)
            {
                if (ImGui::Button("Add Animation"))
                    mShowAddAnimation = true;
            }

            // Render animation details
            if (has_animation)
            {
                ImGuiTreeNodeFlags animation_tree_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
                bool animation_opened = ImGui::TreeNodeEx(ICON_LC_FILM  " Animation", animation_tree_flags);
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(.8f, .8f, .8f, .8f), "(%zu)", sprite.anims.size());
                if (animation_opened)
                {
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::TextUnformatted("Active Animation: ");
                    ImGui::PopFont();
                    ImGui::SameLine();
                    std::string const& active_animation = sprite.anims[sprite.animation_index].name;
                    ImGui::TextUnformatted(active_animation.c_str());

                    if (ImGui::BeginTable("Animations Table", 1))
                    {
                        int i{};
                        for (Animation& animation : sprite.anims)
                        {
                            ImGui::TableNextColumn();
                            ImGui::PushID(i);

                            ImGui::Separator();
                            if (ImGui::BeginTable("Animation Data", 2))
                            {
                                ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
                                ImGui::TableSetupColumn("Labels", column_flags, 100.f);

                                ImGui::TableNextColumn();
                                ImGui::PushFont(FONT_BOLD);
                                ImGui::TextUnformatted("Name");
                                ImGui::PopFont();

                                ImGui::TableNextColumn();
                                char buffer[256]{};
                                std::memcpy(buffer, animation.name.c_str(), animation.name.length());

                                ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
                                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                                if (ImGui::InputText("##AnimationName", buffer, sizeof(buffer), input_text_flags))
                                    animation.name = buffer;

                                ImGui::TableNextColumn();
                                ImGui::PushFont(FONT_BOLD);
                                ImGui::TextUnformatted("Duration");
                                ImGui::PopFont();
                                ImGui::TableNextColumn();
                                ImGui::SliderFloat("##Duration", &animation.animation_duration, 1.f, 10.f, "%.2f sec");

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
                            ImGui::PopID();
                            ++i;
                        }
                        ImGui::EndTable(); // end table Animations
                    }

                    ImGui::TreePop(); // end tree Animation
                }
            }

            if (mShowAddAnimation)
                AddAnimation(sprite);

        }); // end render Sprite Component

        // Rigidbody Component
        RenderComponent<RigidBody>(ICON_LC_PERSON_STANDING "  Rigidbody", entity, [entity, FONT_BOLD](RigidBody& rigidbody)
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
        RenderComponent<ScriptComponent>(ICON_LC_BRACES "  Script", entity, [FONT_BOLD](ScriptComponent& script)
        {
            // Display Sprite Name
            if (ImGui::BeginTable("ScriptTable", 2))
            {
                // Set Table and its flags
                ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
                ImGui::TableSetupColumn("ScriptTable", column_flags, 100.f);

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Script Name:");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::Text("%s", script.mScriptName.c_str());

                ImGui::SameLine();

                // Modify Script
                if (ImGui::Button(ICON_LC_FILE_SEARCH))
                {
                    if (std::filesystem::path filepath(FileUtils::OpenAndGetScript()); !filepath.empty())
                        script.mScriptName = filepath.stem().string();
                }
                ImGui::SetItemTooltip("Browse and replace the existing script");

                ImGui::SameLine();

                // Edit Script
                if (ImGui::Button(ICON_LC_FILE_EDIT))
                    InsightEngine::Instance().OpenGameScript(script.mScriptName + ".cs");
                ImGui::SetItemTooltip("Open and edit script in default application");

                ImGui::EndTable();
            }

        }); // end render Script Component

        // Button Component
        RenderComponent<ButtonComponent>(ICON_LC_SQUARE "  Button", entity, [entity, FONT_BOLD](ButtonComponent& button)
        {
            if (ImGui::BeginTable("Button Table", 2))
            {
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Text");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

                // Edit button text
                std::string& text = button.mButtonText;
                char buffer[256]{};
                auto source = text | std::ranges::views::take(text.size());
                std::ranges::copy(source, std::begin(buffer));
                ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
                if (ImGui::InputText("##ButtonText", buffer, sizeof(buffer), input_text_flags))
                    text = std::string(buffer);

                ImGui::EndTable(); // end table Button Table
            }
        }); // end render Button Component

        ImGui::PopStyleVar(); // end style rounding
        ImGui::PopID();

    } // end RenderComponentNodes()

    template <typename Component, typename RenderFunc>
    void InspectorPanel::RenderComponent(std::string const& label, Entity entity, RenderFunc render)
    {
        // Engine instance
        InsightEngine& engine = InsightEngine::Instance();
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        ImGuiStyle& style = ImGui::GetStyle();

        // Render Component Tree
        if (engine.HasComponent<Component>(entity))
        {
            bool opened = ImGui::TreeNodeEx(std::bit_cast<void*>(typeid(Component).hash_code()), tree_flags, label.c_str());

            // Display Component Config
            ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_LC_MORE_VERTICAL).x - style.ItemSpacing.x);

            if (ImGui::Button(ICON_LC_MORE_VERTICAL))
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

    void InspectorPanel::AddAnimation(Sprite& sprite)
    {
        auto const FONT_BOLD = ImGui::GetIO().Fonts->Fonts[FONT_TYPE_BOLD];
        ImGui::OpenPopup("Add Animation");

        if (ImGui::BeginPopupModal("Add Animation", &mShowAddAnimation, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static char name[256]{};
            static int grid[2]{};
            static float time{};
            static bool show_warning = false;

            if (ImGui::BeginTable("Add Animation Table", 2))
            {
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Name");
                ImGui::PopFont();

                ImGui::TableNextColumn();
                ImGui::InputText("##Animation Name", name, sizeof(name));

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Columns/Rows");
                ImGui::PopFont();

                ImGui::TableNextColumn();
                ImGui::InputInt2("##Animation Grid", grid);

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Time");
                ImGui::PopFont();

                ImGui::TableNextColumn();
                ImGui::InputFloat("##Animation Time", &time);

                if (ImGui::Button("Add"))
                {
                    if (strcmp(name, "") == 0 || grid[0] == 0 || grid[1] == 0 || time == 0.f)
                    {
                        show_warning = true;
                    }
                    else
                    {
                        sprite.AddAnimation(name, grid[0], grid[1], time);
                        mShowAddAnimation = false;
                        // Reset the static variables when the popup is closed or canceled
                        std::memset(name, 0, sizeof(name));
                        std::memset(grid, 0, sizeof(grid));
                        time = 0.0f;
                        show_warning = false;
                    }
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    mShowAddAnimation = false;
                    // Reset the static variables when the popup is closed or canceled
                    std::memset(name, 0, sizeof(name));
                    std::memset(grid, 0, sizeof(grid));
                    time = 0.0f;
                    show_warning = false;
                }

                if (show_warning)
                {
                    ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "One or more fields are invalid");
                }


                ImGui::EndTable();
            }
            ImGui::EndPopup();
        }
    }

} // end namespace IS
