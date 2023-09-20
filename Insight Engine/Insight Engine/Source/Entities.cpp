#include "Pch.h"
#include "Entities.h"  // Replace with the actual path of your header file

namespace IS {

	EntityManager::EntityManager() {
		// Initialize the queue with all possible entity IDs
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
			mAvailableEntities.push(entity);
		}
		mLivingEntityCount = 0;
	}

	Entity EntityManager::CreateEntity() {
		assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

		// Take an ID from the front of the queue
		Entity id = mAvailableEntities.front();
		mAvailableEntities.pop();
		++mLivingEntityCount;

		IS_TRACE("Entity {} created!", id);
		return id;
	}

	void EntityManager::DestroyEntity(Entity entity) {
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// Invalidate the destroyed entity's signature
		mSignatures[entity].reset();

		// Put the destroyed ID at the back of the queue
		mAvailableEntities.push(entity);
		--mLivingEntityCount;
		IS_WARN("Entity {} destroyed!", entity);
	}

	void EntityManager::SetSignature(Entity entity, Signature signature) {
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// Put this entity's signature into the array
		mSignatures[entity] = signature;
	}

	Signature EntityManager::GetSignature(Entity entity) {
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// Get this entity's signature from the array
		return mSignatures[entity];
	}


}