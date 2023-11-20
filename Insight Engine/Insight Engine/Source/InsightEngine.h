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
#include "GameLoop.h"
#include "ScriptEngine.h"
#include "AIFSM.h"
#include "ScriptManager.h"
#include "GameGui.h"
#include "Pathfinder.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "Physics.h"
#include "CollisionSystem.h"

using namespace IS;


//This is a helper function to register all components
void RegisterComponents() {
    InsightEngine& engine = InsightEngine::Instance();

    // Register Components
    engine.RegisterComponent<Sprite>();
    engine.RegisterComponent<Transform>();
    engine.RegisterComponent<RigidBody>();
    engine.RegisterComponent<InputAffector>();
    engine.RegisterComponent<AIState>();
    engine.RegisterComponent<ScriptComponent>();
    engine.RegisterComponent<ButtonComponent>();
    engine.RegisterComponent<Pathfinder>();
    engine.RegisterComponent<AudioEmitter>();
    engine.RegisterComponent<AudioListener>();
    engine.RegisterComponent<Collider>();
    engine.RegisterComponent<ParticleEmitter>();

}

// This is a helper function to register all systems
void RegisterSystems() {
    InsightEngine& engine = InsightEngine::Instance();

    // Signature (components registered for each system)
    Signature sign_default = engine.GenerateSignature<>();
    Signature sign_input = engine.GenerateSignature<InputAffector>();
    Signature sign_physics = engine.GenerateSignature<Transform, RigidBody>();
    Signature sign_graphics = engine.GenerateSignature<Sprite, Transform>();
    Signature sign_fsm = engine.GenerateSignature<AIState>();
    Signature sign_script = engine.GenerateSignature<ScriptComponent>();
    Signature sign_gui = engine.GenerateSignature<ButtonComponent>();
    Signature sign_pathfinding = engine.GenerateSignature<Pathfinder>();
    Signature sign_audio = engine.GenerateSignature<AudioListener, Transform>();
    Signature sign_audio_emitter = engine.GenerateSignature<AudioEmitter, Transform>();
    Signature sign_collision = engine.GenerateSignature<Transform, Collider>();

    Signature sign_particle = engine.GenerateSignature<ParticleEmitter>();

    // Register each system to Insight Engine
    auto insight_window = std::make_shared<WindowSystem>();
    auto insight_input = std::make_shared<InputManager>(insight_window);
    auto insight_audio = std::make_shared<ISAudio>();
    auto insight_audio_emitter = std::make_shared<AudioEmitterSystem>();
    auto insight_asset = std::make_shared<AssetManager>();
    auto insight_physics = std::make_shared<Physics>();
    auto insight_graphics = std::make_shared<ISGraphics>();
    auto insight_fsm = std::make_shared<AIFSMManager>();
    auto insight_scriptmanager = std::make_shared<ScriptManager>();
    auto insight_guisystem = std::make_shared<GuiSystem>();
    auto insight_pathfinding = std::make_shared<Pathfinding>();
    auto insight_collision = std::make_shared<CollisionSystem>();
    auto insight_particle = std::make_shared <ParticleSystem> ();


    engine.AddSystem(insight_window, sign_default);
    engine.AddSystem(insight_input, sign_input);
    engine.AddSystem(insight_audio, sign_audio);
    engine.AddSystem(insight_audio_emitter, sign_audio_emitter);
    engine.AddSystem(insight_asset, sign_default);
    engine.AddSystem(insight_scriptmanager, sign_script);
    engine.AddSystem(insight_physics, sign_physics);
    engine.AddSystem(insight_collision, sign_collision);
    engine.AddSystem(insight_graphics, sign_graphics);
    engine.AddSystem(insight_fsm, sign_fsm);
    engine.AddSystem(insight_guisystem, sign_gui);
    engine.AddSystem(insight_pathfinding, sign_pathfinding);
    engine.AddSystem(insight_particle, sign_particle);
    auto insight_gameloop = std::make_shared<GameLoop>(); // Always added last
    engine.AddSystem(insight_gameloop, sign_default);

}

//This is a function to setup the engine correctly
void EngineSetup() {
    // Initialize log
    ScriptEngine::Init();
    Log::Init(true);
    RegisterComponents();
    RegisterSystems();
}

// The only function that main should ever call
void RunInsightEngine() {
    //This is to set the flow of the engine
    EngineSetup();
    InsightEngine::Instance().Run();
    ScriptEngine::Shutdown();
    IS_CORE_WARN("Insight Engine has terminated!");
}


#endif