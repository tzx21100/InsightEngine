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

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
    ScriptEngine::Init();
    // Initialize log
    Log::init();

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
    engine.AddSystem(std::make_shared<WindowSystem>(), sign_default);
    engine.AddSystem(std::make_shared<InputManager>(), sign_input);
    engine.AddSystem(std::make_shared<ISAudio>(), sign_default);
    engine.AddSystem(std::make_shared<AssetManager>(), sign_default);
    engine.AddSystem(std::make_shared<Physics>(), sign_physics);
    engine.AddSystem(std::make_shared<ISGraphics>(), sign_graphics);
#ifdef USING_IMGUI
    engine.AddSystem(std::make_shared<GUISystem>(), sign_default);
#endif // USING_IMGUI
    engine.AddSystem(std::make_shared<GameLoop>(), sign_default); // Always added last

    //run engine (GAME LOOP)
    engine.SetFPS(60);//set fps to wtv
    engine.Run();
   
    //engine stops technically don't need this since destructor is there and my engine is static
    engine.DestroyAllSystems();
    IS_CORE_WARN("Insight Engine has terminated!");

    return 0;
}