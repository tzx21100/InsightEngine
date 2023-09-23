#ifndef GAM200_INSIGHT_ENGINE_SOURCE_COMPONENT_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_COMPONENT_H_
#include "Pch.h"
#include <json/json.h>
#include "Entities.h"

namespace IS {

	//this is just a parent class for all components
	class IComponent {
	public:
		virtual Json::Value Serialize() {
			//I give a default intialization so that not all components have to use this
			Json::Value empty;
			return empty;
		}

		virtual void Deserialize(Json::Value) {
			//This is a empty default init so that not all components have to override
		}


	};

	//parent component array this allows for destruction of entites of ANY types
	class IComponentArray {
	public:
		virtual ~IComponentArray() {};
		virtual void EntityDestroyed(Entity entity) = 0;//informs when an entity is destroyed
		virtual ComponentType CloneData(Entity entity, Entity old_entity) = 0; //this is to clone a component
		//Small functions to set and get the component type
		void SetComponentType(ComponentType id) {
			mComponentType = id;
		}
		ComponentType GetComponentType() {
			return mComponentType;
		}
	private:
		// The Component Type of the array
		ComponentType mComponentType;
	};

	//This is a templated class so that it can be any component
	template<typename T>
	class ComponentArray : public IComponentArray {
	public:

		void InsertData(Entity entity, T component) {
			assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

			// Put new entry at end and update the maps
			size_t new_index = mSize;
			mEntityToIndexMap[entity] = new_index;
			mIndexToEntityMap[new_index] = entity;
			mComponentArray[new_index] = component;
			++mSize;
		}

		void RemoveData(Entity entity) {
			assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

			// Copy element at end into deleted element's place to maintain density
			size_t removed_index = mEntityToIndexMap[entity];
			size_t last_index = mSize - 1;
			mComponentArray[removed_index] = mComponentArray[last_index];

			// Update map to point to moved spot
			Entity entityOfLastElement = mIndexToEntityMap[last_index];
			mIndexToEntityMap[removed_index] = entityOfLastElement;
			mEntityToIndexMap[entityOfLastElement] = removed_index;
			
			// Erase from map
			mEntityToIndexMap.erase(entity);
			mIndexToEntityMap.erase(last_index);

			--mSize;
		}

		T& GetData(Entity entity) {
			assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");
			// Return a reference to the entity's component
			return mComponentArray[mEntityToIndexMap[entity]];
		}

		void EntityDestroyed(Entity entity) override {
			if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end()){
				// Remove the entity's component if it existed
				RemoveData(entity);
			}
		}

		ComponentType CloneData(Entity entity,Entity old_entity) override{
			//if the old_entity exists
			if (mEntityToIndexMap.find(old_entity) != mEntityToIndexMap.end()) {
				T component = GetData(old_entity);
				InsertData(entity, component);
				return this->GetComponentType();
			}
			return MAX_COMPONENTS+1;
		}


	private:
		// The packed array of components (of generic type T),
		// set to a specified maximum amount, matching the maximum number
		// of entities allowed to exist simultaneously, so that each entity
		// has a unique spot.
		std::array<T, MAX_ENTITIES> mComponentArray;
		// Map from an entity ID to an array index.
		std::unordered_map<Entity, size_t> mEntityToIndexMap;
		// Map from an array index to an entity ID.
		std::unordered_map<size_t, Entity> mIndexToEntityMap;
		// Total size of valid entries in the array.
		size_t mSize;
	};

	class ComponentManager
	{
	public:

		//method to get all component types
		const std::unordered_map<const char*, ComponentType>& GetRegisteredComponents() const {
			return mComponentTypes;
		}


		template<typename T>
		void RegisterComponent() {
			const char* type_name = typeid(T).name();
			assert(mComponentTypes.find(type_name) == mComponentTypes.end() && "Registering component type more than once.");
			// Add this component type to the component type map
			mComponentTypes.insert({ type_name, mNextComponentType });
			// Create a ComponentArray pointer and add it to the component arrays map
			mComponentArrays.insert({ type_name, std::make_shared<ComponentArray<T>>() });
			// Add the componentType to the value
			mComponentArrays[type_name]->SetComponentType(mNextComponentType);
			// Increment the value so that the next component registered will be different
			++mNextComponentType;
			IS_CORE_INFO("Component {} registered!", type_name);
		}

		template<typename T>
		ComponentType GetComponentType() {
			const char* type_name = typeid(T).name();
			assert(mComponentTypes.find(type_name) != mComponentTypes.end() && "Component not registered before use.");
			// Return this component's type - used for creating signatures
			return mComponentTypes[type_name];
		}

		template<typename T>
		void AddComponent(Entity entity, T component) {
			// Add a component to the array for an entity
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity) {
			// Remove a component from the array for an entity
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(Entity entity) {
			// Get a reference to a component from the array for an entity
			return GetComponentArray<T>()->GetData(entity);
		}

		void EntityDestroyed(Entity entity) {
			// Notify each component array that an entity has been destroyed
			// If it has a component for that entity, it will remove it
			for (auto const& pair : mComponentArrays) {
				auto const& component = pair.second;
				component->EntityDestroyed(entity);
			}
		}


		//This is to clone the components
		Signature CloneComponent(Entity entity, Entity old_entity) {
			ComponentType componentType;
			Signature returned_signature;
			for (auto const& pair : mComponentArrays) {
				auto const& component = pair.second;
				componentType = component->CloneData(entity, old_entity);
				if (componentType<=MAX_COMPONENTS) {
					returned_signature.set(componentType);
				}
				
			}
			return returned_signature;
		}


	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> mComponentTypes{};
		// Map from type string pointer to a component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType mNextComponentType{};
		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray(){
			const char* type_name = typeid(T).name();
			assert(mComponentTypes.find(type_name) != mComponentTypes.end() && "Component not registered before use.");
			return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[type_name]);
		}
	};





}

#endif //component