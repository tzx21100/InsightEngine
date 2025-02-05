/*!
 * \file CoreEngine.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2451
 * \date 26-09-2023
 * \brief
 * This header file declares the implementation of the core engine (Insight Engine)
 * It defines the template functions accessed as a coordinator for the ECS and relates
 * to the saving and loading of entites and scene.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_CORE_COREENGINE_H_
#define GAM200_INSIGHT_ENGINE_CORE_COREENGINE_H_
#define ASSET_MANAGER InsightEngine::Instance().GetSystem<AssetManager>("Asset")
#define INPUT_MANAGER InsightEngine::Instance().GetSystem<InputManager>("Input")
#define AUDIO_MANAGER InsightEngine::Instance().GetSystem<ISAudio>("Audio")

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Engine/ECS/Entities.h"
#include "Engine/ECS/System.h"
#include "Engine/JSON/Prefab.h"
#include "Engine/Scripting/ScriptManager.h"
#include "../Systems/Audio/Audio.h"
#include "Editor/Layers/LayerStack.h"
#include "Editor/Layers/ImGuiLayer.h"
#include "Editor/Layers/EditorLayer.h"
#include "Math/Vector2D.h"
#include "../Messages/Message.h"


#include <unordered_map>
#include <chrono>
#include <unordered_set>
#include <iostream>

namespace IS {

    // forward delcarations
    class Sprite;
    class ISGraphics;

    /*!
       * \brief The central engine class for the game.
       *
       * This class is responsible for managing entities, systems, components,
       * and the main game loop. It follows the Singleton pattern, ensuring only
       * one instance of the engine exists throughout the game's lifecycle.
       */
    class InsightEngine : public MessageListener {
    public:
        using layer_type = LayerStack::value_type;

        /*!
         * \brief Flags to control the freeze and continuation of game frames.
         */
        bool mFreezeFrame = false;
        bool mContinueFrame = false;
    #ifdef USING_IMGUI
        bool mRuntime = false;
    #else
        bool mRuntime = true;
    #endif // USING_IMGUI

        bool mUsingGUI = false; ///< Flag indicating if using GUI.
        bool mRenderGUI = false; ///< Flag indicating if render GUI.

        /*!
         * \brief Handles incoming messages for the core engine.
         * \param message The message to be handled.
         */
        virtual void HandleMessage(const Message& message) override;

        /*!
         * \brief Retrieves the singleton instance of the InsightEngine.
         * \return The singleton instance of the engine.
         */
        static InsightEngine& Instance() {static InsightEngine instance;return instance;}

        /*!
         * \brief Adds a system to the engine.
         * \param system The system to add.
         * \param signature The signature of the system.
         */
        void AddSystem(std::shared_ptr<ParentSystem> system, Signature signature);

        /*!
         * \brief Removes a system from the engine by its name.
         * \param name The name of the system to remove.
         */
        void DestroySystem(const std::string& name);

        /*!
         * \brief Removes all systems from the engine.
         */
        void DestroyAllSystems();

        /*!
         * \brief Initializes all systems in the engine.
         */
        void InitializeAllSystems();

        /*!
         * \brief Accessor for system delta times.
         * \return A map of system names to their delta times.
         */
        std::unordered_map<std::string, float> const& GetSystemDeltas() const;

        /*!
         * \brief Accessor for system pointers.
         * \return A map of system names to their shared pointers.
         */
        std::unordered_map<std::string, std::shared_ptr<ParentSystem>> const& GetSystemPointer() const;

        /*!
         * \brief Accessor for the frame count of the engine.
         * \return The current frame count.
         */
        unsigned FrameCount() const;

        /*!
         * \brief Accessor for the list of systems.
         * \return A vector of shared pointers to the systems.
         */
        std::vector<std::shared_ptr<ParentSystem>> const& GetSystemList() const;

        /*!
         * \brief Gets the target frames per second for the engine.
         * \return The target FPS.
         */
        int GetTargetFPS() const;

        /*!
         * \brief Sets the desired frames per second for the engine.
         * \param num The desired FPS.
         */
        void SetFPS(int num);

        /*!
         * \brief Functions for the game loop. Calling Initialize followed by Update in Run funciton.
         */
        void Initialize();
        void Update();
        void Run();
        void Exit();

        /*!
         * \brief Sends a message to the event manager.
         * \param message Pointer to the message to be broadcasted.
         */
        void SendMessage(Message message) { EventManager::Instance().Broadcast(message); };

        /*!
         * \brief Subscribes to a specific type of message.
         * \param type The type of message to subscribe to.
         */
        void Subscribe(MessageType type) { EventManager::Instance().Subscribe(type, this); };

        // abstracted functions to get window properties

        /*!
         * \brief Gets the title of the window.
         * \return The title of the window.
         */
        void GetWindowSize(int& width, int& height);

        /*!
         * \brief Gets the width of the window.
         */
        int GetWindowWidth();

        /*!
         * \brief Gets the height of the window.
         */
        int GetWindowHeight();

        /*!
         * \brief Checks if vsync is enabled for the window.
         * \return True if vsync is enabled, false otherwise.
         */
        bool IsVSync();

        /*!
         * \brief Checks if window is fullscreen.
         * \return true if window is fullscreen, false otherwise.
         */
        bool IsFullScreen();

        /*!
         * \brief Enables or disables vsync for the window.
         * \param enabled Flag indicating whether to enable vsync. (default true)
         */
        void EnableVSync(bool enabled = true);

        /*!
         * \brief Checks if window is focused.
         * \return Boolean flag indicating if window is focused.
         */
        bool IsWindowFocused();

        /*!
         * \brief Checks if window is minimized.
         * \return Boolean flag indicating if window is minimized.
         */
        bool IsWindowMinimized();

        /*!
         * \brief Get the height of the title bar of the window.
         * \return The height of the title bar of the window.
         */
        int GetTitleBarHeight();

        /*!
         * \brief Create Game Script in a set format
         * \return 
         */
        void CreateGameScript(const std::string& ScriptName);

        /*!
         * \brief Opens a game script
         * \return
         */
        void OpenGameScript(const std::string& ScriptName);

        /*!
         * \brief Gets the Caller of the current script in scriptmanager
        * \return the Entity that is calling the script
        */
        Entity GetScriptCaller() {
            auto script = GetSystem<ScriptManager>("ScriptManager");
            return script->GetScriptEntity();
        }


        /**
         * \brief Creates an entity with a specific name.
         * \param name The name for the new entity.
         * \return The created entity.
         */
        Entity CreateEntity(std::string name);

        /**
         * \brief Generates a random entity.
         * \param with_texture A flag indicating if the entity should have a texture. Default is false.
         * \return The randomly generated entity.
         */
        Entity GenerateRandomEntity(bool with_texture = false);

        /**
         * \brief Deletes a specified entity.
         * \param entity The entity to delete.
         */
        void DeleteEntity(Entity entity);

        /**
           * \brief Saves a specified entity to a JSON file.
           * \param entity The entity to save.
           * \param filename The name of the JSON file to save to.
           */
        void SaveEntityToJson(Entity entity, std::string filename);

        /**
         * \brief Saves an entity as a prefab to the prefab file.
         * \param entity The entity to save as a prefab.
         * \param filename The name of the file to save the prefab to.
         */
        void SaveAsPrefab(Entity entity, std::string filename);

        /**
         * \brief Saves the current scene to a file. This will create a directory.
         * \param filename The name of the file to save the scene to.
         */
        void SaveCurrentScene(std::string filename);

        /**
         * \brief Loads a scene from a specified file directory in the Scene folder.
         * \param filename The name of the file to load the scene from.
         * \return boolean flag indicating success of load scene.
         */
        bool LoadScene(std::string filename);


        /**
         * \brief Generates a new scene
         */
        void NewScene();

        /**
         * \brief Loads an entity from a JSON file.
         * \param filename The name of the JSON file to load the entity from.
         * \return The loaded entity.
         */
        Entity LoadEntityFromJson(std::string filename, Entity entity);
        Entity LoadEntityFromJsonPrefab(std::string filename, Entity entity);

        /**
         * \brief Loads an entity from a prefab.
         * \param prefab The prefab to load the entity from.
         * \return The loaded entity.
         */
        Entity LoadFromPrefab(Prefab prefab, Entity entity);

        /**
         * \brief Loads a prefab from a specified file.
         * \param filename The name of the file to load the prefab from.
         * \return The loaded prefab.
         */
        Prefab LoadPrefabFromFile(std::string filename);

        /**
         * \brief Creates a copy of an existing entity.
         * \param old_entity The entity to be copied.
         * \return The newly created copied entity.
         */
        Entity CopyEntity(Entity old_entity);

        /**
         * \brief Retrieves the name associated with a given entity.
         * \param entity The entity whose name is to be retrieved.
         * \return The name of the specified entity.
         * \note This is a const version of the function, ensuring the entity remains unmodified.
         */
        std::string GetEntityName(Entity entity) const {
            return mEntityManager->FindNames(entity);
        }

         /**
         * \brief Retrieves the name associated with a given entity.
         * \param entity The entity whose name is to be retrieved.
         * \return The name of the specified entity.
         */
        std::string& GetEntityName(Entity entity) {
            return mEntityManager->FindNames(entity);
        }


        /**
         * \brief Determines the number of entities that are currently alive.
         * \return The count of currently alive entities.
         */
        uint32_t EntitiesAlive() {
            return mEntityManager->EntitiesAlive();
        }

        // for the latest entity ID
        uint32_t LatestEntity() {
            return mEntityManager->LatestEntityID();
        }


        /**
         * \brief Retrieves a mapping of all living entities and their associated names.
         * \return An unordered map of entities and their respective names.
         */
        std::unordered_map<Entity, std::string>& GetEntitiesAlive() {
            return mEntityManager->GetEntitiesAlive();
        }

        /**
         * \brief This will return true or false depending on if the entity has the component T
         * \return boolean
         */
        template <typename T>
        bool HasComponent(Entity entity) {
            ComponentType type = mComponentManager->GetComponentType<T>();
            return mEntityManager->HasComponent(entity, type);
        }

        /**
         * \brief This will copy all the components of an old_entity into the new one
         * \param entity to acquire the components of. old_entity, to copy the components from.
         */
        void CopyComponents(Entity entity,Entity old_entity) {
            Signature signature=mComponentManager->CloneComponent(entity,old_entity);
            mEntityManager->SetSignature(entity, signature);
            mSystemManager->EntitySignatureChanged(entity, signature);
        }

         /**
         * \brief This converts a prefab's signature into components to a entity passed to it
         * \param Signature. The prefab's signature. entity, the entity to recieve the components.
         */
        void PrefabSignatureToEntity(Signature prefab_signature,Entity entity) {
            Signature signature = mComponentManager->PrefabToEntity(prefab_signature, entity);
            mEntityManager->SetSignature(entity, signature);
            mSystemManager->EntitySignatureChanged(entity, signature);
        }

        /**
         * \brief Registers a new component type for the entity-component-system (ECS) framework.
         * \tparam T The component type to register.
         */
        template <typename T>
        void RegisterComponent() {
            mComponentManager->RegisterComponent<T>();
        }


        // Messages
        Message mSpriteAdded = Message(MessageType::SpriteAdded);
        Message mSpriteRemoved = Message(MessageType::SpriteRemoved);


        /**
         * \brief Adds a specified component to a given entity.
         * \tparam T The type of component to add.
         * \param entity The entity to which the component should be added.
         * \param component The component instance to add.
         */
        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            if (GetComponentType<T>() == GetComponentType<Sprite>()) {
                mSpriteAdded.int_value = entity;
                SendMessage(mSpriteAdded);
            }

            mComponentManager->AddComponent<T>(entity, component);

            auto signature = mEntityManager->GetSignature(entity);
            signature.set(mComponentManager->GetComponentType<T>(), true);
            mEntityManager->SetSignature(entity, signature);

            mSystemManager->EntitySignatureChanged(entity, signature);
        }

        /**
         * \brief Removes a specified component type from a given entity.
         * \tparam T The type of component to remove.
         * \param entity The entity from which the component should be removed.
         */
        template<typename T>
        void RemoveComponent(Entity entity)
        {

            if (GetComponentType<T>() == GetComponentType<Sprite>()) {
                mSpriteRemoved.int_value = entity;
                SendMessage(mSpriteRemoved);
            }

            if (GetComponentType<T>() == GetComponentType<Sprite>()) {}

            if (InsightEngine::Instance().HasComponent<T>(entity)) {
                mComponentManager->RemoveComponent<T>(entity);
            }

            auto signature = mEntityManager->GetSignature(entity);
            signature.set(mComponentManager->GetComponentType<T>(), false);
            mEntityManager->SetSignature(entity, signature);

            mSystemManager->EntitySignatureChanged(entity, signature);
        }

        /**
         * \brief Retrieves a specific component associated with a given entity.
         * \tparam T The type of component to retrieve.
         * \param entity The entity whose component is to be retrieved.
         * \return A reference to the specified component of the entity.
         */
        template<typename T>
        T& GetComponent(Entity entity)
        {
              return mComponentManager->GetComponent<T>(entity);
        }


        /**
         * \brief Retrieves the type identifier of a specified component.
         * \tparam T The type of component for which the identifier is to be retrieved.
         * \return The type identifier for the specified component.
         */
        template<typename T>
        ComponentType GetComponentType()
        {
            return mComponentManager->GetComponentType<T>();
        }

        /**
         * \brief Creates an entity with a specified name and attaches the given components to it.
         *
         * This function allows for the creation of an entity with multiple components in a single call.
         *
         * \tparam Components Variadic template parameters indicating the types of components to attach.
         * \param name The name of the entity to create.
         * \return The created entity.
         *
         * Example usage:
         * Entity player = CreateEntityWithComponents<Position, Velocity, Health>("Player");
         */
        template<typename... Components> // This ... will allow you to add as many Components as you want sort of like printf
        Entity CreateEntityWithComponents(std::string name) {
            Entity entity = CreateEntity(name);
            (AddComponent<Components>(entity, Components()), ...);
            return entity;
        }

        /**
         * \brief Updates the signature of an entity to reflect the addition of a new component.
         *
         * This function modifies the entity's signature to include the specified component type, indicating that the entity now possesses this component.
         *
         * \tparam T The type of component to add to the entity's signature.
         * \param entity The entity whose signature should be updated.
         */
        template<typename T>
        void UpdateSignature(Entity entity) {
            //gets the entity signature
            auto signature = mEntityManager->GetSignature(entity);
            //adds the component to the signature
            signature.set(mComponentManager->GetComponentType<T>(), true);
            mEntityManager->SetSignature(entity, signature);
            mSystemManager->EntitySignatureChanged(entity, signature);
        }

        /**
         * \brief Adds a component to an entity and then updates the entity's signature to reflect this addition.
         *
         * This is a convenience function that performs both the addition of a component and the updating of an entity's signature in a single call.
         *
         * \tparam T The type of component to add.
         * \param entity The entity to which the component should be added.
         * \param component The component instance to add.
         */
        template<typename T>
        void AddComponentAndUpdateSignature(Entity entity, T component) {
            AddComponent<T>(entity, component);
            UpdateSignature<T>(entity);
        }

        /**
         * \brief Generates a signature based on the provided component types.
         *
         * This function creates a signature that represents the presence of the specified component types.
         *
         * \tparam Components Variadic template parameters indicating the types of components for the signature.
         * \return A signature representing the specified component types.
         */
        template<typename... Components>
        Signature GenerateSignature() {
            Signature signature;
            // Use a fold expression to 'or' each component type into the signature
            (..., signature.set(mComponentManager->GetComponentType<Components>()));
            return signature;
        }


        /**
         * \brief Serializes a component attached to an entity into a JSON format.
         *
         * This function is useful for saving the state of a component for later use or for exporting data.
         *
         * \tparam T The type of component to serialize.
         * \param entity The entity whose component should be serialized.
         * \param prefab A reference to a JSON value where the serialized data will be stored.
         * \param componentName The name under which the component data should be saved in the JSON.
         */
        template<typename T>
        void SerializeComponent(Entity entity, Json::Value& prefab, const std::string& componentName) {
            if (HasComponent<T>(entity)) {
                auto& component = GetComponent<T>(entity);
                prefab[componentName] = component.Serialize();
            }
        }
        
        /**
         * \brief Deserializes a component from a JSON format and attaches it to an entity.
         *
         * This function allows for the restoration of component data from a saved state or from imported data.
         *
         * \tparam T The type of component to deserialize and attach.
         * \param entity The entity to which the deserialized component should be attached.
         * \param prefab A reference to a JSON value containing the serialized component data.
         * \param componentName The name under which the component data is stored in the JSON.
         */
        template<typename T>
        void DeserializeComponent(Entity entity, const Json::Value& prefab, const std::string& componentName) {
            if (prefab.isMember(componentName)) {
                AddComponent<T>(entity, T());
                auto& component = GetComponent<T>(entity);
                component.Deserialize(prefab[componentName]);
                mSpriteRemoved.int_value = entity;
                SendMessage(mSpriteRemoved);
                mSpriteAdded.int_value = entity;
                SendMessage(mSpriteAdded);
            }
        }

        /**
         * \brief Retrieves a system of the specified type.
         *
         * This function provides a way to access a specific system by its name. The returned system is cast to the desired type.
         *
         * \tparam T The desired type of system to retrieve.
         * \param systemName The name of the system to retrieve.
         * \return A shared pointer to the system of the specified type, or nullptr if not found.
         */
        template<typename T>
        std::shared_ptr<T> GetSystem(const std::string& systemName) {
            auto it = mAllSystems.find(systemName);
            if (it != mAllSystems.end()) {
                return std::dynamic_pointer_cast<T>(it->second);
            }
            else {
                // System not found
                return nullptr;
            }
        }

        /*!
         * \brief Pushes a layer to the GUI layer stack.
         *
         * \param layer The layer to be pushed.
         */
        void PushLayer(layer_type layer);

        /*!
         * \brief Pushes an overlay to the GUI layer stack.
         *
         * \param overlay The overlay to be pushed.
         */
        void PushOverlay(layer_type overlay);

        /*!
         * \brief Pops a layer from the GUI layer stack.
         *
         * \param layer The layer to be popped.
         */
        void PopLayer(layer_type layer);

        /*!
         * \brief Pops an overlay from the GUI layer stack.
         *
         * \param overlay The overlay to be popped.
         */
        void PopOverlay(layer_type overlay);

        int GetCurrentNumberOfSteps() { return currentNumberOfSteps; };
        //! Unique pointer to the Component Manager.
        std::unique_ptr<ComponentManager> mComponentManager;

        //! Unique pointer to the Entity Manager.
        std::unique_ptr<EntityManager> mEntityManager;

        //! Unique pointer to the System Manager.
        std::unique_ptr<SystemManager> mSystemManager;
    #ifdef USING_IMGUI
        std::shared_ptr<ImGuiLayer> GetImGuiLayer() { return mImGuiLayer; }
        std::shared_ptr<EditorLayer> GetEditorLayer() { return mEditorLayer; }
    #endif // USING_IMGUI

        int currentNumberOfSteps = 0;
        double LimitFPS(double frame_start);

        //! Duration of the delta time between frames.
        double mDeltaTime{ 0.0 };

        //! Counter for elapsed time since start of Engine.
        double mElapsedTime{ 0.0 };

        //! Fixed delta time between frames
        //std::chrono::duration<float> mFixedDeltaTime{ 1.f / 60.f };
        double mFixedDeltaTime{ 1.0 / 60.0 };
    private:
        //! Counter for the number of frames.
        unsigned mFrameCount = 0;


        //! Limits the frames per second. Can potentially be exposed to public if needed.
        std::chrono::high_resolution_clock::time_point LimitFPS(const std::chrono::high_resolution_clock::time_point& frameStart);

        //! Indicates if the game is currently running.
        bool mIsRunning;

        //! Tracks the last runtime of the engine.
        unsigned mLastRuntime;

        double accumulatedTime = 0.0;

        //! Pointer to the target frames per second for the game.
        int* mTargetFPS = nullptr;

    #ifdef USING_IMGUI
        std::shared_ptr<ImGuiLayer> mImGuiLayer;
        std::shared_ptr<EditorLayer> mEditorLayer;

    #endif // USING_IMGUI


        LayerStack mLayers;

        /**
         * \brief Default private constructor for the Singleton pattern.
         */
        InsightEngine();

        /**
         * \brief Destructor for the engine.
         */
        ~InsightEngine();

        /**
        * \brief Processes entities queued for deletion.
        */
        void ProcessEntityDeletion();
        
        /**
         * \brief Destroys a specified entity.
         * \param entity The entity to destroy.
         */
        void DestroyEntity(Entity entity);
        
        /**
         * \brief Map to store all systems by name.
         */
        std::unordered_map<std::string, std::shared_ptr<ParentSystem>> mAllSystems;

        /**
         * \brief Vector to store systems in their load sequence for faster iterations.
         */
        std::vector<std::shared_ptr<ParentSystem>> mSystemList;

        /**
         * \brief Map to store delta times for each system by name.
         */
        std::unordered_map<std::string, float> mSystemDeltas;

        /**
         * \brief Vector to store delta times of every engine frame.
         */
        std::vector<float> mlistOfDelta{ 0.f };

        /**
         * \brief Set to store entities that are flagged for deletion.
         */
        std::unordered_set<Entity> mEntitiesToDelete;

        /**
         * \brief Function To Deserialize All components
         */
        void DeserializeAllComponents(Entity entity, Json::Value& loaded);
        void DeserializeAllComponentsPrefab(Entity entity, Json::Value& loaded);

        /**
         * \brief Function for serializing all components.
         */
        void SerializeAllComponents(Entity entity, Json::Value& loaded);

        /**
         * \brief Push all imgui layers.
         */
        void PushImGuiLayers();

        /**
         * \brief Swap the front and back buffers.
         */
        void SwapBuffers();

        /**
         * \brief Poll for GLFW events. (should always run right after swapping buffers)
         */
        void PollEvents();

    };

}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_
//check if there is a component
