//pch has to go to the top of every cpp
#include "Pch.h"
#include "CoreEngine.h"   // Include the header file
#include "JsonSaveLoad.h" // This is for Saving and Loading
#include "WindowSystem.h" // Access to window
#include "Timer.h"

#include <iostream>
#include <thread>


namespace IS {

    //Basic constructor and setting base FPS to 60 
    InsightEngine::InsightEngine() : is_running(false), last_runtime(0), targetFPS(60) {
        IS_PROFILE_FUNCTION();
        IS_CORE_INFO("Starting Insight Engine...");
        //create the pointers to the managers
        mComponentManager = std::make_unique<ComponentManager>();
        mEntityManager = std::make_unique<EntityManager>();
        mSystemManager = std::make_unique<SystemManager>();
        IS_CORE_INFO("Insight Engine started");
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
        IS_PROFILE_FUNCTION();
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

        auto window = GetSystem<WindowSystem>("Window");
        window->BeginUpdate(); // Poll events

        // Update System deltas every 4s
        const int update_frequency = 4 * targetFPS;
        if (!(frame_count % update_frequency))
            mSystemDeltas["Engine"] = 0;
        
        // Update all systems
        for (const auto& system: mSystemList) {
            if (!mUsingGUI && system->GetName() == "GUI")
                continue;

            Timer timer(system->GetName() + " System", false);
            system->Update(delta_time.count());
            timer.Stop();

            if (!(frame_count % update_frequency)) {
                mSystemDeltas[system->GetName()] = timer.GetDeltaTime();
                mSystemDeltas["Engine"] += timer.GetDeltaTime();
            }
        }

        window->EndUpdate(); // Swap front and back buffers

        //by passing in the start time, we can limit the fps here by sleeping until the next loop and get the time after the loop
        auto frameEnd = LimitFPS(frameStart);

        delta_time = frameEnd - frameStart;

        ++frame_count;
    }

    //moved all the engine stuff under this run function
    void InsightEngine::Run() {
        IS_PROFILE_FUNCTION();
        Initialize();
        //this is the game loop
        while (is_running) {
            Update();
            ProcessEntityDeletion();
        }
    }

    void InsightEngine::Exit() {
        BROADCAST_MESSAGE(MessageType::Quit);
    }

    //This function will add a system to the map with the key being whatever the system defined it to be
    void InsightEngine::AddSystem(std::shared_ptr<ParentSystem> system ,Signature signature) {
        IS_PROFILE_FUNCTION();
        std::string systemName = system->GetName();
        IS_CORE_TRACE("Registering system... {}", systemName);
        mAllSystems[systemName] = system;
        mSystemList.emplace_back(system);
        mSystemManager->RegisterSystem(system);
        mSystemManager->SetSignature(systemName,signature);
    }

