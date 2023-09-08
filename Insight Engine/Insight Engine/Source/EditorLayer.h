#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_EDITORLAYER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_EDITORLAYER_H

#include "Layer.h"

namespace IS {

    class EditorLayer : public Layer {
    public:
        EditorLayer();

        void onAttach() const override;
        void onDetach() const override;
        void onUpdate() const override;
        void onRender() const override;
        void Begin() const;
        void End() const;

        void SetDarkThemeColors() const;
        void EnableDockSpace() const;
        void RenderInspector() const;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_EDITORLAYER_H
