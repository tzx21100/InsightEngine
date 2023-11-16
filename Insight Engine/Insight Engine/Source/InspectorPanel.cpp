/*!
 * \file HierarchyPanel.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for class HierarchyPanel,
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
#include "GameGui.h"
#include "Audio.h"
#include "Body.h"
#include "Collider.h"
#include "CommandHistory.h"
#include "CollisionSystem.h"

// Dependencies
#include <imgui.h>

namespace IS {

    void InspectorPanel::RenderPanel()
    {
        ImGui::BeginDisabled(Camera3D::mActiveCamera == CAMERA_TYPE_GAME);

        ImGui::Begin((ICON_LC_INFO "  " + mName).c_str());

        // Window contents
        {
            switch (mInspectMode)
            {
            case aInspectMode::INSPECT_CAMERA:
                RenderCameraControls();
                break;
            case aInspectMode::INSPECT_SCENE:
                break;
            case aInspectMode::INSPECT_ENTITY:
                if (mEditorLayer.IsAnyEntitySelected())
                {
                    RenderComponentNodes(mEditorLayer.GetSelectedEntity());
                }
                break;
            default:
                break;
            }
        }

        // Save window states
        mFocused = ImGui::IsWindowFocused();
        mAppearing = ImGui::IsWindowAppearing();
        mHovered = ImGui::IsItemHovered();
        ImGui::End(); // end window Inspector
        ImGui::EndDisabled();
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
                    HierarchyPanel::RenderAddComponent(entity);

                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Clone Entity"))
                {
                    engine.CopyEntity(entity);
                }
                if (ImGui::MenuItem("Delete Entity"))
                {
                    engine.DeleteEntity(entity);
                    mEditorLayer.ResetEntitySelection();
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

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);

        // Make everything rounded
        ImGuiIO& io = ImGui::GetIO();
        auto FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];

        // Entity configurations
        RenderEntityConfig(entity);

        // Transform Component
        RenderComponent<Transform>(ICON_LC_MOVE "  Transform", entity, [this, entity, FONT_BOLD](Transform& transform)
        {
            // Render Translation
            Vec2 position = transform.world_position;
            if (EditorUtils::RenderControlVec2("Translation", position))
            {
                if (position != transform.world_position)
                    CommandHistory::AddCommand(std::make_shared<Vec2Command>(transform.world_position, position));
            }

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
                // Apply modification
                if (ImGui::SliderAngle("##Rotation", &rotation, 0.f))
                {
                    float new_rotation = rotation / (PI / 180.f);
                    if (new_rotation != transform.rotation)
                        CommandHistory::AddCommand(std::make_shared<FloatCommand>(transform.rotation, new_rotation));
                }

                ImGui::EndTable();
            }

            // Render Scale
            Vec2 scaling = transform.scaling;
            if (EditorUtils::RenderControlVec2("Scale", scaling, 95.f, 120.f))
            {
                if (scaling != transform.scaling)
                    CommandHistory::AddCommand(std::make_shared<Vec2Command>(transform.scaling, scaling));
            }

        }); // end render Transform Component

        // Sprite Component
        RenderComponent<Sprite>(ICON_LC_IMAGE "  Sprite", entity, [io, FONT_BOLD, entity, this](Sprite& sprite)
        {
            auto& engine = InsightEngine::Instance();
            std::filesystem::path filepath(sprite.img.mFileName);
            ImTextureID texture_id;
            float texture_width;
            float texture_height;
            bool has_texture = sprite.img.texture_id;
            bool no_dimensions = sprite.img.width == 0 || sprite.img.height == 0;
            bool has_animation = !sprite.anims.empty();

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
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::BeginCombo("##TextureCombo", sprite.img.mFileName.c_str()))
                {
                    auto const asset = engine.GetSystem<AssetManager>("Asset");
                    for (auto const& [name, img] : asset->mImageList)
                    {
                        std::filesystem::path path(name);
                        std::string filename = path.filename().string();

                        if (ImGui::Selectable(filename.c_str(), filename == name))
                        {
                            sprite.img = img;
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndCombo(); // end combo TextureCombo
                }

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

                ImGui::EndTable(); // end table Sprite Table
            }

            // Use placeholder if width or height of texture
            if (no_dimensions)
            {
                texture_id = mEditorLayer.GetIcon("TexturePlaceholder");
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
                /*ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Body Shape");
                ImGui::PopFont();
                ImGui::TableNextColumn();*/
                //EditorUtils::RenderComboBoxEnum<BodyShape>("##Body Shape", rigidbody.mBodyShape, { "Box", "Circle", "Line" });

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

                // Static Friction
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Static Friction");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::SliderFloat("##Static Friction", &rigidbody.mStaticFriction, 0.f, 10.f);

                // Dynamic Friction
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Dynamic Friction");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::SliderFloat("##Dynamic Friction", &rigidbody.mDynamicFriction, 0.f, 10.f);

                ImGui::EndTable(); // end table RigidbodyTable
            }
        }); // end render Rigidbody Component

        RenderComponent<Collider>(ICON_LC_FLIP_HORIZONTAL_2 "  Collider", entity, [FONT_BOLD](Collider& collider)
        {
            bool box_enabled = collider.IsBoxColliderEnable();
            bool circle_enabled = collider.IsCircleColliderEnable();

            if (!(box_enabled && circle_enabled))
            {
                if (ImGui::Button("Add Collider"))
                {
                    ImGui::OpenPopup("Collider Popup");
                }
            }

            if (ImGui::BeginPopupContextItem("Collider Popup"))
            {
                if (!box_enabled)
                {
                    if (ImGui::MenuItem("Box Collider"))
                    {
                        collider.EnableBoxCollider();
                    }
                }

                if (!circle_enabled)
                {
                    if (ImGui::MenuItem("Circle Collider"))
                    {
                        collider.EnableCircleCollider();
                    }
                }

                ImGui::EndPopup();
            }

            ImGui::Spacing();

            if (box_enabled)
            {
                ImGui::SeparatorText("Box Collider");
                EditorUtils::RenderControlVec2("Offset", collider.mBoxCollider.offset);
                EditorUtils::RenderControlVec2("Scale", collider.mBoxCollider.sizeScale);
            }

            if (circle_enabled)
            {
                ImGui::SeparatorText("Circle Collider");
                EditorUtils::RenderControlVec2("Offset", collider.mCircleCollider.offset);
                if (ImGui::BeginTable("Circle Radius Scale Table", 2))
                {
                    ImGui::TableSetupColumn("Collider Table Column", ImGuiTableColumnFlags_WidthFixed, 100.f);
                    ImGui::TableNextColumn();
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::TextUnformatted("Radius Scale");
                    ImGui::PopFont();

                    ImGui::TableNextColumn();
                    ImGui::InputFloat("##Radius Scale", &collider.mCircleCollider.radiusScale);

                    ImGui::EndTable(); // end table Circle Radius Scale Table
                }
            }
        });

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

        // Audio Listener Component
        RenderComponent<AudioListener>(ICON_LC_EAR "  Audio Listener", entity, [entity, FONT_BOLD](AudioListener& listener)
        {
            if (ImGui::BeginTable("Audio Listener Table", 2))
            {
                // Set Table and its flags
                ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
                ImGui::TableSetupColumn("ListenerLabel", column_flags, 100.f);

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Volume");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                int volume = static_cast<int>(listener.volume * 100);
                if (ImGui::SliderInt("##Volume", &volume, 0, 100, "%d%%"))
                {
                    listener.volume = static_cast<float>(volume) / 100.f;
                }

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Pitch Correctness");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Pitch Correctness", &listener.pitch_correctness);

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Hearing Range");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Hearing Range", &listener.hearing_range);

                ImGui::EndTable(); // end table Audio Listener Table
            }
        }); // end render Audio Listener Component

        // Aduio Emitter Component
        RenderComponent<AudioEmitter>(ICON_LC_SPEAKER "  Audio Emitter", entity, [entity, FONT_BOLD](AudioEmitter& emitter)
        {
            if (ImGui::BeginTable("Audio Emitter Table", 2))
            {
                // Set Table and its flags
                ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
                ImGui::TableSetupColumn("EmitterLabel", column_flags, 100.f);

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Sound");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::BeginCombo("##SoundName", emitter.soundName.c_str()))
                {
                    auto& engine = InsightEngine::Instance();
                    auto const asset = engine.GetSystem<AssetManager>("Asset");

                    for (auto const& [sound_name, sound] : asset->mSoundList)
                    {
                        if (ImGui::Selectable(sound_name.c_str(), emitter.soundName == sound_name))
                        {
                            emitter.soundName = sound_name;
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndCombo(); // end combo SoundName
                }
                // Accept payload
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMPORTED_SOUND"))
                    {
                        std::filesystem::path path = static_cast<wchar_t*>(payload->Data);
                        emitter.soundName = path.string();
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Looped");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::Checkbox("##Looped", &emitter.isLoop);

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Falloff Factor");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Falloff Factor", &emitter.falloff_factor);

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Volume Level");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Volume Level", &emitter.volumeLevel);

                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Pitch");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Pitch", &emitter.pitch);

                ImGui::EndTable(); // end table Audio Emitter Table
            }
        }); // end render Audio Emitter component

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
    } // end AddAnimation()

    void InspectorPanel::RenderCameraControls()
    {
        auto& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
        const float SIZE = 16.f;
        auto const FONT_BOLD = ImGui::GetIO().Fonts->Fonts[FONT_TYPE_BOLD];
        auto& style = ImGui::GetStyle();
        float zoom_level = camera.GetZoomLevel();

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
        if (ImGui::BeginTable("CameraType", 2))
        {
            ImGui::TableSetupColumn("CameraTypeLabels", ImGuiTableColumnFlags_WidthFixed, 100.f);

            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Type");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(Camera3D::mActiveCamera == CAMERA_TYPE_EDITOR ? "Editor Camera" : "In-game Camera");

            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Projection");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Perspective");

            ImGui::EndTable(); // end camera type table
        }

        // Camera Position
        //Vector2D position = { camera.mPosition.x, camera.mPosition.y };
        //EditorUtils::RenderControlVec2("Position", position);
        Vector3D position = { camera.mPosition.x, camera.mPosition.y, camera.mPosition.z };
        EditorUtils::RenderControlVec3("Position", position);
        ImGui::SetItemTooltip("Adjust the position of the camera in world space");
        camera.SetPosition(position.x, position.y, position.z);

        if (ImGui::BeginTable("CameraTable", 2))
        {
            ImGui::TableSetupColumn("CameraLabels", ImGuiTableColumnFlags_WidthFixed, 100.f);

            // Camera Clipping Planes
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Clipping Planes");
            ImGui::PopFont();

            ImGui::TableNextColumn();
            if (ImGui::BeginTable("Clipping Planes", 2))
            {
                ImGui::TableSetupColumn("PlaneLabel", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("Near").x);

                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Near");
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Near", &camera.mNear);

                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Far");
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Far", &camera.mFar);

                ImGui::EndTable(); // end table Clipping Planes
            }

            // Camera Zoom
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Zoom");
            ImGui::PopFont();
            ImGui::SetItemTooltip("Adjust the zoom level of the camera");

            // Zoom out with - button
            ImGui::TableNextColumn();
            if (ImGui::Button(ICON_LC_ZOOM_OUT))
            {
                zoom_level *= (1 - Camera3D::mZoomSpeed);
                camera.SetZoomLevel(zoom_level);
            }
            ImGui::SetItemTooltip("Zooms out camera");

            // Slider to adjust camera zoom
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - (2 * SIZE + 3 * style.ItemSpacing.x));
            if (ImGui::SliderFloat("##CameraZoomSlider", &zoom_level,
                                   (Camera3D::CAMERA_ZOOM_MIN), (Camera3D::CAMERA_ZOOM_MAX), "%.2fx", ImGuiSliderFlags_Logarithmic))
            {
                // Update the camera's zoom level directly
                camera.SetZoomLevel(zoom_level);
            }

            // Zoom in with + button
            ImGui::SameLine();
            if (ImGui::Button(ICON_LC_ZOOM_IN))
            {
                zoom_level *= (1 + Camera::mZoomSpeed);
                camera.SetZoomLevel(zoom_level);
            }
            ImGui::SetItemTooltip("Zooms in camera");

            // Speed
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Speed");
            ImGui::PopFont();
            ImGui::SetItemTooltip("Adjust speed of camera");
            ImGui::TableNextColumn();
            if (ImGui::BeginTable("CameraSpeedTable", 2))
            {
                ImGui::TableSetupColumn("SpeedLabel", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("Zoom").x);

                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Zoom");
                ImGui::TableNextColumn();
                ImGui::SliderFloat("##CameraZoomSpeed", &Camera3D::mZoomSpeed, Camera3D::CAMERA_ZOOM_SPEED_MIN, Camera3D::CAMERA_ZOOM_SPEED_MAX, "%.2f");

                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Pan");
                ImGui::TableNextColumn();
                ImGui::SliderFloat("##CameraPanSpeed", &Camera3D::mMoveSpeed, Camera3D::CAMERA_MOVE_SPEED_MIN, Camera3D::CAMERA_MOVE_SPEED_MAX, "%.2f");

                ImGui::EndTable(); // end table CameraSpeedTable
            }

            ImGui::EndTable(); // end table CameraTable
        }

        ImGui::PopStyleVar(); // end style rounding

    } // end RenderCameraControls()

} // end namespace IS
