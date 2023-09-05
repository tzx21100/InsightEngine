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
	const Entity MAX_ENTITIES = 5000;

	// Define the component
	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;

	// Set the signature as the max components. In this case 32 so each entity can have up to 32 components
	using Signature = std::bitset<MAX_COMPONENTS>;

	// Entity Manager Class, this is an O(1) function for creating, destroying, and updating entities
	class EntityManager {
	public:
		EntityManager();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void SetSignature(Entity entity, Signature signature);
		Signature GetSignature(Entity entity);
		//see if they have the component
		bool HasComponent(Entity entity, ComponentType componentType) {
			assert(entity < MAX_ENTITIES && "Entity out of range.");
			Signature signature = GetSignature(entity);
			return signature.test(componentType);
		}


	private:
		// Queue of unused entity IDs
		std::queue<Entity> mAvailableEntities;

		// Array of signatures where the index corresponds to the entity ID
		std::array<Signature, MAX_ENTITIES> mSignatures;

		// Total living entities - used to keep limits on how many exist
		uint32_t mLivingEntityCount;
	};

}


#endif //ECS HEADER