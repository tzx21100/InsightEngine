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
#ifdef USING_IMGUI
#include "InspectorPanel.h"

#include "Body.h"
#include "Collider.h"
#include "CollisionSystem.h"
#include "EditorUtils.h"
#include "EditorLayer.h"
#include "FileUtils.h"
#include "GameGui.h"
#include "Audio.h"
#include "CommandHistory.h"

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
            bool begin_combo = ImGui::BeginCombo("##Prefabs", is_prefab ? name.c_str() : "None");
            if (begin_combo)
            {
                for (auto const& [prefab_name, prefab] : prefab_list)
                {
                    const bool is_selected = (name == prefab_name);
                    if (ImGui::Selectable(prefab_name.c_str(), is_selected))
                    {
                        engine.LoadFromPrefab(prefab, entity);
                    }
                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
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
        });

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
                CommandHistory::AddCommand(std::make_shared<ChangeCommand<Vector2D>>(transform.world_position, position));
            }

            // Render Rotation
            EditorUtils::RenderTableFixedWidth("TransformRotation", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Rotation");
                ImGui::TableNextColumn();

                // Apply modification
                float rotation = transform.rotation;
                if (ImGui::SliderFloat("##Rotation", &rotation, 0.f, 360.f, "%.f deg"))
                {
                    CommandHistory::AddCommand(std::make_shared<ChangeCommand<float>>(transform.rotation, rotation));
                }
            });

            // Render Scale
            Vec2 scaling = transform.scaling;
            if (EditorUtils::RenderControlVec2("Scale", scaling, 95.f, 120.f))
            {
                scaling = { abs(scaling.x), abs(scaling.y) };
                CommandHistory::AddCommand(std::make_shared<ChangeCommand<Vector2D>>(transform.scaling, scaling));
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
                Sprite::DrawLayer layer = static_cast<Sprite::DrawLayer>(sprite.layer);
                EditorUtils::RenderComboBoxEnum<Sprite::DrawLayer>("##Layer", layer, { "Background", "Default", "Foreground", "UI" });
                sprite.layer = layer;

                EditorUtils::RenderTableLabel("Color");

                ImGui::TableNextColumn();
                ImGui::ColorEdit4("##Color", &sprite.color[0]);
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

                                ImGui::TableNextColumn();
                                char buffer[256]{};
                                std::memcpy(buffer, animation.name.c_str(), animation.name.length());

                                ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
                                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                                if (ImGui::InputText("##AnimationName", buffer, sizeof(buffer), input_text_flags))
                                    animation.name = buffer;

                                EditorUtils::RenderTableLabel("Duration");
                                ImGui::TableNextColumn();
                                ImGui::SliderFloat("##Duration", &animation.animation_duration, 1.f, 10.f, "%.2f sec");

                                EditorUtils::RenderTableLabel("Columns");
                                ImGui::TableNextColumn();
                                ImGui::SliderInt("##Columns", &animation.x_frames, 1, 20);

                                EditorUtils::RenderTableLabel("Rows");
                                ImGui::TableNextColumn();
                                ImGui::SliderInt("##Rows", &animation.y_frames, 1, 20);
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
            EditorUtils::RenderControlVec2("Velocity", rigidbody.mVelocity);
            EditorUtils::RenderControlVec2("Force", rigidbody.mForce);

            // Render Other Component Attributes
            EditorUtils::RenderTableFixedWidth("Rigidbody Table", 2, [&]()
            {
                // Angular Velocity
                EditorUtils::RenderTableLabel("Angular Velocity");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::DragFloat("##AngularVelocity", &rigidbody.mAngularVelocity, 1.f, 0.f, 0.f, "%.2f");
                ImGui::PopItemWidth();

                // Body Type
                EditorUtils::RenderTableLabel("Body Type");
                ImGui::TableNextColumn();
                EditorUtils::RenderComboBoxEnum<BodyType>("##Body Type", rigidbody.mBodyType, { "Static", "Dynamic", "Kinematic","Ghost" , "WallClimb", "Spikes"});

                if (rigidbody.mBodyType == BodyType::Dynamic)
                {
                    EditorUtils::RenderTableLabel("Gravity Scale");
                    ImGui::TableNextColumn();
                    ImGui::PushItemWidth(80.f);
                    ImGui::DragFloat("##Gravity Scale", &rigidbody.mGravityScale);
                    ImGui::PopItemWidth();
                }

                // Mass
                EditorUtils::RenderTableLabel("Mass");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::DragFloat("##Mass", &rigidbody.mMass, 1.f, 0.f, 0.f, "%.2f");
                ImGui::PopItemWidth();

                // Inverse Mass
                EditorUtils::RenderTableLabel("Inverse Mass");
                ImGui::TableNextColumn();
                ImGui::Text("%.2f", &rigidbody.mInvMass);

                // Inertia
                EditorUtils::RenderTableLabel("Inertia");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::DragFloat("##Inertia", &rigidbody.mInertia, 1.f, 0.f, 0.f, "%.2f");
                ImGui::PopItemWidth();

                // Restitution
                EditorUtils::RenderTableLabel("Restitution");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::SliderFloat("##Restitution", &rigidbody.mRestitution, 0.f, 1.f, "%.2f");
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
                ImGui::InputFloat("##Static Friction", &rigidbody.mStaticFriction);
                ImGui::PopItemWidth();

                // Dynamic Friction
                EditorUtils::RenderTableLabel("Dynamic Friction");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(80.f);
                ImGui::InputFloat("##Dynamic Friction", &rigidbody.mDynamicFriction);
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
                EditorUtils::RenderControlVec2("Offset", collider.mBoxCollider.offset);
                EditorUtils::RenderControlVec2("Scale", collider.mBoxCollider.sizeScale);
            }
            
            if (circle_enabled)
            {
                ImGui::SeparatorText(ICON_LC_CIRCLE_DASHED "  Circle Collider");
                EditorUtils::RenderControlVec2("Offset", collider.mCircleCollider.offset);

                EditorUtils::RenderTableFixedWidth("Circle Radius Scale Table", 2, [&]()
                {
                    EditorUtils::AddTableBoldLabel("Radius Scale");

                    ImGui::TableNextColumn();
                    ImGui::PushItemWidth(80.f);
                    ImGui::InputFloat("##Radius Scale", &collider.mCircleCollider.radiusScale);
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
                        bool selected = script.mScriptName == stem;
                        if (ImGui::Selectable(stem.c_str(), selected))
                        {
                            script.mScriptName = stem;
                        }
                        if (selected)
                        {
                            ImGui::SetItemDefaultFocus();
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
                        script.mScriptName = path.stem().string();
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::SameLine();

                // Modify Script
                if (ImGui::Button(ICON_LC_FILE_SEARCH))
                {
                    if (std::filesystem::path filepath(FileUtils::OpenAndGetScript()); !filepath.empty())
                    {
                        script.mScriptName = filepath.stem().string();
                    }
                }
                ImGui::SetItemTooltip("Browse and replace the existing script");

                ImGui::SameLine();

                // Edit Script
                if (ImGui::Button(ICON_LC_FILE_EDIT))
                    InsightEngine::Instance().OpenGameScript(script.mScriptName + ".cs");
                ImGui::SetItemTooltip("Open and edit script in default application");
            });

        }); // end render Script Component

        // Audio Listener Component
        RenderComponent<AudioListener>(ICON_LC_EAR "  Audio Listener", entity, [entity, FONT_BOLD](AudioListener& listener)
        {
            EditorUtils::RenderTableFixedWidth("Audio Listener Table", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Volume");
                ImGui::TableNextColumn();
                int volume = static_cast<int>(listener.volume * 100);
                if (ImGui::SliderInt("##Volume", &volume, 0, 100, "%d%%"))
                {
                    listener.volume = static_cast<float>(volume) / 100.f;
                }

                EditorUtils::RenderTableLabel("Pitch Correctness");
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Pitch Correctness", &listener.pitch_correctness);

                EditorUtils::RenderTableLabel("Hearing Range");
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Hearing Range", &listener.hearing_range);
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

                EditorUtils::RenderTableLabel("Looped");
                ImGui::TableNextColumn();
                EditorUtils::RenderToggleButton("Looped", emitter.isLoop);

                EditorUtils::RenderTableLabel("Falloff Factor");
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Falloff Factor", &emitter.falloff_factor);

                EditorUtils::RenderTableLabel("Volume Level");
                ImGui::TableNextColumn();
                int volume_level = static_cast<int>(emitter.volumeLevel * 100);
                if (ImGui::SliderInt("##VolumeLevel", &volume_level, 0, 100, "%d%%"))
                {
                    emitter.volumeLevel = static_cast<float>(volume_level) / 100.f;
                }

                EditorUtils::RenderTableLabel("Pitch");
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Pitch", &emitter.pitch);
            });

        }); // end render Audio Emitter component

        // Button Component
        RenderComponent<ButtonComponent>(ICON_LC_SQUARE "  Button", entity, [entity, FONT_BOLD](ButtonComponent& button)
        {
            EditorUtils::RenderTableFixedWidth("Button Table", 2, [&]()
            {
                EditorUtils::RenderTableLabel("Text");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

                // Edit button text
                std::string& text = button.mButtonText;
                char buffer[256]{};
                auto source = text | std::ranges::views::take(text.size());
                std::ranges::copy(source, std::begin(buffer));
                ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
                if (ImGui::InputText("##ButtonText", buffer, sizeof(buffer), input_text_flags))
                {
                    text = std::string(buffer);
                }
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
                engine.RemoveComponent<Component>(entity);
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
                ImGui::InputFloat("##Near", &camera.mNear);

                EditorUtils::RenderTableLabel("Far");
                ImGui::TableNextColumn();
                ImGui::InputFloat("##Far", &camera.mFar);

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
                ImGui::SliderFloat("##CameraZoomSpeed", &Camera3D::mZoomSpeed, Camera3D::CAMERA_ZOOM_SPEED_MIN, Camera3D::CAMERA_ZOOM_SPEED_MAX, "%.2f");

                EditorUtils::RenderTableLabel("Pan");
                ImGui::TableNextColumn();
                ImGui::SliderFloat("##CameraPanSpeed", &Camera3D::mMoveSpeed, Camera3D::CAMERA_MOVE_SPEED_MIN, Camera3D::CAMERA_MOVE_SPEED_MAX, "%.2f");

            }, 0, ImGui::CalcTextSize("Zoom").x);
        });

        ImGui::PopStyleVar(); // end style rounding

    } // end RenderCameraControls()

} // end namespace IS

#endif // USING_IMGUI

