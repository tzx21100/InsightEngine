/*!
 * \file CoreEngine.cpp
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This source file defines the implementation of the core engine (Insight Engine)
 * The engine has a few key features such as accessing the ECS, saving and loading
 * scenes, prefabs and entities. Control key features such as FPS and is the general
 * application responsible for the architecture.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "CoreEngine.h"   // Include the header file
#include "JsonSaveLoad.h" // This is for Saving and Loading
#include "WindowSystem.h" // Access to window

#include <iostream>
#include <thread>

namespace IS {

    constexpr int DEFAULT_FPS(60);

    //Basic constructor and setting base FPS to 60 if its not set by LimitFPS
    InsightEngine::InsightEngine() : mIsRunning(false), mLastRuntime(0) {
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
            mIsRunning = false;
        }
    }

    // Destructor will delete all systems and clear it.
    InsightEngine::~InsightEngine() {
        IS_PROFILE_FUNCTION();
        mSystemList.clear();
        mAllSystems.clear();
        //IS_CORE_DEBUG("Insight Engine terminated");
    }

    void InsightEngine::Initialize() {
        //initialize all systems first
        InitializeAllSystems();
        auto window = GetSystem<WindowSystem>("Window");
        mTargetFPS = window->GetTargetFPS();
        //subscirbe to messages
        Subscribe(MessageType::Quit);
        //run the game
        mIsRunning = true;
    }

    //This is the update portion of the game
    void InsightEngine::Update() {
        //i get the start time 
        auto frameStart = std::chrono::high_resolution_clock::now();

        // Update System deltas every 1s
        static const float UPDATE_FREQUENCY = 1.f;
        static float elapsed_time = 0.f;
        elapsed_time += mDeltaTime.count();
        const bool to_update = mFrameCount == 0 || elapsed_time >= UPDATE_FREQUENCY;
        if (to_update)
            mSystemDeltas["Engine"] = elapsed_time = 0.f;
        
        // Update all systems
        for (const auto& system : mSystemList) {
            if (!mUsingGUI && system->GetName() == "Editor")
                continue;

            Timer timer(system->GetName() + " System", false);
            system->Update(mDeltaTime.count());
            timer.Stop();

            if (to_update) {
                mSystemDeltas[system->GetName()] = timer.GetDeltaTime();
                mSystemDeltas["Engine"] += timer.GetDeltaTime();
            }
        }

        //by passing in the start time, we can limit the fps here by sleeping until the next loop and get the time after the loop
        auto frameEnd = LimitFPS(frameStart);

        mDeltaTime = frameEnd - frameStart;

        ++mFrameCount;
    }

    // Moved all the engine stuff under this run function so we can have draw() load() etc. next time with a GSM
    void InsightEngine::Run() {
        IS_PROFILE_FUNCTION();
        Initialize();
        //this is the game loop
        while (mIsRunning) {
            Update();
            auto const& window = InsightEngine::Instance().GetSystem<WindowSystem>("Window");
            window->SwapBuffers(); // swap buffers after all the rendering
            ProcessEntityDeletion(); // destroy deleted entities
        }
        DestroyAllSystems();
    }

    //We quit
    void InsightEngine::Exit() {
        BROADCAST_MESSAGE(MessageType::Quit);
    }

    // This function will add a system to the map with the key being whatever the system defined it to be
    void InsightEngine::AddSystem(std::shared_ptr<ParentSystem> system ,Signature signature) {
        IS_PROFILE_FUNCTION();
        std::string systemName = system->GetName();
        IS_CORE_TRACE("Registering system... {}", systemName);
        // adding all of them to the different system maps and system manager
        mAllSystems[systemName] = system;
        mSystemList.emplace_back(system);
        mSystemManager->RegisterSystem(system);
        mSystemManager->SetSignature(systemName,signature);
    }

    // This function is meant to specifically find the individual system in the map and destroy it
    void InsightEngine::DestroySystem(const std::string& name) {
        IS_PROFILE_FUNCTION();
        auto it = mAllSystems.find(name);
        if (it != mAllSystems.end()) {
            mAllSystems.erase(it);
        }
        // This is to delete it from the vector. The vector is ordered by its load sequence.
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
        mSystemList.clear();  // Clear the vector
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

    int InsightEngine::GetTargetFPS() const {
        return *mTargetFPS;
    }

    // Get frame count
    unsigned InsightEngine::FrameCount() const { return mFrameCount; }

    // Delete the entity queue
    void InsightEngine::DeleteEntity(Entity entity) { mEntitiesToDelete.insert(entity); }

    // Process the entity deletion from queue
    void InsightEngine::ProcessEntityDeletion() {
        for (Entity entity : mEntitiesToDelete)
            DestroyEntity(entity);
        mEntitiesToDelete.clear();
    }

    // limit fps will return the frameEnd time now so i can use to find delta time
    std::chrono::high_resolution_clock::time_point InsightEngine::LimitFPS(const std::chrono::high_resolution_clock::time_point& frameStart) {
        const std::chrono::nanoseconds targetFrameTime(1'000'000'000 / *mTargetFPS);  // Nanosecond-level precision because accurate
        while (true) { // We chill in this loop until true
            auto now = std::chrono::high_resolution_clock::now();
            auto frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameStart);
            if (frameDuration >= targetFrameTime) {
                return now;  // Return the updated frame end time
            }
        }
    }

    // This is a simple function to set the FPS
    void InsightEngine::SetFPS(int FPS) { *mTargetFPS = FPS; }

    // These functions involve creating entities and destroying them
    Entity InsightEngine::CreateEntity(std::string name) {
        return mEntityManager->CreateEntity(name);
    }

    // When destroying the entity, every ECS must know of it
    void InsightEngine::DestroyEntity(Entity entity) {
        mComponentManager->EntityDestroyed(entity);
        mEntityManager->DestroyEntity(entity);
        mSystemManager->EntityDestroyed(entity);
        IS_CORE_WARN("Entity {} completely destroyed!", entity);
    }

    // Creates a random entity for demo testing
    void InsightEngine::GenerateRandomEntity(bool with_texture) {
        PRNG& prng = PRNG::Instance();
        InsightEngine& engine = Instance();
        auto const& window = engine.GetSystem<WindowSystem>("Window");
        auto [width, height] = window->GetWindowSize();

        Entity e = engine.CreateEntityWithComponents<Sprite, Transform>("Random Entity");
        auto& trans = engine.GetComponent<Transform>(e);

        // Constants
        constexpr float MAX_SCALE = 16.f;
        constexpr float MIN_SCALE = 2.f;
        constexpr float MAX_ROTATION = 720.f;
        constexpr float MIN_ROTATION = -360.f;

        // scale [2, 16], pos [viewport], orientation [-360, 360]
        trans.setScaling((prng.generate() * (MAX_SCALE - MIN_SCALE)) + MIN_SCALE, (prng.generate() * (MAX_SCALE - MIN_SCALE)) + MIN_SCALE);
        trans.setWorldPosition((prng.generate() * width) - width * .5f, (prng.generate()* height) - height * .5f);
        trans.setRotation((prng.generate() * (MAX_ROTATION - MIN_ROTATION)) + MIN_ROTATION);

        // with texture
        if (with_texture) {
            auto& sprite = engine.GetComponent<Sprite>(e);
            auto asset = engine.GetSystem<AssetManager>("Asset");
            float gacha = prng.generate(); // [0, 1]
            constexpr float ULTRA_RARE = 0.3f;
            constexpr float SUPER_RARE = 0.6f;
            constexpr float RARE = 0.8f;

            Image random_img =
                (gacha <= ULTRA_RARE) ? asset->GetImage("Assets/icecream_truck_frame.png") :
                (gacha <= SUPER_RARE) ? asset->GetImage("Assets/player_frame.png") :
                (gacha <= RARE) ? asset->GetImage("Assets/wii.png") : asset->GetImage("Assets/placeholder_background.png");
            sprite.img.texture_id = static_cast<uint8_t>(random_img.mTextureData);
            sprite.texture_width = random_img.width;
            sprite.texture_height = random_img.height;
        }
    }

    // Dynamic entity copying, this will copy all components and their specific component array.
    Entity InsightEngine::CopyEntity(Entity old_entity) {
        Entity newEntity = CreateEntity(mEntityManager->FindNames(old_entity));
        CopyComponents(newEntity, old_entity);
        return newEntity;
    }

    void InsightEngine::DeserializeAllComponents(Entity entity, Json::Value &loaded) {
        DeserializeComponent<RigidBody>(entity, loaded, "RigidBody");
        DeserializeComponent<Sprite>(entity, loaded, "Sprite");
        DeserializeComponent<Transform>(entity, loaded, "Transform");
        DeserializeComponent<ScriptComponent>(entity, loaded, "Script");
    }

    void InsightEngine::SerializeAllComponents(Entity entity, Json::Value &saved_entity) {
        SerializeComponent<RigidBody>(entity, saved_entity, "RigidBody");
        SerializeComponent<Sprite>(entity, saved_entity, "Sprite");
        SerializeComponent<Transform>(entity, saved_entity, "Transform");
        SerializeComponent<ScriptComponent>(entity, saved_entity, "Script");
    }

    /* This will save the entity to a string file.The string file is not defined with a path so it can save it
    *  directly unlike the other saving functions that have their specific use case.
    */
    void InsightEngine::SaveEntityToJson(Entity entity, std::string filename) {
        std::string file_path = filename;
        std::string signature = mEntityManager->GetSignature(entity).to_string();
        Json::Value saved_entity;
        saved_entity["Signature"] = signature;
        saved_entity["Name"] = mEntityManager->FindNames(entity);
        //add in future components
        SerializeAllComponents(entity, saved_entity);

        SaveJsonToFile(saved_entity,file_path);
    }

    /* This directly Loads an entity from an absolute filepath
    *  This is so that it can be used easily by other functions since its not for  a specific use case
    */
    Entity InsightEngine::LoadEntityFromJson(std::string name) {
        std::string filename = name;
        Json::Value loaded;
        LoadJsonFromFile(loaded, filename);
        std::string str_sig = loaded["Signature"].asString();
        std::string entity_name = loaded["Name"].asString();
        Entity entity = CreateEntity(entity_name);
        //add in future components: the DeserializeComponent is defined in the .h as a helper function to make calling easier
        DeserializeAllComponents(entity, loaded);
        return entity;

    }


    // This saves an entity as a prefab directly into the prefab asset. 
    void InsightEngine::SaveAsPrefab(Entity entity,std::string PrefabName) {
        std::string file_path = "Assets/Prefabs/" + PrefabName + ".json";
        SaveEntityToJson(entity,file_path);
        IS_CORE_INFO("Prefab \"{}\" saved at \"{}\"", PrefabName, file_path);
    }

    //creating an entity from prefab
    Entity InsightEngine::LoadFromPrefab(Prefab prefab) {
        Entity entity = CreateEntity(prefab.mName);
        PrefabSignatureToEntity(prefab.mSignature, entity);
        return entity;

    }

    // Helper function for signature and string this is only to help load prefabs from files.
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

    // Loading the prefabs from files and storing it as a prefab.
    Prefab InsightEngine::LoadPrefabFromFile(std::string filename) {
        std::string file_path = filename;
        Json::Value loaded;
        LoadJsonFromFile(loaded, file_path);
        Prefab prefab(StringToSignature(loaded["Signature"].asString()), loaded["Name"].asString());
        return prefab;
    }

    /* This function is meant to save the entire scene.
    *  As our scene is made up of entities, we simply just have to save every entity in the scene.
    */
    void InsightEngine::SaveCurrentScene(std::string filename) {
        std::string file_path = "Assets/Scene/" + filename + "/"+ "mainscene.json";
        int EntitiesAlive = mEntityManager->EntitiesAlive();
        Json::Value scene;
        scene["EntityAmount"] = EntitiesAlive; // This is needed for loading to tell how many entities there are.
        Json::Value entities(Json::arrayValue);
        for (Entity id = 0 ; id < mEntityManager->EntitiesAlive(); id++) {
            Json::Value entity;
            entity["Name"] = mEntityManager->FindNames(id);
            SerializeAllComponents(id, entity);

            entities.append(entity);
            /*std::string entity_names = "Assets/Scene/" + filename + "/Entities/entity_" + std::to_string(id.first) + ".json";
            SaveEntityToJson(id.first, entity_names);*/
        }
        scene["Entities"] = entities;
        SaveJsonToFile(scene, file_path);
        IS_CORE_DEBUG("Saving scene successful: {} entities saved", EntitiesAlive);
    }

    // Using the same format defined above, we simply reverse it and load in our entities.
    void InsightEngine::LoadScene(std::string filename) {
        // Load the main scene file
        std::string mainSceneFile = "Assets/Scene/" + filename + "/mainscene.json";
        Json::Value sceneRoot;
        if (!LoadJsonFromFile(sceneRoot, mainSceneFile)) {
            std::cerr << "Failed to load main scene file: " << mainSceneFile << std::endl;
            return;
        }

        // Start a new scene
        NewScene();
        int EntitiesAlive = sceneRoot["EntityAmount"].asInt();
        const Json::Value entities = sceneRoot["Entities"];
        

        // Load each entity
        for (int i = 0; i < EntitiesAlive; ++i) {
            Json::Value entityData = entities[i]; // Get data for each individual entity from the "Entities" array

            std::string entity_name = entityData["Name"].asString();
            Entity entity = CreateEntity(entity_name);

            // Deserialize components for the entity
            DeserializeAllComponents(entity, entityData);
        }

        IS_CORE_DEBUG("Loading scene successful: {} entities loaded", EntitiesAlive);
    }

    void InsightEngine::NewScene() {
        // Destroy all existing entities
        mComponentManager->ClearEntities();
        mSystemManager->ClearEntities();
        //reset the entity ID to start from 0
        mEntityManager->ResetEntityID();
    }

    // Abstracted functions for window getters/setters
    // Getters
    int InsightEngine::GetMonitorWidth()                { return GetSystem<WindowSystem>("Window")->GetMonitorWidth(); }
    int InsightEngine::GetMonitorHeight()               { return GetSystem<WindowSystem>("Window")->GetMonitorHeight(); }
    std::pair<int, int> InsightEngine::GetMonitorSize() { return GetSystem<WindowSystem>("Window")->GetMonitorSize(); }
    int InsightEngine::GetWindowWidth()                 { return GetSystem<WindowSystem>("Window")->GetWidth(); }
    int InsightEngine::GetWindowHeight()                { return GetSystem<WindowSystem>("Window")->GetHeight(); }
    std::pair<int, int> InsightEngine::GetWindowSize()  { return GetSystem<WindowSystem>("Window")->GetWindowSize(); }
    bool InsightEngine::IsVSync()                       { return GetSystem<WindowSystem>("Window")->IsVSync(); }
    bool InsightEngine::IsFullScreen()                  { return GetSystem<WindowSystem>("Window")->IsFullScreen(); }
    // Setters
    void InsightEngine::EnableVSync(bool enabled)       { GetSystem<WindowSystem>("Window")->EnableVsync(enabled); }

    //abstracted function to make game script
    void InsightEngine::CreateGameScript(const std::string& ScriptName) {
        auto script = GetSystem<ScriptManager>("ScriptManager");
        script->CreateClassFile(ScriptName, "../IS-ScriptCore/Source/" + ScriptName + ".cs");
    }

    void InsightEngine::OpenGameScript(const std::string& ScriptName) {
        auto script = GetSystem<ScriptManager>("ScriptManager");
        script->OpenClassFile(ScriptName);
    }


}


