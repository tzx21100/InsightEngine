/*!
 * \file Entities.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This header file defines the implementation of the Entities as well as define
 * what a Signature and ComponentType is. This is the core "Entity" of our ECS
 * Structure. Only the core Engine should directly call any of the functions here.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ECS_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_ECS_H_

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include <cassert>
#include <bitset>
#include <array>
#include <deque>
#include <queue>

 /*                                                                   macros
 ----------------------------------------------------------------------------- */
#define MAX_ENTITIES 200000
#define MAX_COMPONENTS 32

namespace IS {

	// Entity is just a number
	using Entity = std::uint32_t;
	// Define the component
	using ComponentType = std::uint8_t;
	// Set the signature as the max components. In this case 32 so each entity can have up to 32 components
	using Signature = std::bitset<MAX_COMPONENTS>;

	/**
	 * \class EntityManager
	 * \brief Manages the creation, destruction, and querying of entities.
	 *
	 * This class provides efficient mechanisms for entity management using a queue
	 * for available entity IDs and bitset signatures for component tracking.
	 */
	class EntityManager {
	public:

		/**
		 * \brief Constructs the EntityManager and initializes the entity queue.
		 */
		EntityManager() {
			// Initialize the queue with all possible entity IDs
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
				mEntityQueue.push(entity);
			}
			mEntitiesAlive = 0;
		}

		/**
		 * \brief Creates an entity with a given name.
		 *
		 * \param name The name of the entity to be created.
		 * \return The ID of the newly created entity.
		 */
		Entity CreateEntity(const std::string& name) {
			if (mEntitiesAlive > MAX_ENTITIES) { IS_CORE_WARN("Too many entities in existence."); }

			Entity id = mEntityQueue.front();
			mEntityQueue.pop();
			mEntitiesAlive++;

			// Set the name for the entity
			mEntityNames[name] = id;
			// Set the entity for the name
			mEntityIds[id] = name;

			return id;
		}

		/**
		 * \brief Destroys an entity given its ID.
		 *
		 * \param entity The ID of the entity to be destroyed.
		 */
		void DestroyEntity(Entity entity) {
			if (entity < MAX_ENTITIES) {  
				// Invalidate the destroyed entity's signature
				mSignatures[entity].reset();
			}
			// Remove the mappings for this entity from the maps
			std::string entityName = mEntityIds[entity];
			mEntityNames.erase(entityName);
			mEntityIds.erase(entity);

			// Put the destroyed ID at the back of the queue
			mEntityQueue.push(entity);
			--mEntitiesAlive;
			IS_WARN("Entity {} destroyed!", entity);
		}

		/**
		 * \brief Sets the signature of an entity.
		 *
		 * \param entity The ID of the entity whose signature is to be set.
		 * \param signature The signature to set for the entity.
		 */
		void SetSignature(Entity entity, Signature signature) {
			assert(entity < MAX_ENTITIES && "Entity out of range.");
			// Put this entity's signature into the array
			mSignatures[entity] = signature;
		}

		/**
		 * \brief Retrieves the signature of an entity.
		 *
		 * \param entity The ID of the entity whose signature is to be retrieved.
		 * \return The signature of the specified entity.
		 */
		Signature GetSignature(Entity entity) {
			if (entity < MAX_ENTITIES) {
				return mSignatures[entity];
			}
			return 0;
		}

		/**
		 * \brief Checks if an entity has a specific component.
		 *
		 * \param entity The ID of the entity to check.
		 * \param componentType The type of the component to check for.
		 * \return \c true if the entity has the component, \c false otherwise.
		 */
		bool HasComponent(Entity entity, ComponentType componentType) {
			if (entity > MAX_ENTITIES) { IS_CORE_WARN("Entity out of range."); }
			Signature signature = GetSignature(entity);
			return signature.test(componentType);
		}

		/**
		 * \brief Checks if an entity is alive.
		 *
		 * \param entity The ID of the entity to check.
		 * \return \c true if the entity is alive, \c false otherwise.
		 */
		bool IsEntityAlive(Entity entity) const {
			if (entity < MAX_ENTITIES) { 
				return mSignatures[entity].any();
			}
		}

		/**
		 * \brief Resets all entities, making them available for reuse.
		 */
		void ResetEntityID() {
			// Clear the existing queue of available entity IDs
			while (!mEntityQueue.empty()) {
				mEntityQueue.pop();
			}
			// Re-initialize the queue with all possible entity IDs starting from 0
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
				mEntityQueue.push(entity);
			}
			// Reset living entity count to 0
			mEntitiesAlive = 0;
			// Clear all entity signatures
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
				mSignatures[entity].reset();
			}
			// Clear the name to entity and entity to name mappings
			mEntityNames.clear();
			mEntityIds.clear();

			IS_CORE_DEBUG("All entities reset!");
		}

		/**
		 * \brief Finds the ID of an entity given its name.
		 *
		 * \param name The name of the entity to find.
		 * \return The ID of the entity with the specified name.
		 */
		Entity FindEntity(const std::string& name) {
			return mEntityNames[name];
		}

		/**
		 * \brief Retrieves the name of an entity given its ID (const version).
		 *
		 * \param entity The ID of the entity whose name is to be retrieved.
		 * \return The name of the specified entity.
		 */
		std::string FindNames(Entity entity) const {
			return mEntityIds.at(entity);
		}

		/**
		 * \brief Retrieves the name of an entity given its ID.
		 *
		 * \param entity The ID of the entity whose name is to be retrieved.
		 * \return A reference to the name of the specified entity.
		 */
		std::string& FindNames(Entity entity) {
			return mEntityIds[entity];
		}

		/**
		 * \brief Gets the number of currently alive entities.
		 *
		 * \return The number of alive entities.
		 */
		uint32_t EntitiesAlive() { return mEntitiesAlive; };

		/**
		 * \brief Retrieves a map of all alive entities and their names.
		 *
		 * \return A reference to the map of alive entities.
		 */
		std::unordered_map<Entity, std::string>& GetEntitiesAlive() { return mEntityIds; }

	private:
		// Queue of unused entity IDs
		std::queue<Entity> mEntityQueue;
		// Array of signatures where the index corresponds to the entity ID
		std::array<Signature, MAX_ENTITIES> mSignatures;
		// Total living entities - used to keep limits on how many exist
		uint32_t mEntitiesAlive;
		// The name of entities
		std::unordered_map<std::string,Entity >mEntityNames;
		// Finding the name by the id
		std::unordered_map<Entity ,std::string>mEntityIds;
	};

}


#endif //ECS HEADER