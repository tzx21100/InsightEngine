/*!
 * \file System.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This header file defines the implementation of the SystemManager, parentsystem
 * and their functionalities. These functions should be abstracted by the core Engine
 * excluding the ParentSystem.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_SYSTEM_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_SYSTEM_H_

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Entities.h"
#include "Message.h"
#include "EventManager.h"
#include <string>
#include <unordered_map>
#include <set>


/* create a virtual system class for all systems used in the game
this class should be the parent class for all systems used E.g. Graphics
all are virtual so they can be overwritten by the other systems when initing*/

namespace IS {

    /**
   * \class ParentSystem
   * \brief A base class for all systems used in the game.
   *
   * This class provides basic methods for handling messages and entities, and should
   * be the parent class for all specialized systems like Graphics.
   */
    class ParentSystem :public MessageListener {
    public:
         /**
         * \brief Sends a message to the EventManager to broadcast.
         *
         * \param message The message to be broadcasted.
         */
        void SendMessage(Message* message) { EventManager::Instance().Broadcast(*message); };

         /**
         * \brief Subscribes the system to listen to a specific message type.
         *
         * \param type The type of message to listen to.
         */
        void Subscribe(MessageType type) { EventManager::Instance().Subscribe(type, this); };

        /**
         * \brief Initializes the system. Intended to be overridden by derived systems.
         */
        virtual void Initialize() {};

        /**
         * \brief Retrieves the name of the system. Intended to be overridden by derived systems.
         *
         * \return The name of the system.
         */
        virtual std::string GetName() { return ""; };

        /**
         * \brief Updates the system. Intended to be overridden by derived systems.
         *
         * \param deltaTime Time since the last update.
         */
        virtual void Update(float) {};

        /**
         * \brief Draws or renders components of the system. Intended to be overridden by derived systems.
         *
         * \param deltaTime Time since the last draw call.
         */
        virtual void Draw(float) {};

        
        /**
         * \brief Destructor for ParentSystem.
         */
        virtual ~ParentSystem(){}

        //this is for ECS each system will have their own entities
        std::set<Entity> mEntities;

    };

    /**
     * \class SystemManager
     * \brief Manages the various systems used in the game.
     *
     * The SystemManager class is responsible for registering systems, managing their signatures,
     * and notifying them about entity changes.
     */
    class SystemManager
    {
    public:
        /**
         * \brief Registers a system with the SystemManager.
         *
         * \param system The system to be registered.
         */
        void RegisterSystem(std::shared_ptr<ParentSystem> system) {
            std::string systemName = system->GetName();
            assert(mSystems.find(systemName) == mSystems.end() && "Registering system more than once.");
            mSystems[systemName] = system;
        }

        /**
         * \brief Sets the signature for a specific system.
         *
         * \param systemName The name of the system.
         * \param signature The signature to set for the system.
         */
        void SetSignature(const std::string& systemName, Signature signature) {
            assert(mSystems.find(systemName) != mSystems.end() && "System used before registered.");

            // Set the signature for this system
            mSignatures[systemName] = signature;
        }

        /**
         * \brief Notifies all systems that an entity has been destroyed.
         *
         * \param entity The ID of the destroyed entity.
         */
        void EntityDestroyed(Entity entity) {
            // Erase a destroyed entity from all system lists
            // mEntities is a set so no check needed
            for (auto const& pair : mSystems) {
                auto const& system = pair.second;
                system->mEntities.erase(entity);
            }
        }

        /**
         * \brief Updates the entity associations for all systems based on an entity's signature.
         *
         * \param entity The ID of the entity whose signature has changed.
         * \param entitySignature The new signature of the entity.
         */
        void EntitySignatureChanged(Entity entity, Signature entitySignature) {
            // Notify each system that an entity's signature changed
            for (auto const& pair : mSystems) {
                auto const& type = pair.first;
                auto const& system = pair.second;
                auto const& systemSignature = mSignatures[type];

                //Any component that matches will be added to the system
                if ((entitySignature & systemSignature)==systemSignature) {
                    system->mEntities.insert(entity);
                }
                // Remove thoese that no longer match (even though we have an edge case of HasComponent in each system)
                else {
                    system->mEntities.erase(entity);
                }
            }
        }

        void ClearEntities() {
            for (auto const& pair : mSystems) {
                pair.second->mEntities.clear();
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