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
#include "HierarchyPanel.h"
#include "EditorUtils.h"
#include "CoreEngine.h"
#include "EditorLayer.h"
#include "FileUtils.h"
#include "GameGui.h"
#include "Body.h"
#include "Collider.h"

// Dependencies
#include <imgui.h>

namespace IS {

    void HierarchyPanel::RenderPanel()
    {
        auto& scene_manager = SceneManager::Instance();

        // Begin creating the scene hierarchy panel
        ImGui::Begin((ICON_LC_LIST_TREE "  " + mName).c_str());

        // Window COntents
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);

            mPanelSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

            // Display details about active scene
            RenderActiveSceneDetails();

            ImGui::Spacing();

            RenderLayerControls();

            ImGui::Spacing();

            // Filter entity hierarchy
            EditorUtils::RenderFilterWithHint(mFilter, "Filter Entity...");
            ImGui::Spacing();

            // Render list of scenes and its entities in child window
            ImGui::SetNextWindowBgAlpha(0.3f);
            if (ImGui::BeginChild("Scene Hierarchy Tree"))
            {
                using enum InspectorPanel::aInspectMode;
                ImGuiTreeNodeFlags camera_tree_flags = (mEditorLayer.GetInspectMode() == INSPECT_CAMERA) ? ImGuiTreeNodeFlags_Selected : 0;
                camera_tree_flags |= ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

                bool camera_tree_opened = ImGui::TreeNodeEx(ICON_LC_CAMERA "  Camera", camera_tree_flags);
                if (ImGui::IsItemClicked())
                {
                    mEditorLayer.SetInspectMode(INSPECT_CAMERA);
                    mEditorLayer.ResetEntitySelection();
                }

                ImGui::Separator();

                if (camera_tree_opened)
                    ImGui::TreePop();

                ImGuiTreeNodeFlags scene_tree_flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
                ;           bool scene_tree_opened = ImGui::TreeNodeEx(ICON_LC_NETWORK "  Scenes", scene_tree_flags);
                if (ImGui::IsItemClicked())
                    mEditorLayer.ResetInspectMode();

                if (ImGui::BeginPopupContextItem())
                {
                    if (ImGui::BeginMenu("Add"))
                    {
                        if (ImGui::MenuItem(ICON_LC_BOXES "  Scene")) { scene_manager.NewScene("NewScene"); }
                        ImGui::EndMenu();
                    }

                    ImGui::EndPopup();
                }

                // Display scene count
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(.8f, .8f, .8f, .8f), "(%d)", scene_manager.GetSceneCount());

