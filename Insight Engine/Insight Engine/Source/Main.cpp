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
#include "EditorLayer.h"
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
    //you can now create entities
    Entity newEntity = engine.CreateEntityWithComponents<Position>();
    auto& pos = engine.GetComponent<Position>(newEntity);
    pos.x = 1;
    pos.y = 2;
    engine.SaveToJson(newEntity, "testentity");

    //this is how to clone an entity..
    Entity entity2 = newEntity;
    //this is loading entity from Json
    Entity entity3 = engine.LoadFromJson("testentity");
    //destroy entities
    if (!engine.HasComponent<Position>(newEntity)) {
        //get the component Position
        auto& pos = engine.GetComponent<Position>(newEntity);
        pos.x = 3;
        pos.y = 4;
        //remove the component for some reason
        engine.RemoveComponent<Position>(newEntity);
        engine.DestroyEntity(newEntity);
    }
    else {
        IS_CORE_INFO("TYPE DETECTED!");
    }
    
    //this is to show that entity 3 will give the saved value
    //can delete after
    if (engine.HasComponent<Position>(entity3)) {
        auto& pos = engine.GetComponent<Position>(entity3);
        std::cout <<"POSX" << pos.x << std::endl;
        std::cout <<"POSY" << pos.y << std::endl;
    }

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



    /* adding components to the systems
       your system is going to be made out of different components
       these components will add together to give you a signature
       we are using shared pointers for the system */
    auto InputSys = std::make_shared<InputManager>();
    //this function will give you the signature value (basically what your system contains)
    Signature signatureValue = engine.GenerateSignature<Position, Velocity>();
    //this function will let you add the value inside
    engine.AddSystem(InputSys, signatureValue);
    engine.PushLayer(new EditorLayer());


    // This is the end of the Initialize() part of your systems

    /*
    I will now demostrate how a Update loop will be 
    eg. This is in a physics system's update loop
    Update( float deltatime ) {
        //loops through all Entities registered by the System this mEntities map is written in the parent class
        for (auto const& entity : mEntities) {
            //if the entity has a certain component do something with it
            if (InsightEngine::Instance().HasComponent<Position>(entity)) {
                //do something;
            }
            if (InsightEngine::Instance().HasComponent<Velocity>(entity)) {
                //do something
            }
        }

    }

    Of course, you can do something like using InsightEngine& Engine= InsightEngine::Instance() to make life easier.
    Then it will just be Engine.HasComponent<Position>(entity){
    
    }
    */
    

    //run engine (GAME LOOP)
    engine.SetFPS(60);//set fps to wtv
    engine.Run();
   
    //engine stops technically don't need this since destructor is there and my engine is static
    engine.DestroyAllSystems();

    return 0;
}