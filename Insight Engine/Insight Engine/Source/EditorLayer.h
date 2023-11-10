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

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_EDITORLAYER_H
#define GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_EDITORLAYER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Layer.h"
#include "Panel.h"
#include "HierarchyPanel.h"

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
         * \brief Check if the game panel is in focus.
         *
         * \return Boolean flag indicating focus.
         */
        bool IsGamePanelFocused() const;

        /*!
         * \brief Check if the scene panel is in focus.
         *
         * \return Boolean flag indicating focus.
         */
        bool IsScenePanelFocused() const;

        /*!
         * \brief Get the texture id of an icon.
         *
         * \param icon The key for the icon.
         * \return texture id to the icon.
         */
        ImTextureID GetIcon(const char* icon) const;

        /*!
         * \brief Render outline of the selected entity.
         */
        void RenderSelectedEntityOutline() const;

        /*!
         * \brief Get size of viewport.
         * 
         * \return Size of viewport.
         */
        Vec2 GetViewportSize();

        /*!
         * \brief Get size of console panel.
         *
         * \return Size of console panel.
         */
        Vec2 GetConsolePanelSize() const { return mConsolePanel->GetPanelSize(); }

        /*!
         * \brief Get size of scene hierarchy panel.
         *
         * \return Size of scene hierarchy panel.
         */
        Vec2 GetHierarchyPanel() const { return mHierarchyPanel->GetPanelSize(); }

        std::shared_ptr<Entity> GetSelectedEntity() const { return mHierarchyPanel->GetSelectedEntity(); }

    private:
        // Internal Flags
        bool mShowNewScene = false; ///< Flag indicating to show new scene.
        bool mShowNewScript = false; ///< Flag indicating to show new script.

        Vec2 mDockspacePosition; ///< Position of the dockspace in the editor.

        std::shared_ptr<Entity> mHoveredEntity;
        std::shared_ptr<GamePanel> mGamePanel; ///< Instance of game panel.
        std::shared_ptr<ScenePanel> mScenePanel; ///< Instance of scene panel.
        std::shared_ptr<HierarchyPanel> mHierarchyPanel; ///< Instance of hierarchy panel.
        std::shared_ptr<ConsolePanel> mConsolePanel; ///< Instance of console panel.

        std::unordered_map<std::string, ImTextureID> mIcons; ///< Icons used by the dockspace.
        std::vector<std::shared_ptr<Panel>> mPanels; ///< Panels in the dockspace.

        /*!
         * \brief Render the menu bar.
         */
        void RenderMenuBar();

        /*!
         * \brief Render the tool bar.
         */
        void RenderToolBar();

        ///*!
        // * \brief Render the gizmo.
        // */
        //void RenderGizmo();

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

        /*!
         * \brief Processes camera zoom.
         */
        void ZoomCamera();

        /*!
         * \brief Processes camera panning.
         */
        void PanCamera();
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_EDITORLAYER_H
