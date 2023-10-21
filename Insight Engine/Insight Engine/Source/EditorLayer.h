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
         * \brief Add panels.
         */
        void AddPanels();

        /*!
         * \brief Render the menu bar.
         */
        void RenderMenuBar();

        /*!
         * \brief Get the dockspace position.
         * \return Dockspace position.
         */
        static Vec2 GetDockspacePosition();

    private:
        static Vec2 mDockspacePosition; ///< Position of the dockspace in the editor.
        std::shared_ptr<SceneHierarchyPanel> mSceneHierarchyPanel; ///< Instance of scene hierarchy panel.
        std::vector<std::shared_ptr<Panel>> mPanels; ///< Panels in the dockspace.
        bool mShowLoad = false; ///< Flag indicating to show load scene.
        bool mShowSave = false; ///< Flag indicating to show save scene.
        bool mShowNewScript = false; ///< Flag indicating to show new script.
        bool mShowOpenScript = false; ///< Flag indicating to show save script.

        /*!
         * \brief Load a new scene.
         */
        void NewScene();

        /*!
         * \brief Load the test scene.
         */
        void LoadTestScene();

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
         * \brief Creates a new script.
         */
        void NewScript();

        /*!
         * \brief Open a script.
         */
        void OpenScript();

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
