#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_LAYERS_EDITORLAYER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_LAYERS_EDITORLAYER_H

#include "Layer.h"
#include <cstdarg>

// Dependencies
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace IS {

    // Encapsulate the editor layer
    class EditorLayer : public Layer {
    public:
        EditorLayer();

        void onAttach() override;
        void onDetach() override;
        void onUpdate(float dt) override;
        void onRender() override;

        void RenderInspectorPanel();
        void RenderPerformancePanel();
        void RenderLogConsolePanel();
        void RenderScenePanel();
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_LAYERS_EDITORLAYER_H
