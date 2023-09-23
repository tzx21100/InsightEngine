#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ECS_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_ECS_H_


#include <cassert>
#include <bitset>
#include <array>
#include <deque>
#include <queue>


namespace IS {

	//We can move all these definations elsewhere when we have a main GLOBAL var page
	// Entity is just a number
	using Entity = std::uint32_t;
	// Limits how many entities we can have
	const Entity MAX_ENTITIES = 200000;

	// Define the component
	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32; //this decides the signature

	// Set the signature as the max components. In this case 32 so each entity can have up to 32 components
	using Signature = std::bitset<MAX_COMPONENTS>;

	// Entity Manager Class, this is an O(1) function for creating, destroying, and updating entities
	class EntityManager {
	public:

		EntityManager() {
			// Initialize the queue with all possible entity IDs
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
				mAvailableEntities.push(entity);
			}
			mLivingEntityCount = 0;
		}

		//overloaded create entity function
		Entity CreateEntity(const std::string& name) {
			assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

			Entity id = mAvailableEntities.front();
			mAvailableEntities.pop();
			mLivingEntityCount++;

			// Set the name for the entity
			mEntityNames[name] = id;
			// Set the entity for the name
			mEntityIds[id] = name;

			return id;
		}

		void DestroyEntity(Entity entity) {
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Invalidate the destroyed entity's signature
			mSignatures[entity].reset();

			// Remove the mappings for this entity from the maps
			std::string entityName = mEntityIds[entity];
			mEntityNames.erase(entityName);
			mEntityIds.erase(entity);

			// Put the destroyed ID at the back of the queue
			mAvailableEntities.push(entity);
			--mLivingEntityCount;
			IS_WARN("Entity {} destroyed!", entity);
		}

		void SetSignature(Entity entity, Signature signature) {
			assert(entity < MAX_ENTITIES && "Entity out of range.");
			// Put this entity's signature into the array
			mSignatures[entity] = signature;
		}

		Signature GetSignature(Entity entity) {
			assert(entity < MAX_ENTITIES && "Entity out of range.");
			// Get this entity's signature from the array
			return mSignatures[entity];
		}

		//see if they have the component
		bool HasComponent(Entity entity, ComponentType componentType) {
			assert(entity < MAX_ENTITIES && "Entity out of range.");
			Signature signature = GetSignature(entity);
			return signature.test(componentType);
		}

		//see if they are alive
		bool IsEntityAlive(Entity entity) const {
			assert(entity < MAX_ENTITIES && "Entity out of range.");
			return mSignatures[entity].any();  // Returns true if any bit is set
		}



		//function to find the entity by name
		Entity FindEntity(const std::string& name) {
			return mEntityNames[name];
		}

		//function to find the name by entites
		std::string FindNames(Entity entity) {
			return mEntityIds[entity];
		}


		// return number of entities alive
		uint32_t EntitiesAlive() { return mLivingEntityCount; };

		std::unordered_map<Entity, std::string>& GetEntitiesAlive() { return mEntityIds; }

	private:
		// Queue of unused entity IDs
		std::queue<Entity> mAvailableEntities;
		// Array of signatures where the index corresponds to the entity ID
		std::array<Signature, MAX_ENTITIES> mSignatures;
		// Total living entities - used to keep limits on how many exist
		uint32_t mLivingEntityCount;
		// The name of entities
		std::unordered_map<std::string,Entity >mEntityNames;
		// Finding the name by the id
		std::unordered_map<Entity ,std::string>mEntityIds;
	};

}


#endif //ECS HEADER