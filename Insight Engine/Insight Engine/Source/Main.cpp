///////////////////////////////////////////////////////////////////////////////////////
//
//	Main
//	The main entry point for the game--everything starts here.
//	
//	Authors:  Tan Zheng Xun
//	Copyright 2023, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
//pch has to go to the top of every cpp
#include "Pch.h"

#include "Graphics.h"
#include "Input.h"
#include "CoreEngine.h"
#include "WindowSystem.h"
#include "GUI.h"
#include "MemoryLeakCheck.h"
#include "GameLoop.h"

using namespace IS;

int main() {
    // Enable memory leaks check
    ENABLE_MEMORY_CHECK();
    // Initialize ScriptEngine
    //ScriptEngine::Init();
    // Initialize log
    Log::Init();

    // Engine Instance
    InsightEngine& engine = InsightEngine::Instance();

    // Register Components
    engine.RegisterComponent<Sprite>();
    engine.RegisterComponent<Transform>();
    engine.RegisterComponent<RigidBody>();
    engine.RegisterComponent<InputAffector>();
 
    // Signature (components registered for each system)
    Signature sign_default  = engine.GenerateSignature<>();
    Signature sign_input    = engine.GenerateSignature<InputAffector>();
    Signature sign_physics  = engine.GenerateSignature<Transform, RigidBody>();
    Signature sign_graphics = engine.GenerateSignature<Sprite, Transform>();

    // Register each system to Insight Engine
    auto insight_window   = std::make_shared<WindowSystem>();
    auto insight_input    = std::make_shared<InputManager>();
    insight_input->setCenterPos(insight_window->GetWidth() / 2.f, insight_window->GetHeight() / 2.f); // offset mouse position
    auto insight_audio    = std::make_shared<ISAudio>();
    auto insight_asset    = std::make_shared<AssetManager>();
    auto insight_physics  = std::make_shared<Physics>();
    auto insight_graphics = std::make_shared<ISGraphics>();
    auto insight_gui      = std::make_shared<GUISystem>();
    engine.AddSystem(insight_window, sign_default);
    engine.AddSystem(insight_input, sign_input);
    engine.AddSystem(insight_audio, sign_default);
    engine.AddSystem(insight_asset, sign_default);
    engine.AddSystem(insight_physics, sign_physics);
    engine.AddSystem(insight_graphics, sign_graphics);
    engine.AddSystem(insight_gui, sign_default);
    auto insight_gameloop = std::make_shared<GameLoop>(); // Always added last
    engine.AddSystem(insight_gameloop, sign_default);

    //run engine (GAME LOOP)
    engine.SetFPS(144);//set fps to wtv
    engine.Run();
   
    //engine stops technically don't need this since destructor is there and my engine is static
    engine.DestroyAllSystems();
    IS_CORE_WARN("Insight Engine has terminated!");

    return 0;
}