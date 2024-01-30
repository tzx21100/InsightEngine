/*!
 * \file InspectorPanel.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for class InspectorPanel,
 * which encapsulates the functionalities of an inspector panel which
 * manages an entity and its components
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
#ifdef USING_IMGUI
#include "InspectorPanel.h"

#include "Editor/Utils/FileUtils.h"
#include "Editor/Utils/EditorUtils.h"
#include "Editor/Layers/EditorLayer.h"
#include "Editor/Commands/CommandHistory.h"
#include "Engine/Systems/Button/Button.h"
#include "Engine/Systems/Audio/Audio.h"
#include "Engine/Systems/Asset/Asset.h"
#include "Engine/Systems/Category/Category.h"
#include "Engine/Systems/FSM/FSM.h"
#include "Physics/Dynamics/Body.h"
#include "Physics/Collision/Collider.h"
#include "Physics/System/CollisionSystem.h"
#include "Graphics/Core/Graphics.h"
#include "Graphics/System/Light.h"
#include "Graphics/System/Camera3D.h"

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
        static bool add_new_category = false;

        EditorUtils::RenderTableFixedWidth("Details", 2, [&]()
        {
            EditorUtils::RenderTableLabel("Name");

            // Edit Entity Name
            std::string& name = engine.GetEntityName(entity);
            EditorUtils::RenderTableInputText(name);

            ImGui::SameLine();

            if (ImGui::Button(ICON_LC_MORE_VERTICAL))
                ImGui::OpenPopup("Entity Options");

            if (ImGui::BeginPopup("Entity Options"))
            {
                mEditorLayer.RenderEntityConfig(entity);

                ImGui::EndPopup();
            }

            // Prefab combo
            EditorUtils::RenderTableLabel("Prefabs", "Pre-fabricated Game Objects.\n(i.e., Player, Platform, Background, etc.)");
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(100.f);
            auto const asset = engine.GetSystem<AssetManager>("Asset");
            auto const& prefab_list = asset->mPrefabList;
            bool is_prefab = prefab_list.find(name) != prefab_list.end();
            if (ImGui::BeginCombo("##Prefabs", is_prefab ? name.c_str() : "None"))
            {
                for (auto const& [prefab_name, prefab] : prefab_list)
                {
                    if (ImGui::Selectable(prefab_name.c_str(), name == prefab_name))
                    {
                        engine.LoadFromPrefab(prefab, entity);
                    }
                }
                ImGui::EndCombo(); // end combo Prefabs
            }

            // Save Prefab
            ImGui::SameLine();
            if (ImGui::Button("Save as prefab"))
            {
                engine.SaveAsPrefab(entity, name);
            }

            // Category
            EditorUtils::RenderTableLabel("Category", "Categorizes Entities.\n(i.e., Player, Platform, Background, etc.)");
            ImGui::TableNextColumn();
            bool has_category = engine.HasComponent<Category>(entity);
            std::string entity_category = has_category ? engine.GetComponent<Category>(entity).mCategory : "";
            if (ImGui::BeginCombo("##Category", has_category ? entity_category.c_str() : "None"))
            {
                auto const& category_system = engine.GetSystem<CategorySystem>("CategorySystem");
                for (std::string const& category : category_system->mCategories)
                {
                    if (ImGui::Selectable(category.c_str(), entity_category == category))
                    {
                        if (category != CategorySystem::ADD_NEW_CATEGORY)
                        {
                            if (!engine.HasComponent<Category>(entity))
                            {
                                engine.AddComponent<Category>(entity, Category());
                            }
                            Category& cat = engine.GetComponent<Category>(entity);
                            cat.mCategory = category;
                            continue;
                        }
                        add_new_category = true;
                    }
                }
                ImGui::EndCombo();
            }
        });

        ImGui::Spacing();

        if (add_new_category)
        {
            ImGui::OpenPopup("Add New Category");
            if (ImGui::BeginPopupModal("Add New Category", &add_new_category))
            {
                EditorUtils::RenderTableFixedWidth("Details", 2, [&]()
                {
                    if (!engine.HasComponent<Category>(entity))
                    {
                        engine.AddComponent<Category>(entity, Category());
                    }
                    std::string& category = engine.GetComponent<Category>(entity).mCategory;

                    EditorUtils::RenderTableLabel("New Category");
                    EditorUtils::RenderTableInputText(category, []()
                    {
                        add_new_category = false;
                    });
                    ImGui::SetItemTooltip("Press \"Enter\" to add and save category.");
                });
                ImGui::EndPopup();
            }
        }
        else
        {
            if (engine.HasComponent<Category>(entity))
            {
                std::string const& category = engine.GetComponent<Category>(entity).mCategory;
                if (category.empty())
                {
                    engine.RemoveComponent<Category>(entity);
                }
            }
        }

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
            if (Vector2D position = transform.world_position; EditorUtils::RenderControlVec2("Translation", position))
            {
                CommandHistory::AddCommand<ChangeCommand<Vector2D>>(transform.world_position, position);
            }

            // Render Rotation
            EditorUtils::RenderTableFixedWidth("TransformRotation", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Rotation");
                ImGui::TableNextColumn();

                // Apply modification
                if (float rotation = transform.rotation; ImGui::DragFloat("##Rotation", &rotation, 1.f, 0.f, 360.f, "%.f deg"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(transform.rotation, rotation);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
            });

            // Render Scale
            if (Vector2D scaling = transform.scaling; EditorUtils::RenderControlVec2("Scale", scaling, 128.f, 128.f))
            {
                scaling = { abs(scaling.x), abs(scaling.y) };
                CommandHistory::AddCommand<ChangeCommand<Vector2D>>(transform.scaling, scaling);
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

            EditorUtils::RenderTableFixedWidth("Sprite Table", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Rendering");

                ImGui::TableNextColumn();
                EditorUtils::RenderToggleButton("Rendering", sprite.toRender);

                EditorUtils::RenderTableLabel("Layer");

                ImGui::TableNextColumn();
                if (ImGui::BeginCombo("##Layer", ISGraphics::mLayers[sprite.layer].mName.c_str()))
                {
                    for (int i{ static_cast<int>(ISGraphics::mLayers.size() - 1) }; i >= 0; --i)
                    {
                        if (ImGui::Selectable(ISGraphics::mLayers[i].mName.c_str(), sprite.layer == i))
                        {
                            //sprite.layer = i;
                            ISGraphics::ChangeEntityLayer(sprite.layer, i, entity);
                        }
                    }
                    ImGui::EndCombo();
                }

                EditorUtils::RenderTableLabel("Color");

                ImGui::TableNextColumn();
                if (glm::vec4 color = sprite.color; ImGui::ColorEdit4("##Color", &color[0]))
                {
                    CommandHistory::AddCommand<ChangeCommand<glm::vec4>>(sprite.color, color);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
                ImGui::Spacing();

                EditorUtils::RenderTableLabel("Texture");

                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::BeginCombo("##TextureCombo", sprite.img.mFileName.c_str()))
                {
                    auto const asset = engine.GetSystem<AssetManager>("Asset");
                    for (auto const& [name, img] : asset->mImageList)
                    {
                        std::filesystem::path path(name);
                        std::string filename = path.filename().string();

                        if (ImGui::Selectable(filename.c_str(), filename == sprite.img.mFileName))
                        {
                            CommandHistory::AddCommand<ChangeCommand<Image>>(sprite.img, img);
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
                        Image const& img = *asset->GetImage(path.string());
                        CommandHistory::AddCommand<ChangeCommand<Image>>(sprite.img, img);
                    }
                    ImGui::EndDragDropTarget();
                }
            });

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
                        Image const& img = *asset->GetImage(path.string());
                        CommandHistory::AddCommand<ChangeCommand<Image>>(sprite.img, img);
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
                    float zoom = 5.f;
                    float zoomed_in_height = region_size * zoom;
                    float zoomed_in_width = zoomed_in_height * texture_aspect_ratio;
                    ImGui::Image(texture_id, ImVec2(zoomed_in_width, zoomed_in_height)); // still WIP

                    ImGui::EndTooltip();
                }
            }
            ImGui::EndChild(); // end child window Texture Preview

            // Texture Dimensions
            EditorUtils::RenderTableFixedWidth("Texture", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Width");

                ImGui::TableNextColumn();
                ImGui::Text("%d px", sprite.img.width);

                EditorUtils::RenderTableLabel("Height");

                ImGui::TableNextColumn();
                ImGui::Text("%d px", sprite.img.height);
            });

            if (has_texture)
            {
                if (ImGui::Button("Add Animation"))
                {
                    mShowAddAnimation = true;
                }                    
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

                    EditorUtils::RenderTable("Animations Table", 1, [&]()
                    {
                        for (int i{}; i < static_cast<int>(sprite.anims.size()); ++i)
                        {
                            Animation& animation = sprite.anims[i];
                            ImGui::TableNextColumn();
                            ImGui::PushID(i);

                            ImGui::Separator();
                            EditorUtils::RenderTableFixedWidth("Animation Data", 2, [&]()
                            {
                                EditorUtils::RenderTableLabel("Name");

                                EditorUtils::RenderTableInputText(animation.name);

                                EditorUtils::RenderTableLabel("Duration");
                                ImGui::TableNextColumn();

                                if (float animation_duration = animation.animation_duration; ImGui::DragFloat("##Duration", &animation_duration, 0.1f, 0.f, 20.f, "%.2f sec"))
                                {
                                    CommandHistory::AddCommand<ChangeCommand<float>>(animation.animation_duration, animation_duration);
                                }
                                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                                EditorUtils::RenderTableLabel("Columns");
                                ImGui::TableNextColumn();
                                if (int x_frames = animation.x_frames; ImGui::InputInt("##Columns", &x_frames))
                                {
                                    x_frames = std::max(x_frames, 1);
                                    CommandHistory::AddCommand<ChangeCommand<int>>(animation.x_frames, x_frames);
                                }

                                EditorUtils::RenderTableLabel("Rows");
                                ImGui::TableNextColumn();                                
                                if (int y_frames = animation.y_frames; ImGui::InputInt("##Rows", &y_frames))
                                {
                                    y_frames = std::max(y_frames, 1);
                                    CommandHistory::AddCommand<ChangeCommand<int>>(animation.y_frames, y_frames);
                                }
                            });
                            ImGui::PopID();
                        }
                    });

                    ImGui::TreePop(); // end tree Animation
                }
            }

            if (mShowAddAnimation)
            {
                AddAnimation(sprite);
            }                

        }); // end render Sprite Component

        // Rigidbody Component
        RenderComponent<RigidBody>(ICON_LC_PERSON_STANDING "  Rigidbody", entity, [entity, FONT_BOLD](RigidBody& rigidbody)
        {
            if (Vector2D velocity = rigidbody.mVelocity; EditorUtils::RenderControlVec2("Velocity", rigidbody.mVelocity))
            {
                CommandHistory::AddCommand<ChangeCommand<Vector2D>>(rigidbody.mVelocity, velocity);
            }
            
            if (Vector2D force = rigidbody.mForce; EditorUtils::RenderControlVec2("Force", rigidbody.mForce))
            {
                CommandHistory::AddCommand<ChangeCommand<Vector2D>>(rigidbody.mForce, force);
            }

            // Render Other Component Attributes
            EditorUtils::RenderTableFixedWidth("Rigidbody Table", 2, [&]()
            {
                // Angular Velocity
                EditorUtils::RenderTableLabel("Angular Velocity");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                                
                if (float angular_velocity = rigidbody.mAngularVelocity; ImGui::DragFloat("##AngularVelocity", &angular_velocity, 1.f, 0.f, 0.f, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(rigidbody.mAngularVelocity, angular_velocity);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
                ImGui::PopItemWidth();

                // Body Type
                EditorUtils::RenderTableLabel("Body Type");
                ImGui::TableNextColumn();
                EditorUtils::RenderComboBoxEnum<BodyType>("##Body Type", rigidbody.mBodyType, { "Static", "Dynamic", "Kinematic","Ghost" , "WallClimb", "Spikes", "Ignore", "Stone" , "Grass" });

                if (rigidbody.mBodyType == BodyType::Dynamic)
                {
                    EditorUtils::RenderTableLabel("Gravity Scale");
                    ImGui::TableNextColumn();
                    ImGui::PushItemWidth(80.f);
                    if (float gravity_scale = rigidbody.mGravityScale; ImGui::DragFloat("##Gravity Scale", &gravity_scale, 1.f, 0.f, 0.f, "%.2f"))
                    {
                        CommandHistory::AddCommand<ChangeCommand<float>>(rigidbody.mGravityScale, gravity_scale);
                    }
                    CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
                    ImGui::PopItemWidth();
                }

                // Mass
                EditorUtils::RenderTableLabel("Mass");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                if (float mass = rigidbody.mMass; ImGui::DragFloat("##Mass", &mass, 1.f, 0.f, 0.f, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(rigidbody.mMass, mass);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
                ImGui::PopItemWidth();

                // Inverse Mass
                EditorUtils::RenderTableLabel("Inverse Mass");
                ImGui::TableNextColumn();
                ImGui::Text("%.2f", &rigidbody.mInvMass);

                // Inertia
                EditorUtils::RenderTableLabel("Inertia");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                if (float inertia = rigidbody.mInertia; ImGui::DragFloat("##Inertia", &inertia, 1.f, 0.f, 0.f, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(rigidbody.mInertia, inertia);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
                ImGui::PopItemWidth();

                // Restitution
                EditorUtils::RenderTableLabel("Restitution");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                if (float restitution = rigidbody.mRestitution; ImGui::DragFloat("##Restitution", &restitution, 1.f, 0.f, 0.f, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(rigidbody.mRestitution, restitution);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
                ImGui::PopItemWidth();

                // Density
                EditorUtils::RenderTableLabel("Density");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::Text("%.2f", rigidbody.mDensity);
                ImGui::PopItemWidth();

                // Static Friction
                EditorUtils::RenderTableLabel("Static Friction");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                if (float static_friction = rigidbody.mStaticFriction; ImGui::DragFloat("##Static Friction", &static_friction, 1.f, 0.f, 0.f, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(rigidbody.mStaticFriction, static_friction);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
                ImGui::PopItemWidth();

                // Dynamic Friction
                EditorUtils::RenderTableLabel("Dynamic Friction");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                if (float dynamic_friction = rigidbody.mDynamicFriction; ImGui::DragFloat("##Dynamic Friction", &dynamic_friction, 1.f, 0.f, 0.f, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(rigidbody.mDynamicFriction, dynamic_friction);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
                ImGui::PopItemWidth();
            });

        }); // end render Rigidbody Component

        // Collider Component
        RenderComponent<Collider>(ICON_LC_FLIP_HORIZONTAL_2 "  Collider", entity, [FONT_BOLD](Collider& collider)
        {
            bool box_enabled = collider.IsBoxColliderEnable();
            bool circle_enabled = collider.IsCircleColliderEnable();
            bool all_enabled = box_enabled && circle_enabled;

            if (ImGui::RadioButton("All", all_enabled))
            {
                all_enabled ? collider.DisableAllColliders() : collider.EnableAllColliders();
            }

            ImGui::SameLine();

            if (ImGui::RadioButton("Box", box_enabled))
            {
                box_enabled ? collider.DisableBoxCollider() : collider.EnableBoxCollider();
            }

            ImGui::SameLine();

            if (ImGui::RadioButton("Circle", circle_enabled))
            {
                circle_enabled ? collider.DisableCircleCollider() : collider.EnableCircleCollider();
            }

            ImGui::Spacing();

            if (box_enabled)
            {
                ImGui::SeparatorText(ICON_LC_BOX_SELECT "  Box Collider");
                if (Vector2D offset = collider.mBoxCollider.offset; EditorUtils::RenderControlVec2("Offset", offset))
                {
                    CommandHistory::AddCommand<ChangeCommand<Vector2D>>(collider.mBoxCollider.offset, offset);
                }

                if (Vector2D scale = collider.mBoxCollider.sizeScale; EditorUtils::RenderControlVec2("Scale", scale))
                {
                    CommandHistory::AddCommand<ChangeCommand<Vector2D>>(collider.mBoxCollider.sizeScale, scale);
                }
            }
            
            if (circle_enabled)
            {
                ImGui::SeparatorText(ICON_LC_CIRCLE_DASHED "  Circle Collider");
                if (Vector2D offset = collider.mCircleCollider.offset; EditorUtils::RenderControlVec2("Offset", offset))
                {
                    CommandHistory::AddCommand<ChangeCommand<Vector2D>>(collider.mCircleCollider.offset, offset);
                }

                EditorUtils::RenderTableFixedWidth("Circle Radius Scale Table", 2, [&]()
                {
                    EditorUtils::RenderTableLabel("Radius Scale");

                    ImGui::TableNextColumn();
                    ImGui::PushItemWidth(80.f);
                    if (float scale = collider.mCircleCollider.radiusScale; ImGui::DragFloat("##Radius Scale", &scale))
                    {
                        CommandHistory::AddCommand<ChangeCommand<float>>(collider.mCircleCollider.radiusScale, scale);
                    }
                    CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
                    ImGui::PopItemWidth();
                });
            }

        }); // end render Collider Component

        // Script Component
        RenderComponent<ScriptComponent>(ICON_LC_BRACES "  Script", entity, [FONT_BOLD](ScriptComponent& script)
        {
            EditorUtils::RenderTableFixedWidth("ScriptTable", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Script Name:");
                ImGui::TableNextColumn();
                if (ImGui::BeginCombo("##Scripts", script.mScriptName.c_str()))
                {
                    auto const asset = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
                    for (std::string const& name : asset->mScriptList)
                    {
                        std::filesystem::path path(name);
                        std::string stem = path.stem().string();
                        if (ImGui::Selectable(stem.c_str(), script.mScriptName == stem))
                        {
                            CommandHistory::AddCommand<ChangeCommand<std::string>>(script.mScriptName, stem);
                        }
                    }

                    ImGui::EndCombo();
                }

                // Accept file drop
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMPORTED_SCRIPT"))
                    {
                        std::filesystem::path path = static_cast<wchar_t*>(payload->Data);
                        CommandHistory::AddCommand<ChangeCommand<std::string>>(script.mScriptName, path.stem().string());
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::SameLine();

                // Modify Script
                if (ImGui::Button(ICON_LC_FILE_SEARCH))
                {
                    if (std::filesystem::path filepath(FileUtils::OpenAndGetScript()); !filepath.empty())
                    {
                        CommandHistory::AddCommand<ChangeCommand<std::string>>(script.mScriptName, filepath.stem().string());
                    }
                }
                ImGui::SetItemTooltip("Browse and replace the existing script");

                ImGui::SameLine();

                // Edit Script
                if (ImGui::Button(ICON_LC_FILE_EDIT))
                {
                    InsightEngine::Instance().OpenGameScript(script.mScriptName + ".cs");
                }
                ImGui::SetItemTooltip("Open and edit script in default application");
            });

        }); // end render Script Component

        // State Component
        RenderComponent<StateComponent>(ICON_LC_FLAG "  State", entity, [](StateComponent& state)
        {
            EditorUtils::RenderTableFixedWidth("State Table", 2, [&]()
            {
                // Current State
                EditorUtils::RenderTableLabel("Current State:");
                ImGui::TableNextColumn();
                if (ImGui::BeginCombo("##CurrentStates", state.mCurrentState.mScriptName.c_str()))
                {
                    auto const asset = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
                    for (std::string const& name : asset->mScriptList)
                    {
                        std::filesystem::path path(name);
                        std::string stem = path.stem().string();
                        if (ImGui::Selectable(stem.c_str(), state.mCurrentState.mScriptName == stem))
                        {
                            state.mCurrentState.SetSimpleState(stem);
                        }
                    }

                    ImGui::EndCombo();
                }

                // Accept file drop
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMPORTED_SCRIPT"))
                    {
                        std::filesystem::path path = static_cast<wchar_t*>(payload->Data);
                        std::string stem = path.stem().string();
                        state.mCurrentState.SetSimpleState(stem);
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::SameLine();

                // Modify State Script
                if (ImGui::Button(ICON_LC_FILE_SEARCH))
                {
                    if (std::filesystem::path filepath(FileUtils::OpenAndGetScript()); !filepath.empty())
                    {
                        std::string stem = filepath.stem().string();
                        state.mCurrentState.SetSimpleState(stem);
                    }
                }
                ImGui::SetItemTooltip("Browse and replace the current state.");

                ImGui::SameLine();

                // Edit State Script
                if (ImGui::Button(ICON_LC_FILE_EDIT))
                {
                    InsightEngine::Instance().OpenGameScript(state.mCurrentState.mScriptName + ".cs");
                }
                ImGui::SetItemTooltip("Open and edit the current state in default application");
            });

            //--------------------------------------------------------------------------------
            // Add Condition
            //--------------------------------------------------------------------------------
            ImGuiTreeNodeFlags condition_tree_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
            bool condition_opened = ImGui::TreeNodeEx(ICON_LC_CHECK_CIRCLE  "  Condition", condition_tree_flags);
            if (condition_opened)
            {
                EditorUtils::RenderTableFixedWidth("ConditionTable", 2, [&]()
                {
                    //--------------------------------------------------------------------------------
                    // Condition
                    //--------------------------------------------------------------------------------
                    EditorUtils::RenderTableLabel("State Condition:");
                    ImGui::TableNextColumn();
                    if (ImGui::BeginCombo("##StateCondition", state.mEntityConditions.GetCondition().mScriptName.c_str()))
                    {
                        auto const asset = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
                        for (std::string const& name : asset->mScriptList)
                        {
                            std::filesystem::path path(name);
                            std::string stem = path.stem().string();
                            if (ImGui::Selectable(stem.c_str(), state.mEntityConditions.GetCondition().mScriptName == stem))
                            {
                                state.mEntityConditions.AddCondition(stem);
                            }
                        }

                        ImGui::EndCombo();
                    }

                    // Accept file drop
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMPORTED_SCRIPT"))
                        {
                            std::filesystem::path path = static_cast<wchar_t*>(payload->Data);
                            std::string stem = path.stem().string();
                            state.mEntityConditions.AddCondition(stem);
                        }
                        ImGui::EndDragDropTarget();
                    }

                    ImGui::SameLine();

                    // Modify State Condition
                    if (ImGui::Button(ICON_LC_FILE_SEARCH))
                    {
                        if (std::filesystem::path filepath(FileUtils::OpenAndGetScript()); !filepath.empty())
                        {
                            std::string stem = filepath.stem().string();
                            state.mEntityConditions.AddCondition(stem);
                        }
                    }
                    ImGui::SetItemTooltip("Browse and replace the state condition.");

                    ImGui::SameLine();

                    // Edit State Script
                    if (ImGui::Button(ICON_LC_FILE_EDIT))
                    {
                        InsightEngine::Instance().OpenGameScript(state.mEntityConditions.GetCondition().mScriptName + ".cs");
                    }
                    ImGui::SetItemTooltip("Open and edit the state condition in default application");

                    //--------------------------------------------------------------------------------
                    // Current State
                    //--------------------------------------------------------------------------------
                    EditorUtils::RenderTableLabel("Current State:");
                    ImGui::TableNextColumn();
                    if (ImGui::BeginCombo("##CurrentState", state.mCurrentState.mScriptName.c_str()))
                    {
                        auto const asset = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
                        for (std::string const& name : asset->mScriptList)
                        {
                            std::filesystem::path path(name);
                            std::string stem = path.stem().string();
                            if (ImGui::Selectable(stem.c_str(), state.mEntityConditions.GetCurrentState().mScriptName == stem))
                            {
                                SimpleState temp = CreateSimpleState(stem);
                                state.mEntityConditions.SetCurrentState(temp);
                            }
                        }

                        ImGui::EndCombo();
                    }

                    // Accept file drop
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMPORTED_SCRIPT"))
                        {
                            std::filesystem::path path = static_cast<wchar_t*>(payload->Data);
                            std::string stem = path.stem().string();
                            SimpleState temp = CreateSimpleState(stem);
                            state.mEntityConditions.SetCurrentState(temp);
                        }
                        ImGui::EndDragDropTarget();
                    }

                    ImGui::SameLine();

                    // Modify State Condition
                    if (ImGui::Button(ICON_LC_FILE_SEARCH))
                    {
                        if (std::filesystem::path filepath(FileUtils::OpenAndGetScript()); !filepath.empty())
                        {
                            std::string stem = filepath.stem().string();
                            SimpleState temp = CreateSimpleState(stem);
                            state.mEntityConditions.SetCurrentState(temp);
                        }
                    }
                    ImGui::SetItemTooltip("Browse and replace the state condition current state.");

                    ImGui::SameLine();

                    // Edit State Script
                    if (ImGui::Button(ICON_LC_FILE_EDIT))
                    {
                        InsightEngine::Instance().OpenGameScript(state.mEntityConditions.GetCurrentState().mScriptName + ".cs");
                    }
                    ImGui::SetItemTooltip("Open and edit the state condition current state in default application");

                    //--------------------------------------------------------------------------------
                    // Target State
                    //--------------------------------------------------------------------------------
                    EditorUtils::RenderTableLabel("Target State:");
                    ImGui::TableNextColumn();
                    if (ImGui::BeginCombo("##TargetState", state.mEntityConditions.GetTargetState().mScriptName.c_str()))
                    {
                        auto const asset = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
                        for (std::string const& name : asset->mScriptList)
                        {
                            std::filesystem::path path(name);
                            std::string stem = path.stem().string();
                            if (ImGui::Selectable(stem.c_str(), state.mEntityConditions.GetCondition().mScriptName == stem))
                            {
                                SimpleState temp = CreateSimpleState(stem);
                                state.mEntityConditions.SetTargetState(temp);
                            }
                        }

                        ImGui::EndCombo();
                    }

                    // Accept file drop
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMPORTED_SCRIPT"))
                        {
                            std::filesystem::path path = static_cast<wchar_t*>(payload->Data);
                            std::string stem = path.stem().string();
                            SimpleState temp = CreateSimpleState(stem);
                            state.mEntityConditions.SetTargetState(temp);
                        }
                        ImGui::EndDragDropTarget();
                    }

                    ImGui::SameLine();

                    // Modify State Condition
                    if (ImGui::Button(ICON_LC_FILE_SEARCH))
                    {
                        if (std::filesystem::path filepath(FileUtils::OpenAndGetScript()); !filepath.empty())
                        {
                            std::string stem = filepath.stem().string();
                            SimpleState temp = CreateSimpleState(stem);
                            state.mEntityConditions.SetTargetState(temp);
                        }
                    }
                    ImGui::SetItemTooltip("Browse and replace the state condition target state.");

                    ImGui::SameLine();

                    // Edit State Script
                    if (ImGui::Button(ICON_LC_FILE_EDIT))
                    {
                        InsightEngine::Instance().OpenGameScript(state.mEntityConditions.GetTargetState().mScriptName + ".cs");
                    }
                    ImGui::SetItemTooltip("Open and edit the state condition target state in default application");
                });

                ImGui::TreePop();
            }

        }); // end render Script Component

        // Light Component
        RenderComponent<Light>(ICON_LC_SUN "  Light", entity, [](Light& light)
        {
            EditorUtils::RenderTableFixedWidth("Light Render Table", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Rendering");
                ImGui::TableNextColumn();
                EditorUtils::RenderToggleButton("Render Light", light.mRender);
            });

            if (Vector2D offset = light.mOffset; EditorUtils::RenderControlVec2("Offset", offset))
            {
                CommandHistory::AddCommand<ChangeCommand<Vector2D>>(light.mOffset, offset);
            }

            EditorUtils::RenderTableFixedWidth("Light Table", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Hue");
                ImGui::TableNextColumn();
                if (Vector3D color = light.mHue; ImGui::ColorEdit3("##LightHue", &color.x))
                {
                    CommandHistory::AddCommand<ChangeCommand<Vector3D>>(light.mHue, color);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                EditorUtils::RenderTableLabel("Intensity");
                ImGui::TableNextColumn();
                if (int percent = static_cast<int>(light.mIntensity * 100.f); ImGui::SliderInt("##LightIntensity", &percent, 0, 100, "%d%%"))
                {
                    float intensity = static_cast<float>(percent) / 100.f;
                    CommandHistory::AddCommand<ChangeCommand<float>>(light.mIntensity, intensity);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                EditorUtils::RenderTableLabel("Size");
                ImGui::TableNextColumn();
                if (float size = light.mSize; ImGui::DragFloat("##LightSize", &size))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(light.mSize, size);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
            });
            
        });

        // Audio Listener Component
        RenderComponent<AudioListener>(ICON_LC_EAR "  Audio Listener", entity, [entity, FONT_BOLD](AudioListener& listener)
        {
            EditorUtils::RenderTableFixedWidth("Audio Listener Table", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Volume");
                ImGui::TableNextColumn();

                if (int volume = static_cast<int>(listener.volume * 100); ImGui::SliderInt("##Volume", &volume, 0, 100, "%d%%"))
                {
                    float percent = static_cast<float>(volume) / 100.f;
                    CommandHistory::AddCommand<ChangeCommand<float>>(listener.volume, percent);
                }

                EditorUtils::RenderTableLabel("Pitch Correctness");
                ImGui::TableNextColumn();

                if (float correctness = listener.pitch_correctness; ImGui::DragFloat("##Pitch Correctness", &correctness, 1.f, 0.f, 0.f, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(listener.pitch_correctness, correctness);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                EditorUtils::RenderTableLabel("Hearing Range");
                ImGui::TableNextColumn();

                if (float range = listener.hearing_range; ImGui::DragFloat("##Hearing Range", &range, 1.f, 0.f, 0.f, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(listener.hearing_range, range);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
            });

        }); // end render Audio Listener Component

        // Aduio Emitter Component
        RenderComponent<AudioEmitter>(ICON_LC_SPEAKER "  Audio Emitter", entity, [entity, FONT_BOLD](AudioEmitter& emitter)
        {
            EditorUtils::RenderTableFixedWidth("Audio Emitter Table", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Sound");
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
                            CommandHistory::AddCommand<ChangeCommand<std::string>>(emitter.soundName, sound_name);
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
                        CommandHistory::AddCommand<ChangeCommand<std::string>>(emitter.soundName, path.string());
                    }
                    ImGui::EndDragDropTarget();
                }

                EditorUtils::RenderTableLabel("Looped");
                ImGui::TableNextColumn();
                EditorUtils::RenderToggleButton("Looped", emitter.isLoop);

                EditorUtils::RenderTableLabel("Falloff Factor");
                ImGui::TableNextColumn();

                if (float factor = emitter.falloff_factor; ImGui::DragFloat("##Falloff Factor", &factor, 1.f, 0.f, 0.f, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(emitter.falloff_factor, factor);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                EditorUtils::RenderTableLabel("Volume Level");
                ImGui::TableNextColumn();

                if (int volume_level = static_cast<int>(emitter.volumeLevel * 100); ImGui::SliderInt("##VolumeLevel", &volume_level, 0, 100, "%d%%"))
                {                    
                    float volume = static_cast<float>(volume_level) / 100.f;
                    CommandHistory::AddCommand<ChangeCommand<float>>(emitter.volumeLevel, volume);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                EditorUtils::RenderTableLabel("Pitch");
                ImGui::TableNextColumn();

                if (float pitch = emitter.pitch; ImGui::DragFloat("##Pitch", &pitch, 1.f, 0.f, 0.f, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(emitter.pitch, pitch);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
            });

        }); // end render Audio Emitter component

        // Button Component
        RenderComponent<ButtonComponent>(ICON_LC_SQUARE "  Button", entity, [entity, FONT_BOLD](ButtonComponent& button)
        {
            EditorUtils::RenderTableFixedWidth("Button Table", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Text");
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

                // Edit button text
                EditorUtils::RenderTableInputText(button.mButtonText);

                EditorUtils::RenderTableLabel("Idle Alpha");
                ImGui::TableNextColumn();
                if (float idle_alpha = button.mIdleAlpha; ImGui::DragFloat("##IdleAlpha", &idle_alpha))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(button.mIdleAlpha, idle_alpha);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                EditorUtils::RenderTableLabel("Hover Alpha");
                ImGui::TableNextColumn();
                if (float hover_alpha = button.mHoverAlpha; ImGui::DragFloat("##HoverAlpha", &hover_alpha))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(button.mHoverAlpha, hover_alpha);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                EditorUtils::RenderTableLabel("Click Alpha");
                ImGui::TableNextColumn();
                if (float click_alpha = button.mClickAlpha; ImGui::DragFloat("##ClickAlpha", &click_alpha))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(button.mClickAlpha, click_alpha);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                EditorUtils::RenderTableLabel("Idle Scale");
                ImGui::TableNextColumn();
                if (float idle_scale = button.mIdleScale; ImGui::DragFloat("##IdleScale", &idle_scale))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(button.mIdleScale, idle_scale);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                EditorUtils::RenderTableLabel("Hovered Scale");
                ImGui::TableNextColumn();
                if (float hover_scale = button.mHoverScale; ImGui::DragFloat("##HoverScale", &hover_scale))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(button.mHoverScale, hover_scale);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());
            });

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
            bool vertical_scroll_visible = ImGui::GetScrollMaxY() != 0;
            float x_offset = ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_LC_MORE_VERTICAL).x - (vertical_scroll_visible ? 3.f : 1.f) * style.ItemSpacing.x;

            // Display Component Config
            ImGui::SameLine(x_offset);

            if (ImGui::Button(ICON_LC_MORE_VERTICAL))
            {
                ImGui::OpenPopup("ComponentConfig");
            }

            // Set Remove Component
            bool remove_component = false;
            if (ImGui::BeginPopup("ComponentConfig"))
            {
                if (ImGui::MenuItem("Remove Component"))
                {
                    remove_component = true;
                }
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
            {
                CommandHistory::AddCommand<RemoveComponentCommand<Component>>(entity);
            }
        }

    } // end RenderComponent()

    void InspectorPanel::AddAnimation(Sprite& sprite)
    {
        ImGui::OpenPopup("Add Animation");

        if (ImGui::BeginPopupModal("Add Animation", &mShowAddAnimation, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static char name[256]{};
            static int grid[2]{};
            static float time{};
            static bool show_warning = false;

            EditorUtils::RenderTableFixedWidth("Add Animation Table", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Name");

                ImGui::TableNextColumn();
                ImGui::InputText("##Animation Name", name, sizeof(name));

                EditorUtils::RenderTableLabel("Columns/Rows");

                ImGui::TableNextColumn();
                ImGui::InputInt2("##Animation Grid", grid);

                EditorUtils::RenderTableLabel("Time");

                ImGui::TableNextColumn();
                ImGui::InputFloat("##Animation Time", &time);

                if (ImGui::Button("Add"))
                {
                    if (strcmp(name, "") == 0 || grid[0] == 0 || grid[1] == 0 || time == 0.f)
                    {
                        show_warning = true;
                    } else
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
            });
            ImGui::EndPopup();
        }
    } // end AddAnimation()

    void InspectorPanel::RenderCameraControls()
    {
        auto& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
        const float SIZE = 16.f;
        auto& style = ImGui::GetStyle();
        float zoom_level = camera.GetZoomLevel();

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
        EditorUtils::RenderTableFixedWidth("CameraType", 2, [&]()
        {
            EditorUtils::RenderTableLabel("Type");
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(Camera3D::mActiveCamera == CAMERA_TYPE_EDITOR ? "Editor Camera" : "In-game Camera");

            EditorUtils::RenderTableLabel("Projection");
            ImGui::TableNextColumn();
            EditorUtils::RenderComboBoxEnum<aCameraProjection>("##Projection", Camera3D::mProjectionType, { "Orthographic", "Perspective" });
        });

        // Camera Position
        Vector3D position = { camera.mPosition.x, camera.mPosition.y, camera.mPosition.z };
        if (EditorUtils::RenderControlVec3("Position", position))
        {
            camera.SetPosition(position.x, position.y, position.z);
        }
        ImGui::SetItemTooltip("Adjust the position of the camera in world space");

        EditorUtils::RenderTableFixedWidth("CameraTable", 2, [&]()
        {
            // Camera Clipping Planes
            EditorUtils::RenderTableLabel("Clipping Planes");

            ImGui::TableNextColumn();
            EditorUtils::RenderTableFixedWidth("Clipping Planes", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Near");
                ImGui::TableNextColumn();
                if (float near = camera.mNear; ImGui::DragFloat("##Near", &near))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(camera.mNear, near);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                EditorUtils::RenderTableLabel("Far");
                ImGui::TableNextColumn();
                if (float far = camera.mFar; ImGui::DragFloat("##Far", &far))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(camera.mFar, far);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

            }, 0, ImGui::CalcTextSize("Near").x);

            // Camera Zoom
            EditorUtils::RenderTableLabel("Zoom", "Adjust the zoom level of the camera");

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
            EditorUtils::RenderTableLabel("Speed", "Adjust speed of camera");
            ImGui::TableNextColumn();
            EditorUtils::RenderTableFixedWidth("CameraSpeedTable", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Zoom");
                ImGui::TableNextColumn();
                if (float zoom = Camera3D::mZoomSpeed; ImGui::DragFloat("##CameraZoomSpeed", &zoom, 0.01f, Camera3D::CAMERA_ZOOM_SPEED_MIN, Camera3D::CAMERA_ZOOM_SPEED_MAX, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(Camera3D::mZoomSpeed, zoom);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

                EditorUtils::RenderTableLabel("Pan");
                ImGui::TableNextColumn();
                if (float pan = Camera3D::mMoveSpeed; ImGui::DragFloat("##CameraPanSpeed", &pan, 1.f, Camera3D::CAMERA_MOVE_SPEED_MIN, Camera3D::CAMERA_MOVE_SPEED_MAX, "%.2f"))
                {
                    CommandHistory::AddCommand<ChangeCommand<float>>(Camera3D::mMoveSpeed, pan);
                }
                CommandHistory::SetNoMergeMostRecent(ImGui::IsItemDeactivatedAfterEdit());

            }, 0, ImGui::CalcTextSize("Zoom").x);
        });

        if (ImGui::Button("Reset Camera"))
        {
            camera.Reset();
        }

        ImGui::PopStyleVar(); // end style rounding

    } // end RenderCameraControls()

} // end namespace IS

#endif // USING_IMGUI
