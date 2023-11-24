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
#include "EditorLayer.h"
#include "Body.h"
#include "Collider.h"
#include "Light.h"

#include <iostream>
#include <thread>

namespace IS {

    constexpr int DEFAULT_FPS(60);

    int InsightEngine::currentNumberOfSteps = 0;

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

    #ifdef USING_IMGUI
        PushImGuiLayers();
    #endif

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
        //auto frameStart = std::chrono::high_resolution_clock::now();
        auto frameStart = glfwGetTime();

        // Update System deltas every 1s
        static const float UPDATE_FREQUENCY = 1.f;
        static float elapsed_time = 0.f;
        elapsed_time += static_cast<float>(mDeltaTime);
        const bool to_update = mFrameCount == 0 || elapsed_time >= UPDATE_FREQUENCY;

        if (to_update)
            mSystemDeltas["Engine"] = elapsed_time = 0.f;

        

        accumulatedTime += mDeltaTime; //adding actual game loop time

        while (accumulatedTime >= 1.f/60.f) {
            accumulatedTime -= 1.f / 60.f; //this will store the
            //exact accumulated time differences, among all game loops
            currentNumberOfSteps++;
        }
        currentNumberOfSteps = 1;
        if (currentNumberOfSteps > 0) {

            // Update all systems
            for (const auto& system : mSystemList)
            {

                Timer timer(system->GetName() + " System", false);
                system->Update((float)mDeltaTime);
                timer.Stop();

                if (to_update && currentNumberOfSteps==1) {
                    mSystemDeltas[system->GetName()] = timer.GetDeltaTime();
                    mSystemDeltas["Engine"] += timer.GetDeltaTime();
                }
            }

            currentNumberOfSteps = 0;
        }


    #ifdef USING_IMGUI
        // Update and render GUI
        if (mRenderGUI)
        {
            mImGuiLayer->Begin();
            mLayers.Update(1.f / 60.f);
            mLayers.Render();
            mImGuiLayer->End();
        }
    #endif // USING_IMGUI

