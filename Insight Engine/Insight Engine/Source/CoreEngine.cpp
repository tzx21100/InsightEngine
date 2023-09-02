//pch has to go to the top of every cpp
#include "Pch.h"
#include "CoreEngine.h" // Include the header file
#include <iostream>
#include <thread>

namespace IS {

    //Basic constructor and setting base FPS to 60 
    InsightEngine::InsightEngine() : is_running(false), last_runtime(0), targetFPS(60) {}

    //destructor will delete all systems and clear it. I have a destroyallsystem function but this is just in case.
    InsightEngine::~InsightEngine() {
        for (auto& pair : all_systems) {
            delete pair.second;
        }
        all_systems.clear();
    }

    //moved all the engine stuff under this run function
    void InsightEngine::Run() {

        //initialize all systems first
        InitializeAllSystems();
        is_running = true;
        
        //this is the game loop
        while (is_running) {

            //i get the start time 
            auto frameStart = std::chrono::high_resolution_clock::now();

            //by passing in the start time, we can limit the fps here by sleeping until the next loop and get the time after the loop
            auto frameEnd=LimitFPS(frameStart);

            /*This is the delta time I'm explaining it to my future self 
            because frameStart and frameEnd are types std::chrono::high_resolution_clock::time_point
            deducting them will give me a time value which i will then cast it to microseconds with 
            duration_cast<std::chrono::microseconds>. Lastly, .count() just turns it into integers
            This gives us how many microseconds that have passed in int terms.
            */
            int deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart).count();

            //looping through the map and updating
            for (const auto& pair : all_systems) {
                pair.second->Update(deltaTime);  // Pass the actual delta time here so all systems can use it
            }

            //Messages can be passed here with the event manager here is an example
            Message testMessage(MessageType::DebugInfo);
            //every system that has subscribed to DebugInfo will now recieve this message and do the relavent actions
            BroadcastMessage(&testMessage);

        }
    }

    //This function will add a system to the map with the key being whatever the system defined it to be
    void InsightEngine::AddSystem(ParentSystem* system) {
        all_systems[system->getName()] = system;
    }

    //This function is meant to specifically find the individual system in the map and destroy it
    void InsightEngine::DestroySystem(const std::string& name) {
        auto it = all_systems.find(name);
        if (it != all_systems.end()) {
            delete it->second; //delete the object
            all_systems.erase(it);
        }
    }

    //This function will destroy all systems and clear it from the map
    void InsightEngine::DestroyAllSystems() {
        for (auto& pair : all_systems) {
            delete pair.second;  // Delete the system object
        }
        all_systems.clear();  // Clear the map
    }

    //loop through all the systems stored
    void InsightEngine::InitializeAllSystems() {
        for (const auto& pair : all_systems) {
            pair.second->Initialize();
        }
    }

    //This will use the Event Manager and broadcast the message to relavent systems that are subscribed to that message
    void InsightEngine::BroadcastMessage(Message* message) {
        eventManager.Broadcast(*message);
    }

    //limit fps will return the frameEnd time now so i can use to find delta time
    std::chrono::high_resolution_clock::time_point InsightEngine::LimitFPS(const std::chrono::high_resolution_clock::time_point& frameStart) {
        const std::chrono::milliseconds targetFrameTime(1000 / targetFPS);
        //i used auto for now not sure if its fully allowed
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

        if (frameDuration < targetFrameTime) {
            std::this_thread::sleep_for(targetFrameTime - frameDuration);
        }

        return std::chrono::high_resolution_clock::now(); // Return the updated frame end time
    }

    //This is a simple function to set the FPS
    void InsightEngine::SetFPS(int FPS) {
        targetFPS = FPS;
    }
}