                // Display scene tree
                if (scene_tree_opened)
                {
                    // Render all scenes
                    scene_manager.RunSceneFunction([this, &scene_manager](SceneID scene_id)
                    {
                        if (scene_id != scene_manager.GetActiveScene())
                        {
                            ImGui::SetNextItemOpen(false);
                        }

                        RenderSceneNode(scene_id);
                    });

                    ImGui::TreePop(); // end tree Scenes
                }
            }

            ImGui::EndChild(); // end child window Scene Hierarchy Tree

            // Deselect entity and reset inpect mode
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && (ImGui::IsItemHovered() || ImGui::IsWindowHovered()))
            {
                mEditorLayer.ResetEntitySelection();
                mEditorLayer.ResetInspectMode();
            }

            // Accept file drop
            mEditorLayer.AcceptAssetBrowserPayload();

            ImGui::PopStyleVar(); // frame rounding
        }

        // Save window states
        mFocused    = ImGui::IsWindowFocused();
        mAppearing  = ImGui::IsWindowAppearing();
        mHovered    = ImGui::IsItemHovered();
        ImGui::End(); // end window Scene Hierarchy

    } // end RenderPanel()

    void HierarchyPanel::RenderActiveSceneDetails()
    {
        auto& scene_manager = SceneManager::Instance();
        auto const FONT_BOLD = ImGui::GetIO().Fonts->Fonts[FONT_TYPE_BOLD];

        // Comma separted numbers
        std::ostringstream entity_count, max_entity_count;
        entity_count.imbue(std::locale(""));
        entity_count << std::fixed << scene_manager.GetActiveEntityCount();
        max_entity_count.imbue(std::locale(""));
        max_entity_count << std::fixed << MAX_ENTITIES;

        // Display details about active scene
        if (ImGui::BeginTable("ActiveScene", 2))
        {
            // Display active scene
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Active Scene:");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(scene_manager.GetActiveSceneName());

            // Entities Alive
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Entities Alive:");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(entity_count.str().c_str());
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(.8f, .8f, .8f, .8f), "(Max: %s)", max_entity_count.str().c_str());

            ImGui::EndTable(); // end table ActiveScene
        }

    } // end RenderSceneDetails()

    void HierarchyPanel::RenderLayerControls()
    {
        auto& style = ImGui::GetStyle();
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;
        bool layers_opened = ImGui::TreeNodeEx(ICON_LC_LAYERS "  Layers", tree_flags);
        static bool to_render[Sprite::INVALID_LAYER]{ true };
        
        if (layers_opened)
        {
            if (ImGui::BeginTable("LayersTable", 2, ImGuiTableFlags_BordersH))
            {
                ImGui::TableSetupColumn("Visible", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize(ICON_LC_EYE).x + style.FramePadding.x);
                for (int i{}; i < Sprite::INVALID_LAYER; ++i)
                {
                    to_render[i] = (Sprite::layersToIgnore.find(i) == Sprite::layersToIgnore.end());

                    Sprite::DrawLayer layer = static_cast<Sprite::DrawLayer>(i);
                    ImGui::TableNextColumn();
                    const char* icon = to_render[i] ? ICON_LC_EYE : ICON_LC_EYE_OFF;
                    ImGui::PushID(i);
                    ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
                    if (ImGui::Button(icon))
                        Sprite::toggleLayer(layer);
                    ImGui::PopStyleColor();
                    ImGui::PopID();
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(Sprite::LayerToString(layer).c_str());
                }
                ImGui::EndTable(); // end table LayersTable
            }
            ImGui::TreePop(); // pop tree Layers
        }
    } // end RenderLayers()

    void HierarchyPanel::RenderSceneNode(SceneID scene)
    {
        auto& scene_manager = SceneManager::Instance();        
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        bool opened = ImGui::TreeNodeEx((ICON_LC_BOXES "  " + std::string(scene_manager.GetSceneName(scene))).c_str(), tree_flags);
        if (ImGui::IsItemClicked())
            mEditorLayer.ResetInspectMode();

        // Right click on scene
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::BeginMenu("Add"))
            {
                if (ImGui::MenuItem(ICON_LC_BOX "  Entity"))        { scene_manager.AddEntity("Entity"); }
                if (ImGui::MenuItem(ICON_LC_SHUFFLE "  Random Entity")) { scene_manager.AddRandomEntity(); }

                ImGui::EndMenu(); // end menu Add
            }

            ImGui::EndPopup();
        }

        // Switch scenes
        if (ImGui::IsItemClicked())
        {
            mEditorLayer.ResetEntitySelection();
            scene_manager.SwitchScene(scene);
            IS_CORE_DEBUG("Scene {} : {} clicked", scene, scene_manager.GetSceneName(scene));
            IS_CORE_DEBUG("Active Scene {}", scene_manager.GetActiveScene());
        }

        // Display scene entity count
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(.8f, .8f, .8f, .8f), "(%d)", scene_manager.GetEntityCount(scene));

        // Render entity tree
        if (opened)
        {
            // Render all filtered entities
            scene_manager.RunEntityFunction(scene, [this](Entity entity, std::string name)
            {
                if (mFilter.PassFilter(name.c_str()))
                    RenderEntityNode(entity);
            });

            ImGui::TreePop(); // end tree scene
        }
        if (scene_manager.GetActiveScene() == scene)
        {
            ImGui::SetNextItemOpen(false);
        }
    }

    void HierarchyPanel::RenderEntityNode(Entity entity)
    {
        using enum InspectorPanel::aInspectMode;
        InsightEngine& engine = InsightEngine::Instance();
        const bool is_selected_entity = mEditorLayer.IsAnyEntitySelected() && (mEditorLayer.GetSelectedEntity() == entity);
        const bool is_inspect_entity = mEditorLayer.GetInspectMode() == INSPECT_ENTITY;

        ImGuiTreeNodeFlags tree_flags = is_selected_entity && is_inspect_entity ? ImGuiTreeNodeFlags_Selected : 0;
        tree_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;

        ImGui::PushID(entity);
        bool opened = ImGui::TreeNodeEx((ICON_LC_BOX "  " + engine.GetEntityName(entity)).c_str(), tree_flags);

        // Set as selected entity
        if (ImGui::IsItemClicked())
        {
            mEditorLayer.SetSelectedEntity(std::make_shared<Entity>(entity));
            mEditorLayer.SetInspectMode(INSPECT_ENTITY);
        }

        if (ImGui::IsWindowFocused())
            ProcessSelectedEntityShortcuts();

        // Right click on entity
        RenderEntityConfig(entity);

        if (opened)
            ImGui::TreePop();
        ImGui::PopID();

    } // end RenderEntityNode()

    void HierarchyPanel::ProcessSelectedEntityShortcuts()
    {
        if (!mEditorLayer.IsAnyEntitySelected())
            return;

        Entity entity = mEditorLayer.GetSelectedEntity();

        auto& engine = InsightEngine::Instance();
        auto input = engine.GetSystem<InputManager>("Input");
        //const bool CTRL_PRESSED = input->IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || input->IsKeyPressed(GLFW_KEY_RIGHT_CONTROL);
        //const bool D_PRESSED = input->IsKeyPressed(GLFW_KEY_D);
        const bool DELETE_PRESSED = input->IsKeyPressed(GLFW_KEY_DELETE);

        //if (CTRL_PRESSED && D_PRESSED) { CloneEntity(entity); }  // Ctrl+D
        if (DELETE_PRESSED) { DeleteEntity(entity); } // Delete
    }

    //void HierarchyPanel::RenderConfirmDelete(Entity entity, bool& show)
    //{
    //    InsightEngine& engine = InsightEngine::Instance();
    //    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;

    //    if (ImGui::Begin("Confirm delete?", &show, window_flags))
    //    {
    //        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.8f, .1f, .15f, 1.f));
    //        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.9f, .2f, .2f, 1.f));
    //        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.8f, .1f, .15f, 1.f));

    //        ImGuiTableFlags table_flags = ImGuiTableFlags_NoBordersInBody;
    //        ImGui::BeginTable("Confirm actions", 2, table_flags, ImVec2(0, 0), 10.f);
    //        ImGui::TableNextColumn();
    //        if (ImGui::Button("CONFIRM"))
    //        {
    //            engine.DeleteEntity(entity);
    //            if (mSelectedEntity && *mSelectedEntity == entity)
    //                mSelectedEntity.reset();
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

    void HierarchyPanel::CloneEntity(Entity entity) { SceneManager::Instance().CloneEntity(entity); }

    void HierarchyPanel::DeleteEntity(Entity entity)
    {
        SceneManager::Instance().DeleteEntity(entity);
        if (mEditorLayer.IsAnyEntitySelected() && mEditorLayer.GetSelectedEntity() == entity)
            mEditorLayer.ResetEntitySelection();
    }

    void HierarchyPanel::RenderAddComponent(Entity entity)
    {
        auto& engine = InsightEngine::Instance();

        // Entity already has all the components
        if (engine.HasComponent<Transform>(entity) && engine.HasComponent<Sprite>(entity) &&
            engine.HasComponent<RigidBody>(entity) && engine.HasComponent<Collider>(entity) &&
            engine.HasComponent<ScriptComponent>(entity) && engine.HasComponent<AudioListener>(entity) && 
            engine.HasComponent<AudioEmitter>(entity) && engine.HasComponent<ButtonComponent>(entity))
        {
            if (ImGui::MenuItem("Already have all components"))
                ImGui::CloseCurrentPopup();
            return;
        }

        // Choose Available Component to add

        // Add Transform Component
        if (!engine.HasComponent<Transform>(entity))
        {
            if (ImGui::MenuItem(ICON_LC_MOVE "  Transform"))
            {
                engine.AddComponent<Transform>(entity, Transform());
                ImGui::CloseCurrentPopup();
            }
        }

        // Add Sprite Component
        if (!engine.HasComponent<Sprite>(entity))
        {
            if (ImGui::MenuItem(ICON_LC_IMAGE "  Sprite"))
            {
                engine.AddComponent<Sprite>(entity, Sprite());
                ImGui::CloseCurrentPopup();
            }
        }

        // Add Rigidbody Component
        if (!engine.HasComponent<RigidBody>(entity))
        {
            if (ImGui::MenuItem(ICON_LC_PERSON_STANDING "  Rigidbody"))
            {
                engine.AddComponent<RigidBody>(entity, RigidBody());
                ImGui::CloseCurrentPopup();
            }
        }

        // Add Collider Component
        if (!engine.HasComponent<Collider>(entity))
        {
            if (ImGui::MenuItem(ICON_LC_FLIP_HORIZONTAL_2 "  Collider"))
            {
                engine.AddComponent<Collider>(entity, Collider());
                ImGui::CloseCurrentPopup();
            }
        }

        // Add Script Component
        if (!engine.HasComponent<ScriptComponent>(entity))
        {
            if (ImGui::MenuItem(ICON_LC_BRACES "  Script"))
            {
                // Browse for script to add
                if (std::filesystem::path filepath(FileUtils::OpenAndGetScript()); !filepath.empty())
                {
                    engine.AddComponent<ScriptComponent>(entity, ScriptComponent());
                    auto& script = engine.GetComponent<ScriptComponent>(entity);
                    script.mScriptName = filepath.stem().string();
                }
                ImGui::CloseCurrentPopup();
            }
        }

        // Add Audio Listener Component
        if (!engine.HasComponent<AudioListener>(entity))
        {
            if (ImGui::MenuItem(ICON_LC_EAR "  Audio Listener"))
            {
                engine.AddComponent<AudioListener>(entity, AudioListener());
                ImGui::CloseCurrentPopup();
            }
        }

        // Add Audio Emitter Component
        if (!engine.HasComponent<AudioEmitter>(entity))
        {
            if (ImGui::MenuItem(ICON_LC_SPEAKER "  Audio Emitter"))
            {
                engine.AddComponent<AudioEmitter>(entity, AudioEmitter());
                ImGui::CloseCurrentPopup();
            }
        }

        // Add Button Component
        if (!engine.HasComponent<ButtonComponent>(entity))
        {
            if (ImGui::MenuItem(ICON_LC_SQUARE "  Button"))
            {
                engine.AddComponent<ButtonComponent>(entity, ButtonComponent());
                ImGui::CloseCurrentPopup();
            }
        }

    } // end RenderAddComponent()

    void HierarchyPanel::RenderEntityConfig(Entity entity)
    {
        if (ImGui::BeginPopupContextItem(nullptr, ImGuiPopupFlags_MouseButtonRight))
        {
            // Add Component
            if (ImGui::BeginMenu("Add Component"))
            {
                RenderAddComponent(entity);
                ImGui::EndMenu();
            }

            // Clone/Delete entity
            if (ImGui::MenuItem(ICON_LC_COPY "  Clone")) { CloneEntity(entity); }
            if (ImGui::MenuItem(ICON_LC_TRASH_2 "  Delete", "Del")) { DeleteEntity(entity); }

            ImGui::EndPopup();
        }
    }

    Vec2 HierarchyPanel::GetPanelSize() const { return mPanelSize; }

} // end namespace IS
