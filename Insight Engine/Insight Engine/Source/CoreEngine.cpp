//pch has to go to the top of every cpp
#include "Pch.h"
#include "CoreEngine.h" // Include the header file

namespace Zx_Framework {



    //moved all the engine stuff under this run function
    void InsightEngine::Run() {
        InitializeAllSystems();
        is_running = true;

        while (is_running) {
            auto frameStart = std::chrono::high_resolution_clock::now();

            for (const auto& pair : all_systems) {
                pair.second->Update(0);  // Pass the actual delta time here once done
            }

            // Handle messages, etc.

            LimitFPS(frameStart);
        }
    }

    void InsightEngine::AddSystem(ParentSystem* system) {
        all_systems[system->getName()] = system;
    }

    void InsightEngine::DestroySystem(const std::string& name) {
        auto it = all_systems.find(name);
        if (it != all_systems.end()) {
            delete it->second;
            all_systems.erase(it);
        }
    }

    void InsightEngine::DestroyAllSystems() {
        for (auto& pair : all_systems) {
            delete pair.second;  // Delete the system object
        }
        all_systems.clear();  // Clear the map
    }

    void InsightEngine::InitializeAllSystems() {
        for (const auto& pair : all_systems) {
            pair.second->Initialize();
        }
    }

    void InsightEngine::BroadcastMessage(Message* message) {
        eventManager.Broadcast(*message);
    }

    void InsightEngine::LimitFPS(const std::chrono::high_resolution_clock::time_point& frameStart) {
        const std::chrono::milliseconds targetFrameTime(1000 / targetFPS);

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

        if (frameDuration < targetFrameTime) {
            std::this_thread::sleep_for(targetFrameTime - frameDuration);
        }
    }

    void InsightEngine::SetFPS(int FPS) {
        targetFPS = FPS;
    }
}
