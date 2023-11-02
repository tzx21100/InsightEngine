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
#include "SceneHierarchyPanel.h"
#include "EditorUtils.h"
#include "CoreEngine.h"
#include "EditorLayer.h"
#include "FileUtils.h"
#include "Editor.h"
#include "GameGui.h"

// Dependencies
#include <imgui.h>

namespace IS {

    void SceneHierarchyPanel::RenderPanel()
    {
        auto& engine = InsightEngine::Instance();
        auto& scene_manager = SceneManager::Instance();

        // Begin creating the scene hierarchy panel
        ImGui::Begin("Scene Hierarchy");
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);

        mPanelSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

        // Display details about active scene
        RenderActiveSceneDetails();

        ImGui::Spacing();
        RenderLayers();
        ImGui::Spacing();

        // Filter entity hierarchy
        EditorUtils::RenderFilterWithHint(mFilter, "Search Entity...");
        ImGui::Spacing();

        // Render list of scenes and its entities in child window
        ImGui::SetNextWindowBgAlpha(0.3f);
        if (ImGui::BeginChild("Scene Hierarchy Tree"))
        {
            // Set up scene tree
            ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
            bool opened = ImGui::TreeNodeEx("Scenes", tree_flags);
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Add Scene")) { scene_manager.NewScene("NewScene"); }

                ImGui::EndPopup();
            }

