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
        IS_CORE_DEBUG("Starting Insight Engine...");
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
        mSystemList.clear();
        mAllSystems.clear();
        //IS_CORE_DEBUG("Insight Engine terminated");
    }

    void InsightEngine::Initialize() {

        //initialize all systems first
        InitializeAllSystems();
        //subscirbe to messages
        Subscribe(MessageType::Quit);
        //run the game
        is_running = true;
    }

    void InsightEngine::Update() {
        //i get the start time 
        auto frameStart = std::chrono::high_resolution_clock::now();

        //looping through the map and updating
        
        glfwPollEvents();

        // Update System deltas every 4s
        const int update_frequency = 4 * targetFPS;
        if (!(frame_count % update_frequency))
            mSystemDeltas["Engine"] = 0;

        for (const auto& system: mSystemList) {
            Timer timer(system->getName().c_str(), false);
            system->Update(delta_time.count());
            timer.Stop();

            if (!(frame_count % update_frequency)) {
                mSystemDeltas[system->getName()] = timer.GetDeltaTime();
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

    //This function will add a system to the map with the key being whatever the system defined it to be
    void InsightEngine::AddSystem(std::shared_ptr<ParentSystem> system ,Signature signature) {
        std::string systemName = system->getName();
        IS_CORE_TRACE("Registering system... {}", systemName);
        mAllSystems[systemName] = system;
        mSystemList.emplace_back(system);
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
        int i = 0;
        for (auto& system : mSystemList) {
            if (system == it->second) {
                mSystemList.erase(mSystemList.begin()+i);
            }
            i++;
        }

    }

    //This function will destroy all systems and clear it from the map
    void InsightEngine::DestroyAllSystems() {
        mSystemList.clear();
        mAllSystems.clear();  // Clear the map
    }

#pragma warning(push)
#pragma warning(disable: 4456)
    //loop through all the systems stored
    void InsightEngine::InitializeAllSystems() {
        IS_PROFILE_FUNCTION();
        for (auto const& system : mSystemList) {
            IS_PROFILE_SCOPE(system->getName().c_str());
            system->Initialize();
            //IS_CORE_INFO("{} initialized", name);
        }
    }
#pragma warning(pop)

    // Accessor for system deltas
    std::unordered_map<std::string, float> const& InsightEngine::GetSystemDeltas() const {
        return mSystemDeltas;
    }
    // Accessor for system pointers
    std::unordered_map<std::string, std::shared_ptr<ParentSystem>> const& InsightEngine::GetSystemPointer() const {
        return mAllSystems;
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
        mComponentManager->EntityDestroyed(entity);
        mEntityManager->DestroyEntity(entity);
        mSystemManager->EntityDestroyed(entity);
        IS_CORE_DEBUG("Entity {} completely destroyed!", entity);
    }

    void InsightEngine::GenerateRandomEntity() {
        PRNG prng;
        InsightEngine& engine = Instance();
        Entity e = engine.CreateEntityWithComponents<Sprite, Transform>();
        auto& trans = engine.GetComponent<Transform>(e);
        // scale [2, 16], pos [viewport], orientation [-360, 360]
        trans.setScaling((prng.generate() * 18.f) + 2.f, (prng.generate() * 18.f) + 2.f);
        trans.setWorldPosition((prng.generate()* WIDTH) - WIDTH / 2.f, (prng.generate()* HEIGHT) - HEIGHT / 2.f);
        trans.setRotation((prng.generate() * 720.f) - 360.f);
    }

    void InsightEngine::SaveToJson(Entity entity, std::string filename) {
        std::string file_path = "Prefabs/" + filename + ".json";
        std::string signature = mEntityManager->GetSignature(entity).to_string();
        Json::Value prefab;
        prefab["Signature"] = signature;
        
        //add in future components
       // SerializeComponent<Position>(entity, prefab, "POS");

        SaveJsonToFile(prefab,file_path);
    }

    Entity InsightEngine::LoadFromJson(std::string name) {
        std::string filename = "Prefabs/" + name + ".json";
        Entity entity = CreateEntity();
        Json::Value loaded;
        LoadJsonFromFile(loaded, filename);

        //add in future components
       // DeserializeComponent<Position>(entity, loaded, "POS");
        return entity;

    }

}


