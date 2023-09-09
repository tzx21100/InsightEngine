#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_EDITORLAYER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_EDITORLAYER_H

#include "Layer.h"

namespace IS {

    // Encapsulate the editor layer
    class EditorLayer : public Layer {
    public:
        EditorLayer();

        void onAttach() override;
        void onDetach() override;
        void onUpdate(float dt) override;
        void onRender() override;

        // Probably a function for each window
        void RenderInspector();
        void RenderPerformanceViewer();
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_EDITORLAYER_H
