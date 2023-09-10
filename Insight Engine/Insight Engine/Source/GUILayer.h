#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_GUILAYER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_GUILAYER_H

#include "Layer.h"

namespace IS {

    // Encapsulate the GUI layer
    class GUILayer : public Layer {
    public:
        GUILayer();

        void onAttach() override;
        void onDetach() override;
        void onUpdate(float dt) override;
        void onRender() override;

        // Ran only once
        void Begin();
        void End();

        void SetDarkThemeColors() const;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_GUILAYER_H
