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


using namespace IS;

int main() {
    ENABLE_MEMCHECK;

    // Initialize log
    Log::init();

    //engine get
    InsightEngine& engine = InsightEngine::Instance();


    //The following demonstrates the Initialize() Part of your systems
    
    //this is just to show how the new system works everthing can be deleted
    //register the position component
    engine.RegisterComponent<Position>();
    engine.RegisterComponent<Velocity>();
    engine.RegisterComponent<RigidBody>();
    engine.RegisterComponent<Transform>();
    engine.RegisterComponent<Sprite>();

    //this just loads in the window and audio I will give them their components next time
    Signature signature;
    // create window first so other systems can just point to current context
    auto mySystem = std::make_shared<WindowSystem>(WIDTH, HEIGHT, "Insight Engine");
    signature = engine.GenerateSignature<Position, Velocity>();
    engine.AddSystem(mySystem, signature);
    auto mySystem3 = std::make_shared<ISAudio>();
    engine.AddSystem(mySystem3, signature);
    auto mySystem4 = std::make_shared<ISAsset>();
    engine.AddSystem(mySystem4, signature);
    auto gui = std::make_shared<GUISystem>();
    engine.AddSystem(gui, signature);
    auto physicsSystem = std::make_shared<Physics>();
    signature = engine.GenerateSignature<RigidBody, Transform>();
    engine.AddSystem(physicsSystem, signature);
    signature = engine.GenerateSignature<Transform, Sprite>();
    auto graphicSystem = std::make_shared<ISGraphics>();
    engine.AddSystem(graphicSystem, signature);
    /* adding components to the systems
       your system is going to be made out of different components
       these components will add together to give you a signature
       we are using shared pointers for the system */
    auto InputSys = std::make_shared<InputManager>();
    //this function will give you the signature value (basically what your system contains)
    Signature signatureValue = engine.GenerateSignature<Position, Velocity>();
    //this function will let you add the value inside
    engine.AddSystem(InputSys, signatureValue);

    //showing you how to create your entity

    /*          YAN KHANG LOOK HERE      */

    //creating the entity (i might make the transform a compulsory component)
    Entity testImage = engine.CreateEntityWithComponents<Sprite>();
    Entity testImage2 = engine.CreateEntityWithComponents<Sprite>();
    auto& model = engine.GetComponent<Sprite>(testImage2);
    model.model = ModelType::Point;


    //run engine (GAME LOOP)
    engine.SetFPS(60);//set fps to wtv
    engine.Run();
   
    //engine stops technically don't need this since destructor is there and my engine is static
    engine.DestroyAllSystems();

    return 0;
}