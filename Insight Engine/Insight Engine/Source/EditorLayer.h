/*!
 * \file EditorLayer.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 *      This header file declares the interface for class EditorLayer which
 *      encapsulates the functionalities of a level editor layer.
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

    // Encapsulate the editor layer
    class EditorLayer : public Layer {
    public:
        EditorLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float dt) override;
        void OnRender() override;

        void RenderScenePanel();
        void RenderPerformancePanel();
        void RenderLogConsolePanel();
        void RenderSceneHierarchyPanel();
        void RenderSceneOverlay();

    private:
        Vec2 mDockspacePosition;
        Vec2 mScenePanelSize;
        SceneHierarchyPanel mSceneHierarchyPanel;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_EDITORLAYER_H
