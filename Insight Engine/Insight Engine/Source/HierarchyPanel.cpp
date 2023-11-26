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

    void HierarchyPanel::UpdatePanel()
    {
        if (!(mEditorLayer.IsAnyEntitySelected() && mChildWindowHovered))
            return;

        Entity entity = mEditorLayer.GetSelectedEntity();

        auto& engine = InsightEngine::Instance();
        auto input = engine.GetSystem<InputManager>("Input");
        const bool CTRL_PRESSED = input->IsKeyHeld(GLFW_KEY_LEFT_CONTROL) || input->IsKeyHeld(GLFW_KEY_RIGHT_CONTROL);
        const bool D_PRESSED = input->IsKeyPressed(GLFW_KEY_D);
        const bool DELETE_PRESSED = input->IsKeyPressed(GLFW_KEY_DELETE);

        if (CTRL_PRESSED && D_PRESSED) { mEditorLayer.CloneEntity(entity); }  // Ctrl+D
        if (DELETE_PRESSED) { mEditorLayer.DeleteEntity(entity); } // Delete
    }

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
            EditorUtils::RenderFilterWithHint(mFilter, "Filter Game Object...");
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
            mChildWindowHovered = ImGui::IsWindowHovered();
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
        mHovered    = ImGui::IsWindowHovered();
        ImGui::End(); // end window Scene Hierarchy

    } // end RenderPanel()

    void HierarchyPanel::RenderActiveSceneDetails()
    {
        auto& scene_manager = SceneManager::Instance();
        auto const FONT_BOLD = ImGui::GetIO().Fonts->Fonts[FONT_TYPE_BOLD];

        // Comma separted numbers
        std::ostringstream entity_count;
        entity_count.imbue(std::locale(""));
        entity_count << std::fixed << scene_manager.GetActiveEntityCount();

        // Display details about active scene
        EditorUtils::RenderTableFixedWidth("ActiveScene", 2, [&]()
        {
            // Display active scene
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Active Scene:");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(scene_manager.GetActiveSceneName());

            // Display no. of game objects in active scene
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Game Objects:");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(entity_count.str().c_str());
        });

    } // end RenderSceneDetails()

    void HierarchyPanel::RenderLayerControls()
    {
        auto& style = ImGui::GetStyle();
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;
        bool layers_opened = ImGui::TreeNodeEx(ICON_LC_LAYERS "  Layers", tree_flags);
        static bool to_render[INVALID_LAYER]{ true };
        
        if (layers_opened)
        {
            EditorUtils::RenderTable("LayersTable", 2, [&]()
            {
                ImGui::TableSetupColumn("Visible", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize(ICON_LC_EYE).x + style.FramePadding.x);
                for (int i{}; i < INVALID_LAYER; ++i)
                {
                    to_render[i] = (Sprite::layersToIgnore.find(i) == Sprite::layersToIgnore.end());

                    DrawLayer layer = static_cast<DrawLayer>(i);
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
            }, ImGuiTableFlags_BordersH);

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
                if (ImGui::MenuItem(ICON_LC_BOX "  Game Object"))        { scene_manager.AddEntity("Game Object"); }
                if (ImGui::MenuItem(ICON_LC_SHUFFLE "  Random Game Object")) { scene_manager.AddRandomEntity(); }

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

        // Right click on entity
        if (ImGui::BeginPopupContextItem(nullptr, ImGuiPopupFlags_MouseButtonRight))
        {
            mEditorLayer.RenderEntityConfig(entity);
            ImGui::EndPopup();
        }

        if (opened)
            ImGui::TreePop();
        ImGui::PopID();

    } // end RenderEntityNode()

    void HierarchyPanel::ProcessSelectedEntityShortcuts()
    {
        if (!(mEditorLayer.IsAnyEntitySelected() && mHovered))
            return;

        Entity entity = mEditorLayer.GetSelectedEntity();

        auto& engine = InsightEngine::Instance();
        auto input = engine.GetSystem<InputManager>("Input");
        const bool CTRL_PRESSED = input->IsKeyHeld(GLFW_KEY_LEFT_CONTROL) || input->IsKeyHeld(GLFW_KEY_RIGHT_CONTROL);
        const bool D_PRESSED = input->IsKeyPressed(GLFW_KEY_D);
        const bool DELETE_PRESSED = input->IsKeyPressed(GLFW_KEY_DELETE);

        if (CTRL_PRESSED && D_PRESSED) { mEditorLayer.CloneEntity(entity); }  // Ctrl+D
        if (DELETE_PRESSED) { mEditorLayer.DeleteEntity(entity); } // Delete
    }

} // end namespace IS


#endif // USING_IMGUI
