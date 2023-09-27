/*!
 * \file InsightEngine.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 *         Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This  is to register systems and components. We just make it look neater
 * by abstracting whats needed and creating a single run function to run the engine.
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_INSIGHTENGINE_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_INSIGHTENGINE_H_

#include "CoreEngine.h"
#include "Graphics.h"
#include "Input.h"
#include "WindowSystem.h"
#include "GUI.h"
#include "GameLoop.h"

using namespace IS;

//This is a helper function to register all components
void RegisterComponents() {
    InsightEngine& engine = InsightEngine::Instance();

    // Register Components
    engine.RegisterComponent<Sprite>();
    engine.RegisterComponent<Transform>();
    engine.RegisterComponent<RigidBody>();
    engine.RegisterComponent<InputAffector>();

}

// This is a helper function to register all systems
void RegisterSystems() {
    InsightEngine& engine = InsightEngine::Instance();

    // Signature (components registered for each system)
    Signature sign_default = engine.GenerateSignature<>();
    Signature sign_input = engine.GenerateSignature<InputAffector>();
    Signature sign_physics = engine.GenerateSignature<Transform, RigidBody>();
    Signature sign_graphics = engine.GenerateSignature<Sprite, Transform>();

    // Register each system to Insight Engine
    auto insight_window = std::make_shared<WindowSystem>();
    auto insight_input = std::make_shared<InputManager>();
    insight_input->setCenterPos(insight_window->GetWidth() / 2.f, insight_window->GetHeight() / 2.f); // offset mouse position
    auto insight_audio = std::make_shared<ISAudio>();
    auto insight_asset = std::make_shared<AssetManager>();
    auto insight_physics = std::make_shared<Physics>();
    auto insight_graphics = std::make_shared<ISGraphics>();
    auto insight_gui = std::make_shared<GUISystem>();
    engine.AddSystem(insight_window, sign_default);
    engine.AddSystem(insight_input, sign_input);
    engine.AddSystem(insight_audio, sign_default);
    engine.AddSystem(insight_asset, sign_default);
    engine.AddSystem(insight_physics, sign_physics);
    engine.AddSystem(insight_graphics, sign_graphics);
    engine.AddSystem(insight_gui, sign_default);
    auto insight_gameloop = std::make_shared<GameLoop>(); // Always added last
    engine.AddSystem(insight_gameloop, sign_default);

}

//This is a function to setup the engine correctly
void EngineSetup() {
    RegisterComponents();
    RegisterSystems();
}

// The only function that main should ever call
void RunInsightEngine() {
    InsightEngine& engine = InsightEngine::Instance();
    //This is to set the flow of the engine
    EngineSetup();
    engine.Run();
    IS_CORE_WARN("Insight Engine has terminated!");
}


#endif