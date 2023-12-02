#include "Pch.h"
#include "Application.h"
#include "InsightEngine.h"

namespace IS {

    Application::Application()
    {
        IS_ASSERT_MESG(mInstance, "Application already exists!");
        mInstance = this;
    }

    void Application::Run()
    {
        RunInsightEngine();
    }

    void Application::PushLayer(LayerType layer)
    {
        mLayers.PushLayer(layer);
    }

    void Application::PushOverlay(LayerType overlay)
    {
        mLayers.PushOverlay(overlay);
    }

}
