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
#include "SceneHierarchyPanel.h"

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
         * \param dt The time elapsed since the last frame (in seconds).
         */
        void OnUpdate(float dt) override;

        /*!
         * \brief Called for rendering the editor scene panel.
         */
        void OnRender() override;

        /*!
         * \brief Get the dockspace position.
         * \return Dockspace position.
         */
        static Vec2 GetDockspacePosition();

        /*!
         * \brief Load a scene.
         *
         * \param path Path to the scene.
         */
        void OpenScene(std::string const& path);

    private:
        static Vec2 mDockspacePosition; ///< Position of the dockspace in the editor.
        std::shared_ptr<SceneHierarchyPanel> mSceneHierarchyPanel; ///< Instance of scene hierarchy panel.
        std::vector<std::shared_ptr<Panel>> mPanels; ///< Panels in the dockspace.

        std::string mActiveScene = "emptyscene"; ///< Name of the active scene.

        // Internal Flags
        bool mShowNewScene = false; ///< Flag indicating to show new scene.
        bool mShowNewScript = false; ///< Flag indicating to show new script.

        /*!
         * \brief Render the menu bar.
         */
        void RenderMenuBar();

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
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_EDITORLAYER_H
