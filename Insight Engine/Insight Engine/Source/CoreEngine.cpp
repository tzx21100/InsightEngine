//pch has to go to the top of every cpp
#include "Pch.h"
#include "CoreEngine.h" // Include the header file
#include "JsonSaveLoad.h" // This is for Saving and Loading
#include <iostream>
#include <thread>
#include <GLFW/glfw3.h>


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
        for (auto& pair : all_systems) {
           // delete pair.second;
            IS_CORE_INFO("{} terminated", pair.second->getName());
        }
        all_systems.clear();
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

        // Very scuffed way
        if ((frame_count % 240) == 0)
            systemDeltas["Engine"] = 0;
        for (const auto& [name, system] : all_systems) {
            auto frameStart = std::chrono::high_resolution_clock::now();
            system->Update(deltaTime.count());  // Pass the actual delta time here so all systems can use it
            auto frameEnd= std::chrono::high_resolution_clock::now();
            float delta= std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart).count();
            if ((frame_count % 240) == 0) {
                systemDeltas[system->getName()] = delta;
                systemDeltas["Engine"] += delta;
            }
        }

        layers.Update(deltaTime.count());

        gui_layer->Begin();
        layers.Render();
        gui_layer->End();

        GLFWwindow* window = glfwGetCurrentContext();
        glfwSwapBuffers(window);

        //by passing in the start time, we can limit the fps here by sleeping until the next loop and get the time after the loop
        auto frameEnd = LimitFPS(frameStart);
        ++frame_count;

        /*This is the delta time I'm explaining it to my future self
        because frameStart and frameEnd are types std::chrono::high_resolution_clock::time_point
        deducting them will give me a time value which i will then cast it to microseconds with
        duration_cast<std::chrono::microseconds>. Lastly, .count() just turns it into integers
        This gives us how many microseconds that have passed in float terms.
        */
        // float deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart).count());
        deltaTime = frameEnd - frameStart;

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
        all_systems[systemName] = system;
        mSystemManager->RegisterSystem(system);
        mSystemManager->SetSignature(systemName,signature);
    }

    //This function is meant to specifically find the individual system in the map and destroy it
    void InsightEngine::DestroySystem(const std::string& name) {
        auto it = all_systems.find(name);
        if (it != all_systems.end()) {
            //delete it->second; //delete the object
            all_systems.erase(it);
        }
    }

    //This function will destroy all systems and clear it from the map
    void InsightEngine::DestroyAllSystems() {
        for (auto& [key, system] : all_systems) {
            //delete pair.second;  // Delete the system object
            IS_CORE_INFO("{} terminated", system->getName());
        }
        all_systems.clear();  // Clear the map
    }

    //loop through all the systems stored
    void InsightEngine::InitializeAllSystems() {
        for (auto const& [key, system] : all_systems) {
            system->Initialize();
            IS_CORE_INFO("{} initialized", system->getName());
        }
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
            //prefab["RigidBodyVelocityX"] = rigidbody.velocity.x;
            //prefab["RigidBodyVelocityY"] = rigidbody.velocity.y;
            //prefab["RigidBodyAccelerationX"] = rigidbody.acceleration.x;
            //prefab["RigidBodyAccelerationY"] = rigidbody.acceleration.y;
            //prefab["RigidBodyPositionX"] = rigidbody.position.x;
            //prefab["RigidBodyPositionY"] = rigidbody.position.y;
            //prefab["RigidBodyBodyType"] = static_cast<int>(rigidbody.bodyType);
            //prefab["RigidBodyGravity"] = rigidbody.gravity;
            //prefab["RigidBodyMass"] = rigidbody.mass;
            //prefab["RigidBodyFriction"] = rigidbody.friction;
            //prefab["RigidBodyRestitution"] = rigidbody.restitution;
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