            // Display scene count
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(.8f, .8f, .8f, .8f), "(%d)", scene_manager.GetSceneCount());

            // Display scene tree
            if (opened)
            {
                // Render all scenes
                scene_manager.RunSceneFunction([this](SceneID scene_id)
                {
                    RenderSceneNode(scene_id);
                });

                ImGui::TreePop(); // end tree Scenes
            }
        }

        ImGui::EndChild(); // end child window Scene Hierarchy Tree
        
        // Deselect entity
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && (ImGui::IsItemHovered() || ImGui::IsWindowHovered()))
            ResetSelection();

        ImGui::PopStyleVar();

        // Accept file drop
        auto editor = engine.GetSystem<Editor>("Editor");
        editor->GetEditorLayer()->AcceptAssetBrowserPayload();

        ImGui::End(); // end window Scene Hierarchy

    } // end RenderPanel()

    void SceneHierarchyPanel::RenderActiveSceneDetails()
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

    void SceneHierarchyPanel::RenderLayers()
    {
        auto const FONT_BOLD = ImGui::GetIO().Fonts->Fonts[FONT_TYPE_BOLD];
        
        ImGui::PushFont(FONT_BOLD);
        ImGui::TextUnformatted("Layers");
        ImGui::PopFont();

        static bool to_render[Sprite::INVALID_LAYER]{ true };

        for (int i{}; i < Sprite::INVALID_LAYER; ++i)
        {
            if (Sprite::layersToIgnore.find(i) == Sprite::layersToIgnore.end())
            {
                to_render[i] = true;
            }
            else
            {
                to_render[i] = false;
            }
            Sprite::DrawLayer layer = static_cast<Sprite::DrawLayer>(i);
            if (ImGui::Checkbox(("##" + std::to_string(i) + "RenderLayer").c_str(), &to_render[i]))
                Sprite::toggleLayer(layer);
            ImGui::SameLine();
            ImGui::TextUnformatted(Sprite::LayerToString(layer).c_str());
        }
    }

    void SceneHierarchyPanel::RenderSceneNode(SceneID scene)
    {
        auto& scene_manager = SceneManager::Instance();        
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx(scene_manager.GetSceneName(scene), tree_flags);

        // Right click on scene
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::BeginMenu("Add"))
            {
                if (ImGui::MenuItem("Entity"))        { scene_manager.AddEntity("Entity"); }
                if (ImGui::MenuItem("Random Entity")) { scene_manager.AddRandomEntity(); }

                ImGui::EndMenu(); // end menu Add
            }

            ImGui::EndPopup();
        }

        // Switch scenes
        if (ImGui::IsItemClicked())
        {
            ResetSelection();
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

    void SceneHierarchyPanel::RenderEntityNode(Entity entity)
    {
        InsightEngine& engine = InsightEngine::Instance();
        bool is_selected_entity = mSelectedEntity && (*mSelectedEntity == entity);
        ImGuiTreeNodeFlags tree_flags = is_selected_entity ? ImGuiTreeNodeFlags_Selected : 0;
        tree_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;

        ImGui::PushID(entity);
        bool opened = ImGui::TreeNodeEx(engine.GetEntityName(entity).c_str(), tree_flags);

        // Set as selected entity
        if (ImGui::IsItemClicked())
            mSelectedEntity = std::make_shared<Entity>(entity);

        if (ImGui::IsWindowFocused())
            ProcessSelectedEntityShortcuts();

        // Right click on entity
        RenderEntityConfig(entity);

        if (opened)
            ImGui::TreePop();
        ImGui::PopID();

    } // end RenderEntityNode()

    void SceneHierarchyPanel::ProcessSelectedEntityShortcuts()
    {
        if (!mSelectedEntity)
            return;

        Entity entity = *mSelectedEntity;

        auto& engine = InsightEngine::Instance();
        auto input = engine.GetSystem<InputManager>("Input");
        //const bool CTRL_PRESSED = input->IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || input->IsKeyPressed(GLFW_KEY_RIGHT_CONTROL);
        //const bool D_PRESSED = input->IsKeyPressed(GLFW_KEY_D);
        const bool DELETE_PRESSED = input->IsKeyPressed(GLFW_KEY_DELETE);

        //if (CTRL_PRESSED && D_PRESSED) { CloneEntity(entity); }  // Ctrl+D
        if (DELETE_PRESSED) { DeleteEntity(entity); } // Delete
    }

    //void SceneHierarchyPanel::RenderConfirmDelete(Entity entity, bool& show)
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

    void SceneHierarchyPanel::CloneEntity(Entity entity) { SceneManager::Instance().CloneEntity(entity); }

    void SceneHierarchyPanel::DeleteEntity(Entity entity)
    {
        SceneManager::Instance().DeleteEntity(entity);
        if (mSelectedEntity && *mSelectedEntity == entity)
            mSelectedEntity.reset();
    }

    void SceneHierarchyPanel::RenderAddComponent(Entity entity)
    {
        auto& engine = InsightEngine::Instance();

        // Entity already has all the components
        if (engine.HasComponent<Transform>(entity) && engine.HasComponent<Sprite>(entity) &&
            engine.HasComponent<RigidBody>(entity) && engine.HasComponent<ScriptComponent>(entity) &&
            engine.HasComponent<ButtonComponent>(entity))
        {
            if (ImGui::MenuItem("Already have all components"))
                ImGui::CloseCurrentPopup();
            return;
        }

        // Choose Available Component to add

        // Add Transform Component
        if (!engine.HasComponent<Transform>(entity))
        {
            if (ImGui::MenuItem("Transform"))
            {
                engine.AddComponent<Transform>(entity, Transform());
                ImGui::CloseCurrentPopup();
            }
        }

        // Add Sprite Component
        if (!engine.HasComponent<Sprite>(entity))
        {
            if (ImGui::MenuItem("Sprite"))
            {
                engine.AddComponent<Sprite>(entity, Sprite());
                ImGui::CloseCurrentPopup();
            }
        }

        // Add Rigidbody Component
        if (!engine.HasComponent<RigidBody>(entity))
        {
            if (ImGui::MenuItem("Rigidbody"))
            {
                engine.AddComponent<RigidBody>(entity, RigidBody());
                ImGui::CloseCurrentPopup();
            }
        }

        // Add Script Component
        if (!engine.HasComponent<ScriptComponent>(entity))
        {
            if (ImGui::MenuItem("Script"))
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

        // Add Button Component
        if (!engine.HasComponent<ButtonComponent>(entity))
        {
            if (ImGui::MenuItem("Button"))
            {
                engine.AddComponent<ButtonComponent>(entity, ButtonComponent());
                ImGui::CloseCurrentPopup();
            }
        }

    } // end RenderAddComponent()

    void SceneHierarchyPanel::ResetSelection() { mSelectedEntity.reset(); }

    SceneHierarchyPanel::EntityPtr SceneHierarchyPanel::GetSelectedEntity() { return mSelectedEntity; }

    void SceneHierarchyPanel::SetSelectedEntity(EntityPtr entity_ptr) { mSelectedEntity = entity_ptr; }

    void SceneHierarchyPanel::RenderSelectedEntityOutline()
    {
        if (!mSelectedEntity)
            return;

        Entity entity = *mSelectedEntity;

        auto& engine = InsightEngine::Instance();
        if (engine.HasComponent<Sprite>(entity))
        {
            auto& sprite = engine.GetComponent<Sprite>(entity);
            //auto& body = engine.GetComponent<RigidBody>(entity);
            ISGraphics::DrawOutLine(sprite, { 1.f, .675f, .11f }, 3.f);
        }
    }

    void SceneHierarchyPanel::RenderEntityConfig(Entity entity)
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
            if (ImGui::MenuItem("Clone")) { CloneEntity(entity); }
            if (ImGui::MenuItem("Delete", "Del")) { DeleteEntity(entity); }

            ImGui::EndPopup();
        }
    }

    Vec2 SceneHierarchyPanel::GetPanelSize() const { return mPanelSize; }

} // end namespace IS
