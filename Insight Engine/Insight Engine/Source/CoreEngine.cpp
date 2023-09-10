//pch has to go to the top of every cpp
#include "Pch.h"
#include "CoreEngine.h" // Include the header file
#include "JsonSaveLoad.h" // This is for Saving and Loading
#include <iostream>
#include <thread>
#include <GLFW/glfw3.h>
#include "Timer.h"


namespace IS {

    //Basic constructor and setting base FPS to 60 
    InsightEngine::InsightEngine() : is_running(false), last_runtime(0), targetFPS(60) {
        IS_CORE_TRACE("Starting Insight Engine...");
        //create the pointers to the managers
        mComponentManager = std::make_unique<ComponentManager>();
        mEntityManager = std::make_unique<EntityManager>();
        mSystemManager = std::make_unique<SystemManager>();
        IS_CORE_DEBUG("Insight Engine started");
    }

    //handling messages
    void InsightEngine::HandleMessage(const Message& message) {
        //handling quit message
        if (message.GetType() == MessageType::Quit) {
            is_running = false;
        }
    }

    //destructor will delete all systems and clear it. I have a destroyallsystem function but this is just in case.
    InsightEngine::~InsightEngine() {
        for (auto& pair : mAllSystems) {
           // delete pair.second;
            IS_CORE_INFO("{} terminated", pair.second->getName());
        }
        mAllSystems.clear();
        layers.clearStack();
        IS_CORE_DEBUG("Insight Engine shutdown");
    }

    void InsightEngine::Initialize() {

        //initialize all systems first
        InitializeAllSystems();
        //subscirbe to messages
        Subscribe(MessageType::Quit);
        //run the game
        is_running = true;

        gui_layer = new GUILayer();
        PushOverlay(gui_layer);
    }

    void InsightEngine::Update() {
        //i get the start time 
        auto frameStart = std::chrono::high_resolution_clock::now();

        //looping through the map and updating
        
        glfwPollEvents();

        // Update System deltas every 6s
        const int update_frequency = 6 * targetFPS;
        if (!(frame_count % update_frequency))
            mSystemDeltas["Engine"] = 0;

        for (const auto& [name, system] : mAllSystems) {
            Timer timer(name.c_str());
            system->Update(delta_time.count());
            timer.Stop();

            if (!(frame_count % update_frequency)) {
                mSystemDeltas[name] = timer.GetDeltaTime();
                mSystemDeltas["Engine"] += timer.GetDeltaTime();
            }
        }

        { // Scoped so timer will be destroyed out of scope
            Timer timer("Layers");
            gui_layer->Begin();
            layers.Update(delta_time.count());
            layers.Render();
            gui_layer->End();
            timer.Stop();
            if (!(frame_count % update_frequency)) {
                mSystemDeltas["UI"] = timer.GetDeltaTime();
                mSystemDeltas["Engine"] += timer.GetDeltaTime();
            }
        }

        // Swap front and back buffers
        GLFWwindow* window = glfwGetCurrentContext();
        glfwSwapBuffers(window);

        //by passing in the start time, we can limit the fps here by sleeping until the next loop and get the time after the loop
        auto frameEnd = LimitFPS(frameStart);

        delta_time = frameEnd - frameStart;

        ++frame_count;
    }

    //moved all the engine stuff under this run function
    void InsightEngine::Run() {
        Initialize();
        //this is the game loop
        while (is_running) {
            Update();
        }
    }

    void InsightEngine::Exit() {
        Message quit = Message(MessageType::Quit);
        EventManager::Instance().Broadcast(quit);
    }

    void InsightEngine::PushLayer(Layer* layer) {
        layers.pushLayer(layer);
        layer->onAttach();
    }

    void InsightEngine::PushOverlay(Layer* overlay) {
        layers.pushOverlay(overlay);
        overlay->onAttach();
    }

    void InsightEngine::PopLayer(Layer* layer) {
        layers.popLayer(layer);
        layer->onDetach();
    }

