/*!
 * \file EditorLayer.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This header file declares the interface for class EditorLayer which
 * encapsulates the functionalities of a level editor layer.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
#ifdef USING_IMGUI

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_EDITORLAYER_H
#define GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_EDITORLAYER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Engine/ECS/Entities.h"
#include "Layer.h"
#include "Editor/Panels/Panel.h"
#include "Editor/Panels/ScenePanel.h"
#include "Editor/Panels/InspectorPanel.h"
#include "Editor/Panels/HierarchyPanel.h"
#include "Utils/Containers/OrderedMap.h"

#include <functional>

namespace IS {

    /*!
     * \class EditorLayer
     * \brief A class representing the editor layer in the application.
     *
     * The \c EditorLayer class inherits from the \c Layer class and provides functionality for
     * managing the editor interface, rendering panels, and handling updates.
     */
    class EditorLayer : public Layer {
    public:
        template <typename PanelType>
        using PanelPointer = std::shared_ptr<PanelType>;

        /*!
         * \brief Constructor for the EditorLayer class.
         */
        EditorLayer();

        /*!
         * \brief Called when the layer is attached to the application.
         */
        void OnAttach() override;

        /*!
         * \brief Called when the layer is detached from the application.
         */
        void OnDetach() override;

        /*!
         * \brief Called on every frame update.
         * 
         * \param dt The time elapsed since the last frame (in seconds).
         */
        void OnUpdate(float dt) override;

        /*!
         * \brief Called for rendering the editor scene panel.
         */
        void OnRender() override;

        /*!
         * \brief Get the dockspace position.
         * 
         * \return Dockspace position.
         */
        Vec2 GetDockspacePosition();

        /*!
         * \brief Load a scene.
         *
         * \param path Path to the scene.
         */
        void OpenScene(std::string const& path);

        /*!
         * \brief Accept and process payload from asset browser.
         */
        void AcceptAssetBrowserPayload();

        /*!
         * \brief Renders gui to add component to an entity.
         *
         * \param entity The Entity to add components to.
         */
        void RenderAddComponent(Entity entity);

        /*!
         * \brief Renders configuration for entities.
         *
         * Configurations include rename, save/load prefab,
         * add component, clone/delete entity.
         */
        void RenderEntityConfig(Entity entity);

        /*!
         * \brief Renders configuration for the specified Entity.

         * \param entity The Entity to be deleted.
         * \param show Boolean flag to determine where to show window.
         */
        void RenderConfirmDelete(Entity entity);

        /*!
         * \brief Clone an existing entity.
         *
         * \param entity The Entity to be cloned.
         */
        void CloneEntity(Entity entity);

        /*!
         * \brief Delete an existing entity.
         *
         * \param entity The Entity to be deleted.
         */
        void DeleteEntity(Entity entity);

        /*!
         * \brief Check if the game panel is in focus.
         *
         * \return Boolean flag indicating focus.
         */
        inline bool IsGamePanelFocused() const { return mPanels.Get<GamePanel>("Game")->IsFocused(); }

        /*!
         * \brief Check if the scene panel is in focus.
         *
         * \return Boolean flag indicating focus.
         */
        inline bool IsScenePanelFocused() const { return mPanels.Get<ScenePanel>("Scene")->IsFocused(); }

        /*!
         * \brief Get the texture id of an icon.
         *
         * \param icon The key for the icon.
         * \return texture id to the icon.
         */
        inline ImTextureID GetIcon(const char* icon) const { return mIcons.at(icon); }

        /*!
         * \brief Get size of viewport.
         * 
         * \return Size of viewport.
         */
        inline Vec2 GetViewportSize() { return mPanels.Get<ScenePanel>("Scene")->GetSize(); }

        inline std::pair<int, int> GetMousePos() { return mPanels.Get<ScenePanel>("Scene")->GetMousePos(); }

        /*!
         * \brief Checks if any entity is currently selected.
         *
         * \return `true` if any entity is selected, `false` otherwise.
         */
        inline bool IsAnyEntitySelected() const { return mSelectedEntity ? true : false; }

        /*!
         * \brief Gets the selected entity.
         *
         * \return The selected entity.
         */
        inline Entity GetSelectedEntity() const { return *mSelectedEntity; }

        /*!
         * \brief Resets the currently selected entity.
         *
         * This function clears the selected entity by resetting the associated shared pointer.
         */
        void ResetEntitySelection() { mSelectedEntity.reset(); }

        /*!
         * \brief Sets the selected entity using a shared pointer to an Entity.
         *
         * \param entity_ptr A shared pointer to the Entity to be set as the selected entity.
         */
        void SetSelectedEntity(std::shared_ptr<Entity> entity_ptr) { mSelectedEntity = entity_ptr; }

        /*!
         * \brief Gets the current inspect mode.
         *
         * \return The current inspect mode.
         */
        InspectorPanel::aInspectMode GetInspectMode() const { return mPanels.Get<InspectorPanel>("Inspector")->mInspectMode; }

        /*!
         * \brief Sets the current inspect mode.
         *
         * \param inspect_mode The inspect mode to set to.
         */
        void SetInspectMode(InspectorPanel::aInspectMode inspect_mode) { mPanels.Get<InspectorPanel>("Inspector")->mInspectMode = inspect_mode; }

        /*!
         * \brief Resets the inspect mode of the InspectorPanel.
         *
         * This function sets the inspect mode of the InspectorPanel to INSPECT_NONE.
         */
        void ResetInspectMode() { mPanels.Get<InspectorPanel>("Inspector")->mInspectMode = InspectorPanel::aInspectMode::INSPECT_NONE; }

        /*!
         * \brief Retrieves a pointer to a panel of the specified type using the panel name.
         *
         * This function performs a dynamic cast to obtain a pointer of the specified PanelType.
         *
         * \tparam PanelType The type of the panel to retrieve.
         * \param panel_name The name of the panel to retrieve.
         * \return A PanelPointer<PanelType> pointing to the retrieved panel.
         */
        template <typename PanelType>
        PanelPointer<PanelType> GetPanel(std::string const& panel_name) { return std::dynamic_pointer_cast<PanelType>(mPanels[panel_name]); }

    private:
        bool mShowNewScene = false; ///< Flag indicating to show new scene.
        bool mShowNewScript = false; ///< Flag indicating to show new script.
        bool mShowDelete = false;

        Vec2 mDockspacePosition{}; ///< Position of the dockspace in the editor.

        std::shared_ptr<Entity> mSelectedEntity{}; ///< Pointer to the selected entity.
        Entity mEntityToDelete{};

        std::unordered_map<std::string, ImTextureID> mIcons{}; ///< Icons used by the dockspace.
        OrderedMap<std::string, Panel> mPanels{}; ///< An ordered map of panels.

        /*!
         * \brief Render the menu bar.
         */
        void RenderMenuBar();

        /*!
         * \brief Render the tool bar.
         */
        void RenderToolBar();

        /*!
         * \brief Attach panels.
         */
        void AttachPanels();

        /*! \brief Show a create popup modal with a customizable name, default text, and create action.
         *
         * This function displays a create popup modal using ImGui. The popup can be customized
         * with a unique name, default text, and an action to execute when the "Create" button is pressed.
         *
         * \param popup_name The name of the popup modal.
         * \param default_text The default text displayed in the input field.
         * \param show A pointer to a boolean variable that controls the visibility of the popup.
         * \param CreateAction A function that takes a const char* parameter and is executed when the "Create" button is pressed.
         *
         * Example usage:
         * \code
         * bool mShowNewScript = false;
         * ShowCreatePopup("Create new script", "NewScript", &mShowNewScript, [](const char* script_name) {
         *     // Your create action code here
         * });
         * \endcode
         */
        void ShowCreatePopup(const char* popup_name, const char* default_text, bool* show, std::function<void(const char*)> CreateAction);

        /*!
         * \brief Load a scene.
         */
        void OpenScene();

        /*!
         * \brief Save current scene.
         */
        void SaveScene();

        /*!
         * \brief Save all scenes.
         */
        void SaveAllScenes();

        /*!
         * \brief Save current scene as.
         */
        void SaveSceneAs();

        /*!
         * \brief Toggle fullscreen mode.
         */
        void ToggleFullscreen();

        /*!
         * \brief Exit current program.
         */
        void ExitProgram();
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_EDITORLAYER_H

#endif // USING_IMGUI
