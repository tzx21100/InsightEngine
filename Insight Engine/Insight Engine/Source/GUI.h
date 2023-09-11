#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_SYSTEM_GUI_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_SYSTEM_GUI_H

#include "System.h"
#include "Layer.h"
#include "LayerStack.h"

namespace IS {

    class GUISystem : public ParentSystem {
    public:
        GUISystem();
        ~GUISystem() override;

        void Initialize() override;
        void Update(float delta_time) override;
        void Terminate();
        void HandleMessage(Message const& message) override;
        std::string getName() override;

        // Ran only once
        void Begin();
        void End();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        void SetDarkThemeColors() const;
    private:        
        LayerStack layers;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_SYSTEM_GUI_H