    void InsightEngine::PopOverlay(Layer* overlay) {
        layers.popOverlay(overlay);
        overlay->onDetach();
    }

    //This function will add a system to the map with the key being whatever the system defined it to be
    void InsightEngine::AddSystem(std::shared_ptr<ParentSystem> system ,Signature signature) {
        std::string systemName = system->getName();
        IS_CORE_TRACE("Registering system... {}", systemName);
        mAllSystems[systemName] = system;
        mSystemManager->RegisterSystem(system);
        mSystemManager->SetSignature(systemName,signature);
    }

    //This function is meant to specifically find the individual system in the map and destroy it
    void InsightEngine::DestroySystem(const std::string& name) {
        auto it = mAllSystems.find(name);
        if (it != mAllSystems.end()) {
            //delete it->second; //delete the object
            mAllSystems.erase(it);
        }
    }

    //This function will destroy all systems and clear it from the map
    void InsightEngine::DestroyAllSystems() {
        for (auto& [key, system] : mAllSystems) {
            //delete pair.second;  // Delete the system object
            IS_CORE_INFO("{} terminated", system->getName());
        }
        mAllSystems.clear();  // Clear the map
    }

    //loop through all the systems stored
    void InsightEngine::InitializeAllSystems() {
        for (auto const& [key, system] : mAllSystems) {
            system->Initialize();
            IS_CORE_INFO("{} initialized", system->getName());
        }
    }

    // Accessor for system deltas
    std::unordered_map<std::string, float> const& InsightEngine::GetSystemDeltas() const {
        return mSystemDeltas;
    }

    // Get frame count
    unsigned InsightEngine::FrameCount() const {
        return frame_count;
    }

    //limit fps will return the frameEnd time now so i can use to find delta time
    std::chrono::high_resolution_clock::time_point InsightEngine::LimitFPS(const std::chrono::high_resolution_clock::time_point& frameStart) {
        const std::chrono::nanoseconds targetFrameTime(1'000'000'000 / targetFPS);  // Nanosecond-level precision because accurate
        while (true) { // We chill in this loop until true
            auto now = std::chrono::high_resolution_clock::now();
            auto frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameStart);
            if (frameDuration >= targetFrameTime) {
                return now;  // Return the updated frame end time
            }
        }
    }

    //This is a simple function to set the FPS
    void InsightEngine::SetFPS(int FPS) {
        targetFPS = FPS;
    }

    //These functions involve creating entities and destroying them
    Entity InsightEngine::CreateEntity() { 
        return mEntityManager->CreateEntity(); 
    }

    void InsightEngine::DestroyEntity(Entity entity) {
        mEntityManager->DestroyEntity(entity);
        mComponentManager->EntityDestroyed(entity);
        mSystemManager->EntityDestroyed(entity);
    }

    void InsightEngine::SaveToJson(Entity entity, std::string filename) {
        std::string file_path = "Prefabs/" + filename;
        std::string signature = mEntityManager->GetSignature(entity).to_string();
        Json::Value prefab;
        prefab["Signature"] = signature;
        if (HasComponent<RigidBody>(entity)) {
            auto& rigidbody = GetComponent<RigidBody>(entity);
            prefab["RigidBody"] = rigidbody.Serialize();
        }

        if (HasComponent<Position>(entity)) {
            auto& pos = GetComponent<Position>(entity);
            prefab["POS"] = pos.Serialize();
        }

        SaveJsonToFile(prefab,file_path);
    }

    Entity InsightEngine::LoadFromJson(std::string name) {
        std::string filename = "Prefabs/" + name;
        Entity entity = CreateEntity();
        Json::Value loaded;
        LoadJsonFromFile(loaded, filename);
        if (loaded.isMember("POS")) {
            AddComponent<Position>(entity,Position());
            auto& pos=GetComponent<Position>(entity);
            pos.Deserialize(loaded["POS"]);
        }
        return entity;

    }

}


