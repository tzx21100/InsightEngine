#ifndef GAM200_INSIGHT_ENGINE_ENGINE_CORE_APPLICATION_H
#define GAM200_INSIGHT_ENGINE_ENGINE_CORE_APPLICATION_H

#include "Core.h"
#include "Editor/Layers/LayerStack.h"
#include "Engine/Systems/Window/WindowSystem.h"

namespace IS {

    class IS_API Application
    {
    public:
        using LayerType = LayerStack::value_type;

        Application();
        virtual ~Application() = default;

        void Run();
        void PushLayer(LayerType layer);
        void PushOverlay(LayerType overlay);

        inline static Application& Get() { return *mInstance; }

    private:
        bool mRunning = false;
        LayerStack mLayers;
        static Application* mInstance;
    };

    Application* CreateApplication();

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_ENGINE_CORE_APPLICATION_H
