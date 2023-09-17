#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_LAYERS_EDITORLAYER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_LAYERS_EDITORLAYER_H

#include "Layer.h"
#include "SceneHierarchyPanel.h"

// Dependencies
#include <imgui.h>

namespace IS {

    // Encapsulate the editor layer
    class EditorLayer : public Layer {
    public:
        EditorLayer();

        void onAttach() override;
        void onDetach() override;
        void onUpdate(float dt) override;
        void onRender() override;

        void RenderScenePanel();
        void RenderPerformancePanel();
        void RenderLogConsolePanel();
        void RenderSceneHierarchyPanel();
        void RenderSceneOverlay();

    private:
        Vec2 dockspace_pos;
        Vec2 scene_panel_size;
        SceneHierarchyPanel scene_hierarchy_panel;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_LAYERS_EDITORLAYER_H
