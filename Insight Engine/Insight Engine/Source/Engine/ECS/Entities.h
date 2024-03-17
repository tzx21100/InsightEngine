/*!
 * \file Entities.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2451
 * \date 26-09-2023
 * \brief
 * This header file defines the implementation of the Entities as well as define
 * what a Signature and ComponentType is. This is the core "Entity" of our ECS
 * Structure. Only the core Engine should directly call any of the functions here.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
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
#define MAX_ENTITIES 20'0000
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
			mEntitiesAlive = 0;
		}

		/**
		 * \brief Creates an entity with a given name.
		 *
		 * \param name The name of the entity to be created.
		 * \return The ID of the newly created entity.
		 */
		Entity CreateEntity(const std::string& name) {

			Entity id;

			if (!mAvailableEntityIDs.empty()) {
				// Reuse an available entity ID
				id = mAvailableEntityIDs.front();
				mAvailableEntityIDs.pop_front();
			}
			else {
				if (mEntitiesAlive >= MAX_ENTITIES) {
					IS_CORE_WARN("Too many entities in existence.");
					return MAX_ENTITIES + 1;
				}
				id = mEntitiesAlive;
				mEntitiesAlive++;
			}


			// Set the name for the entity
			mEntityNames[name].push_back(id);
			// Set the entity for the name
			mEntityIds[id] = name;

			// Initialize its signature as empty in the mSignatures map
			mSignatures[id] = Signature();

			return id;
		}

		/**
		 * \brief Destroys an entity given its ID.
		 *
		 * \param entity The ID of the entity to be destroyed.
		 */
		void DestroyEntity(Entity entity) {
			//if (mSignatures.find(entity) == mSignatures.end()) {
			//	IS_CORE_WARN("Entity {} not found!", entity);
			//	return;
			//}

			// Remove the entity's signature
			mSignatures.erase(entity);

			// Remove the mappings for this entity from the maps
			std::string entityName = mEntityIds[entity];
			mEntityIds.erase(entity);

			// Remove the entity from the list of entities with its name
			auto& entitiesWithName = mEntityNames[entityName];
			entitiesWithName.erase(std::remove(entitiesWithName.begin(), entitiesWithName.end(), entity), entitiesWithName.end());

			if (entitiesWithName.empty()) {
				mEntityNames.erase(entityName);
			}

			mAvailableEntityIDs.push_back(entity);

			// Decrement the count of living entities
			//--mEntitiesAlive;

			IS_CORE_WARN("Entity {} destroyed!", entity);
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
			if (mSignatures.find(entity) != mSignatures.end()) {
				return mSignatures[entity];
			}
			return 0;  // Return an empty signature if not found
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
			auto it = mSignatures.find(entity);
			if (it != mSignatures.end()) {
				return it->second.any();
			}
			return false;
		}

		/**
		 * \brief Resets all entities, making them available for reuse.
		 */
		void ResetEntityID() {
			mEntitiesAlive = 0;

			mAvailableEntityIDs.clear();
			// Clear all entity signatures
			mSignatures.clear();
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
		std::vector<Entity> FindEntitiesByName(const std::string& name) {
			if (mEntityNames.find(name) != mEntityNames.end()) {
				return mEntityNames[name];
			}
			return {};  // Return an empty vector if no entities found
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

		void SetName(Entity entity, std::string name) {
			std::string entityName = mEntityIds[entity];
			auto& entitiesWithName = mEntityNames[entityName];
			entitiesWithName.erase(std::remove(entitiesWithName.begin(), entitiesWithName.end(), entity), entitiesWithName.end());

			if (entitiesWithName.empty()) {
				mEntityNames.erase(entityName);
			}

			mEntityNames[name].push_back(entity);
			mEntityIds[entity] = name;
		}

		/**
		 * \brief Gets the number of currently alive entities.
		 *
		 * \return The number of alive entities.
		 */
		uint32_t EntitiesAlive() { return static_cast<uint32_t>(mEntityIds.size()); };

		/**
		 * \brief Retrieves a map of all alive entities and their names.
		 *
		 * \return A reference to the map of alive entities.
		 */
		std::unordered_map<Entity, std::string>& GetEntitiesAlive() { return mEntityIds; }

		// Total living entities
		uint32_t mEntitiesAlive;
		// Store available entities
		std::deque<Entity> mAvailableEntityIDs;
		// Entity and signature
		std::unordered_map<Entity, Signature> mSignatures;
		// The name of entities are now stored like this to handle multiple names :)
		std::unordered_map<std::string, std::vector<Entity>> mEntityNames;
		// Finding the name by the id
		std::unordered_map<Entity, std::string>mEntityIds;
	private:

		

	};

}


#endif //ECS HEADER