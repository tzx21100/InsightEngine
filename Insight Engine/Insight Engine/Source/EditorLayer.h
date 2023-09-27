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
#include "SceneHierarchyPanel.h"

// Dependencies
#include <imgui.h>

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
         * \brief Render the menu bar.
         */
        void RenderMenuBar();

        /*!
         * \brief Render the scene panel in the editor interface.
         */
        void RenderScenePanel();

        /*!
         * \brief Render the performance panel in the editor interface.
         */
        void RenderPerformancePanel();

        /*!
         * \brief Render the log console panel in the editor interface.
         */
        void RenderLogConsolePanel();

        /*!
         * \brief Render the scene hierarchy panel in the editor interface.
         */
        void RenderSceneHierarchyPanel();

        /*!
         * \brief Render an overlay on top of the scene in the editor interface.
         */
        void RenderSceneOverlay();

    private:
        Vec2 mDockspacePosition;               ///< Position of the dockspace in the editor.
        Vec2 mScenePanelSize;                  ///< Size of the scene panel.
        SceneHierarchyPanel mSceneHierarchyPanel; ///< Instance of the scene hierarchy panel.
        bool show_load = false;
        bool show_save = false;

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
        void LoadScene();

        /*!
         * \brief Save current scene.
         */
        void SaveScene();

        /*!
         * \brief Save current scene as.
         */
        void SaveSceneAs();
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_EDITORLAYER_H
