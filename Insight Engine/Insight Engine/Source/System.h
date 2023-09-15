#ifndef GAM200_INSIGHT_ENGINE_SOURCE_SYSTEM_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_SYSTEM_H_

#include "Entities.h"
#include"Message.h"
#include "EventManager.h"
#include <string>
#include <unordered_map>
#include <set>


/* create a virtual system class for all systems used in the game
this class should be the parent class for all systems used E.g. Graphics
all are virtual so they can be overwritten by the other systems when initing*/

namespace IS {

    //all systems should be able to listen to messages as well
    class ParentSystem :public MessageListener {
    public:
        //override the MessageListener
        //virtual void HandleMessage(const Message& message) override {};
        //sendMessage will use the InsightEngine to broadcast all the messages to everyone
        void SendMessage(Message* message) { EventManager::Instance().Broadcast(*message); };
        void Subscribe(MessageType type) { EventManager::Instance().Subscribe(type, this); };
        virtual void Initialize() {};
        virtual std::string getName() = 0;
        virtual void Update(float time) = 0;
        virtual void Draw() {};

        
        
        virtual ~ParentSystem(){}

        //this is for ECS each system will have their own entities
        std::set<Entity> mEntities;


    };

    // System Manager
    /* The reason for the system manager being seperated from the Insight Engine is because this
    makes the code less dependant on each other and so that I don't mix up the maps */
    class SystemManager
    {
    public:
        void RegisterSystem(std::shared_ptr<ParentSystem> system) {
            std::string systemName = system->getName();
            assert(mSystems.find(systemName) == mSystems.end() && "Registering system more than once.");
            mSystems[systemName] = system;
        }

        void SetSignature(const std::string& systemName, Signature signature) {
            assert(mSystems.find(systemName) != mSystems.end() && "System used before registered.");

            // Set the signature for this system
            mSignatures[systemName] = signature;
        }

        void EntityDestroyed(Entity entity) {
            // Erase a destroyed entity from all system lists
            // mEntities is a set so no check needed
            for (auto const& pair : mSystems) {
                auto const& system = pair.second;
                system->mEntities.erase(entity);
            }
        }

        void EntitySignatureChanged(Entity entity, Signature entitySignature) {
            // Notify each system that an entity's signature changed
            for (auto const& pair : mSystems) {
                auto const& type = pair.first;
                auto const& system = pair.second;
                auto const& systemSignature = mSignatures[type];

                //Any component that matches will be added to the system
                if ((entitySignature & systemSignature).any()) {
                    system->mEntities.insert(entity);
                }
                // Remove thoese that no longer match (even though we have an edge case of HasComponent in each system)
                else {
                    system->mEntities.erase(entity);
                }
            }
        }

    private:
        // Map from system name to a signature
        std::unordered_map<std::string, Signature> mSignatures;

        // Map from system name to a system pointer
        std::unordered_map<std::string, std::shared_ptr<ParentSystem>> mSystems;
    };

    
}


#endif //GAM200_INSIGHT_ENGINE_SOURCE_SYSTEM_H_