    //This function is meant to specifically find the individual system in the map and destroy it
    void InsightEngine::DestroySystem(const std::string& name) {
        IS_PROFILE_FUNCTION();
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
        IS_PROFILE_FUNCTION();
        mSystemList.clear();
        mAllSystems.clear();  // Clear the map
        IS_CORE_WARN("All systems terminated!");
    }

#pragma warning(push)
#pragma warning(disable: 4456) // concatnating __LINE__ for variable name in macro
    //loop through all the systems stored
    void InsightEngine::InitializeAllSystems() {
        IS_PROFILE_FUNCTION();
        for (auto const& system : mSystemList) {
            IS_PROFILE_SCOPE(system->GetName());
            system->Initialize();
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
    //accessor for system list
    std::vector<std::shared_ptr<ParentSystem>> const& InsightEngine::GetSystemList() const {
        return mSystemList;
    }

    // Get frame count
    unsigned InsightEngine::FrameCount() const { return frame_count; }

    void InsightEngine::DeleteEntity(Entity entity) { mEntitiesToDelete.insert(entity); }

    void InsightEngine::ProcessEntityDeletion() {
        for (Entity entity : mEntitiesToDelete)
            DestroyEntity(entity);
        mEntitiesToDelete.clear();
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
    Entity InsightEngine::CreateEntity(std::string name) {
        return mEntityManager->CreateEntity(name);
    }

    void InsightEngine::DestroyEntity(Entity entity) {
        mComponentManager->EntityDestroyed(entity);
        mEntityManager->DestroyEntity(entity);
        mSystemManager->EntityDestroyed(entity);
        IS_CORE_WARN("Entity {} completely destroyed!", entity);
    }

    void InsightEngine::GenerateRandomEntity() {
        PRNG prng;
        InsightEngine& engine = Instance();
        Entity e = engine.CreateEntityWithComponents<Sprite, Transform>("Random Entity");
        auto& trans = engine.GetComponent<Transform>(e);
        // scale [2, 16], pos [viewport], orientation [-360, 360]
        trans.setScaling((prng.generate() * 18.f) + 2.f, (prng.generate() * 18.f) + 2.f);
        trans.setWorldPosition((prng.generate()* WIDTH) - WIDTH / 2.f, (prng.generate()* HEIGHT) - HEIGHT / 2.f);
        trans.setRotation((prng.generate() * 720.f) - 360.f);
    }

    // Dynamic entity copying 
    Entity InsightEngine::CopyEntity(Entity old_entity) {
        Entity newEntity = CreateEntity(mEntityManager->FindNames(old_entity));
        CopyComponents(newEntity, old_entity);
        return newEntity;
    }


    void InsightEngine::SaveEntityToJson(Entity entity, std::string filename) {
        std::string file_path = "Assets/Entity/" + filename + ".json";
        std::string signature = mEntityManager->GetSignature(entity).to_string();
        Json::Value saved_entity;
        saved_entity["Signature"] = signature;
        saved_entity["Name"] = mEntityManager->FindNames(entity);
        //add in future components
        SerializeComponent<RigidBody>(entity, saved_entity, "RigidBody");
        SerializeComponent<Sprite>(entity, saved_entity, "Sprite");
        SerializeComponent<Transform>(entity, saved_entity, "Transform");

        SaveJsonToFile(saved_entity,file_path);
    }

    Entity InsightEngine::LoadEntityFromJson(std::string name) {
        std::string filename = "Assets/Entity/" + name + ".json";
        Json::Value loaded;
        LoadJsonFromFile(loaded, filename);
        std::string str_sig = loaded["Signature"].asString();
        std::string entity_name = loaded["Name"].asString();
        Entity entity = CreateEntity(entity_name);
        //add in future components
        DeserializeComponent<RigidBody>(entity, loaded, "RigidBody");
        DeserializeComponent<Sprite>(entity, loaded, "Sprite");
        DeserializeComponent<Transform>(entity, loaded, "Transform");
        return entity;

    }

    void InsightEngine::SaveAsPrefab(Entity entity,std::string PrefabName) {
        std::string file_path = "Assets/Prefabs/" + PrefabName + ".json";
        std::string signature = mEntityManager->GetSignature(entity).to_string();
        Json::Value prefab;
        prefab["Signature"] = signature;
        prefab["Name"] = PrefabName;
        SaveJsonToFile(prefab, file_path);

        IS_CORE_INFO("Prefab \"{}\" saved at \"{}\"", PrefabName, file_path);
    }

    //creating an entity from prefab
    Entity InsightEngine::LoadFromPrefab(Prefab prefab) {

        Entity entity = CreateEntity(prefab.mName);
        PrefabSignatureToEntity(prefab.mSignature, entity);
        return entity;

    }

    //Helper function for signature and string
    Signature StringToSignature(const std::string& str) {
        Signature signature;
        size_t strLength = str.size();
        for (size_t i = 0; i < strLength && i < MAX_COMPONENTS; ++i) {
            if (str[strLength - 1 - i] == '1') {
                signature.set(i, true);
            }
        }
        return signature;
    }


    Prefab InsightEngine::LoadPrefabFromFile(std::string filename) {
        std::string file_path = filename;
        Json::Value loaded;
        LoadJsonFromFile(loaded, file_path);
        Prefab prefab(StringToSignature(loaded["Signature"].asString()), loaded["Name"].asString());
        return prefab;
    }




}