        //by passing in the start time, we can limit the fps here by sleeping until the next loop and get the time after the loop
        mDeltaTime = LimitFPS(frameStart)-frameStart;

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
            SceneManager::Instance().UpdateActiveScene(); // update active scene
        }
        auto script = GetSystem<ScriptManager>("ScriptManager");
        script->CleanUp();
        DestroyAllSystems();
        mLayers.ClearStack();
    }

    //We quit
    void InsightEngine::Exit() {
        BROADCAST_MESSAGE(MessageType::Quit);
    }

    // This function will add a system to the map with the key being whatever the system defined it to be
    void InsightEngine::AddSystem(std::shared_ptr<ParentSystem> system ,Signature signature) {
        IS_PROFILE_FUNCTION();
        std::string systemName = system->GetName();
        IS_CORE_DEBUG("Registering system... {}", systemName);
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
        {
            DestroyEntity(entity);
        }
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

    double InsightEngine::LimitFPS(double frame_start)
    {
        while (true)
        {
            double now = glfwGetTime();
            double dt = (now - frame_start);
            double target = (1.0 / *mTargetFPS);
            if (dt >= target)
            {
                return now;
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
    Entity InsightEngine::GenerateRandomEntity(bool with_texture) {
        PRNG& prng = PRNG::Instance();
        InsightEngine& engine = Instance();
        Camera3D& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
        Vector2D window_size = { static_cast<float>(GetWindowWidth()), static_cast<float>(GetWindowHeight())};
        window_size /= camera.GetZoomLevel();

        Entity e = engine.CreateEntityWithComponents<Sprite, Transform>("Random Entity");
        auto& trans = engine.GetComponent<Transform>(e);

        // Constants
        constexpr float MAX_ROTATION = 720.f;
        constexpr float MIN_ROTATION = -360.f;
        float MAX_SCALE = 16.f;
        float MIN_SCALE = 2.f;

        MAX_SCALE /= camera.GetZoomLevel();
        MIN_SCALE /= camera.GetZoomLevel();

        // scale [2, 16], pos [viewport], orientation [-360, 360]
        trans.setScaling((prng.generate() * (MAX_SCALE - MIN_SCALE)) + MIN_SCALE, (prng.generate() * (MAX_SCALE - MIN_SCALE)) + MIN_SCALE);
        trans.setWorldPosition((prng.generate() * window_size.x) - window_size.x * .5f, (prng.generate()* window_size.y) - window_size.y * .5f);
        trans.setRotation((prng.generate() * (MAX_ROTATION - MIN_ROTATION)) + MIN_ROTATION);

        auto& sprite = engine.GetComponent<Sprite>(e);
        sprite.color = { prng.generate(), prng.generate(), prng.generate(), 1.f };
        // with texture
        if (with_texture) {
            auto asset = engine.GetSystem<AssetManager>("Asset");
            float gacha = prng.generate(); // [0, 1]
            constexpr float ULTRA_RARE = .25f;
            constexpr float SUPER_RARE = .5f;
            constexpr float RARE = .75f;

            Image random_img =
                (gacha <= ULTRA_RARE) ? *asset->GetImage("Assets/Textures/icecream_truck_frame.png") :
                (gacha <= SUPER_RARE) ? *asset->GetImage("Assets/Textures/player_frame.png") :
                (gacha <= RARE) ? *asset->GetImage("Assets/Textures/wii.png") : *asset->GetImage("Assets/Textures/placeholder_background.png");
            sprite.img = random_img;
        }

        return e;
    }

    // Dynamic entity copying, this will copy all components and their specific component array.
    Entity InsightEngine::CopyEntity(Entity old_entity) {
        Entity new_entity = CreateEntity(mEntityManager->FindNames(old_entity));
        CopyComponents(new_entity, old_entity);
        if (HasComponent<Transform>(new_entity))
        {
            Transform& transform = GetComponent<Transform>(new_entity);
            transform.world_position += Vector2D(20.f, -20.f);
        }

        return new_entity;
    }

    void InsightEngine::DeserializeAllComponents(Entity entity, Json::Value &loaded) {
        DeserializeComponent<RigidBody>(entity, loaded, "RigidBody");
        DeserializeComponent<Sprite>(entity, loaded, "Sprite");
        DeserializeComponent<Transform>(entity, loaded, "Transform");
        DeserializeComponent<ScriptComponent>(entity, loaded, "Script");
        DeserializeComponent<ButtonComponent>(entity, loaded, "ButtonComponent");
        DeserializeComponent<Collider>(entity, loaded, "Collider");
        DeserializeComponent<AudioListener>(entity, loaded, "AudioListener");
        DeserializeComponent<AudioEmitter>(entity, loaded, "AudioEmitter");
        DeserializeComponent<Light>(entity, loaded, "Light");
    }    
    
    void InsightEngine::DeserializeAllComponentsPrefab(Entity entity, Json::Value &loaded) {
        DeserializeComponent<RigidBody>(entity, loaded, "RigidBody");
        DeserializeComponent<Sprite>(entity, loaded, "Sprite");
        DeserializeComponent<Transform>(entity, loaded, "Transform");
        DeserializeComponent<ScriptComponent>(entity, loaded, "Script");
        DeserializeComponent<ButtonComponent>(entity, loaded, "ButtonComponent");
        DeserializeComponent<Collider>(entity, loaded, "Collider");
        DeserializeComponent<AudioListener>(entity, loaded, "AudioListener");
        DeserializeComponent<AudioEmitter>(entity, loaded, "AudioEmitter");
        DeserializeComponent<Light>(entity, loaded, "Light");
    }

    void InsightEngine::SerializeAllComponents(Entity entity, Json::Value &saved_entity) {
        SerializeComponent<RigidBody>(entity, saved_entity, "RigidBody");
        SerializeComponent<Sprite>(entity, saved_entity, "Sprite");
        SerializeComponent<Transform>(entity, saved_entity, "Transform");
        SerializeComponent<ScriptComponent>(entity, saved_entity, "Script");
        SerializeComponent<ButtonComponent>(entity, saved_entity, "ButtonComponent");
        SerializeComponent<Collider>(entity, saved_entity, "Collider");
        SerializeComponent<AudioListener>(entity, saved_entity, "AudioListener");
        SerializeComponent<AudioEmitter>(entity, saved_entity, "AudioEmitter");
        SerializeComponent<Light>(entity, saved_entity, "Light");
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
    Entity InsightEngine::LoadEntityFromJson(std::string name, Entity entity) {
        std::string filename = name;
        Json::Value loaded;
        LoadJsonFromFile(loaded, filename);
        std::string str_sig = loaded["Signature"].asString();
        std::string entity_name = loaded["Name"].asString();
        mComponentManager->EntityDestroyed(entity);
        mSystemManager->EntityDestroyed(entity);
        mEntityManager->SetName(entity, entity_name);
        //add in future components: the DeserializeComponent is defined in the .h as a helper function to make calling easier
        DeserializeAllComponents(entity, loaded);
        return entity;

    }    
    
    Entity InsightEngine::LoadEntityFromJsonPrefab(std::string name, Entity entity) {
        std::string filename = name;
        Json::Value loaded;
        LoadJsonFromFile(loaded, filename);
        std::string str_sig = loaded["Signature"].asString();
        std::string entity_name = loaded["Name"].asString();
        if (HasComponent<Transform>(entity)) {
            Transform save = GetComponent<Transform>(entity);
            Vector2D save_pos=save.getWorldPosition();
            mComponentManager->EntityDestroyed(entity);
            mSystemManager->EntityDestroyed(entity);
            mEntityManager->SetName(entity, entity_name);
            DeserializeAllComponentsPrefab(entity, loaded);
            auto &a = GetComponent<Transform>(entity);
            a.setWorldPosition(save_pos.x, save_pos.y);

        }
        else {
            mComponentManager->EntityDestroyed(entity);
            mSystemManager->EntityDestroyed(entity);
            mEntityManager->SetName(entity, entity_name);
            //add in future components: the DeserializeComponent is defined in the .h as a hel  er function to make calling easier
            DeserializeAllComponents(entity, loaded);

        }

        return entity;

    }


    // This saves an entity as a prefab directly into the prefab asset. 
    void InsightEngine::SaveAsPrefab(Entity entity,std::string PrefabName) {
        std::string file_path = "Assets/Prefabs/" + PrefabName + ".json";
        SaveEntityToJson(entity,file_path);
        auto system=GetSystem<AssetManager>("Asset");
        system->LoadPrefab(file_path);
        IS_CORE_INFO("Prefab \"{}\" saved at \"{}\"", PrefabName, file_path);
    }

    //creating an entity from prefab
    Entity InsightEngine::LoadFromPrefab(Prefab prefab ,Entity entity) {
        LoadEntityFromJsonPrefab("Assets/Prefabs/"+ prefab.mName + ".json", entity);
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
        std::string file_path = "Assets\\Scenes\\" + filename + ".insight";
        int EntitiesAlive = mEntityManager->EntitiesAlive();
        Json::Value scene;
        scene["EntityAmount"] = EntitiesAlive; // This is needed for loading to tell how many entities there are.
        Json::Value entities(Json::arrayValue);
        auto& aliveEntities = mEntityManager->GetEntitiesAlive();
        for (const auto& [id, name] : aliveEntities) {
            Json::Value entity;
            entity["Name"] = mEntityManager->FindNames(id);
            SerializeAllComponents(id, entity);

            entities.append(entity);
            /*std::string entity_names = "Assets/Scenes/" + filename + "/Entities/entity_" + std::to_string(id.first) + ".json";
            SaveEntityToJson(id.first, entity_names);*/
        }
        scene["Entities"] = entities;
        if (SaveJsonToFile(scene, file_path))
        {
            IS_CORE_INFO("Saving scene {} successful!", file_path);
            IS_CORE_INFO("{} entities saved.", EntitiesAlive);
        }
    }

    // Using the same format defined above, we simply reverse it and load in our entities.
    bool InsightEngine::LoadScene(std::string filename) {
        // Load the main scene file
        std::filesystem::path filepath(filename);
        if (filepath.extension() != ".insight")
        {
            IS_CORE_ERROR("Invalid scene file, use \".insight\" file");
            return false;
        }
        Json::Value sceneRoot;
        if (!LoadJsonFromFile(sceneRoot, filepath.string())) {
            IS_CORE_ERROR("Failed to load scene file: {}", filepath.string());
            return false;
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

        IS_CORE_INFO("Loading scene {} successful!", filepath.string());
        IS_CORE_INFO("{} entities loaded", EntitiesAlive);
        return true;
    }

    void InsightEngine::NewScene() {
        auto scriptsystem = GetSystem<ScriptManager>("ScriptManager");
        scriptsystem->CleanUp();
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
    bool InsightEngine::IsMinimized()                     { return GetSystem<WindowSystem>("Window")->IsMinimized(); }
    // Setters
    void InsightEngine::EnableVSync(bool enabled)       { GetSystem<WindowSystem>("Window")->EnableVsync(enabled); }

    //abstracted function to make game script
    void InsightEngine::CreateGameScript(const std::string& ScriptName) {
        auto script = GetSystem<ScriptManager>("ScriptManager");
        script->CreateClassFile(ScriptName, "../Insight Engine/Assets/Scripts/" + ScriptName + ".cs");
    }

    void InsightEngine::OpenGameScript(const std::string& ScriptName) {
        auto script = GetSystem<ScriptManager>("ScriptManager");
        script->OpenClassFile(ScriptName);
    }

    void InsightEngine::PushLayer(layer_type layer)
    {
        mLayers.PushLayer(layer);
        layer->OnAttach();
    }

    void InsightEngine::PushOverlay(layer_type overlay)
    {
        mLayers.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void InsightEngine::PopLayer(layer_type layer)
    {
        mLayers.PopLayer(layer);
        layer->OnDetach();
    }

    void InsightEngine::PopOverlay(layer_type overlay)
    {
        mLayers.PopOverlay(overlay);
        overlay->OnDetach();
    }

    void InsightEngine::PushImGuiLayers()
    {
    #ifdef USING_IMGUI
        mImGuiLayer = std::make_shared<ImGuiLayer>();
        mEditorLayer = std::make_shared<EditorLayer>();
        PushOverlay(mImGuiLayer);
        PushLayer(mEditorLayer);
        mUsingGUI = true;
        mRenderGUI = true;

    #endif // USING_IMGUI

    }


}


