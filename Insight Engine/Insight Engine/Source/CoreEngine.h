#ifndef GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_

#include "System.h"
#include "Vector2D.h"
#include "Component.h"
#include "LayerStack.h"
#include <unordered_map>
#include <chrono>

namespace IS {
    class InsightEngine : public MessageListener {
    public:

        //override message handling this way the core engine also will recieve and send messages
        virtual void HandleMessage(const Message& message) override;

        //this is to instatiate only ONE engine
        static InsightEngine& Instance() {
            static InsightEngine instance;
            return instance;
        }

        //All Systems adding and removing of systems. It is integrated with the system manager
        void AddSystem(std::shared_ptr<ParentSystem> system, Signature signature);
        void DestroySystem(const std::string& name);
        void DestroyAllSystems();
        void InitializeAllSystems();

        //This is FPS
        void SetFPS(int num);

        //These are the functions for the core game loop;
        void Initialize();
        void Update();
        void Run();

        // layers
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        //eventmanager
        void sendMessage(Message* message) { EventManager::Instance().Broadcast(*message); };
        void Subscribe(MessageType type) { EventManager::Instance().Subscribe(type, this); };

        //Entity Creation
        Entity CreateEntity();
        void DestroyEntity(Entity entity);

        //Basic components functions
        template <typename T>
        bool HasComponent(Entity entity) {
            ComponentType type = mComponentManager->GetComponentType<T>();
            return mEntityManager->HasComponent(entity, type);
        }

        template <typename T>
        void RegisterComponent() {
            mComponentManager->RegisterComponent<T>();
        }

        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            mComponentManager->AddComponent<T>(entity, component);

            auto signature = mEntityManager->GetSignature(entity);
            signature.set(mComponentManager->GetComponentType<T>(), true);
            mEntityManager->SetSignature(entity, signature);

            mSystemManager->EntitySignatureChanged(entity, signature);
        }

        template<typename T>
        void RemoveComponent(Entity entity)
        {
            mComponentManager->RemoveComponent<T>(entity);

            auto signature = mEntityManager->GetSignature(entity);
            signature.set(mComponentManager->GetComponentType<T>(), false);
            mEntityManager->SetSignature(entity, signature);

            mSystemManager->EntitySignatureChanged(entity, signature);
        }

        template<typename T>
        T& GetComponent(Entity entity)
        {
            return mComponentManager->GetComponent<T>(entity);
        }

        template<typename T>
        ComponentType GetComponentType()
        {
            return mComponentManager->GetComponentType<T>();
        }

        // Abstracted Component Functions!
        template<typename... Components> // This ... will allow you to add as many Components as you want sort of like printf
        Entity CreateEntityWithComponents() {
            Entity entity = CreateEntity();
            (AddComponent<Components>(entity, Components()), ...);
            return entity;
        }
        //Example : Entity player = CreateEntityWithComponents<Position, Velocity, Health>();

        //Automatically updates the signature of an entity and adds a component
        template<typename T>
        void UpdateSignature(Entity entity) {
            //gets the entity signature
            auto signature = mEntityManager->GetSignature(entity);
            //adds the component to the signature
            signature.set(mComponentManager->GetComponentType<T>(), true);
            mEntityManager->SetSignature(entity, signature);
            mSystemManager->EntitySignatureChanged(entity, signature);
        }

        //automatically adds the component and updates the signature
        template<typename T>
        void AddComponentAndUpdateSignature(Entity entity, T component) {
            AddComponent<T>(entity, component);
            UpdateSignature<T>(entity);
        }

        //This generates a signature from the classes given
        template<typename... Components>
        Signature GenerateSignature() {
            Signature signature;
            // Use a fold expression to 'or' each component type into the signature
            (..., signature.set(mComponentManager->GetComponentType<Components>()));
            return signature;
        }





    private:
        //putting this here as a hard cap to fps, could move it to public as well
        std::chrono::high_resolution_clock::time_point LimitFPS(const std::chrono::high_resolution_clock::time_point& frameStart);
        bool is_running;
        //this is to create a map of key string and shared ptr to all systems. Instead of regular pointers.
        std::unordered_map<std::string, std::shared_ptr<ParentSystem>> all_systems;
        unsigned last_runtime;
        int targetFPS{ 60 };
        LayerStack layers;
        
        //follow the singleton pattern for only one engine
        InsightEngine();
        ~InsightEngine();

        //For ECS
        std::unique_ptr<ComponentManager> mComponentManager;
        std::unique_ptr<EntityManager> mEntityManager;
        std::unique_ptr<SystemManager> mSystemManager;

    };

}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_COREENGINE_H_
//check if there is a component



