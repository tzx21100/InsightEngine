/*!
 * \file Component.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This header file defines the implementation of the Components in our
 * ECS system. Only the CoreEngine should be called to access the ECS and these
 * functions shouldn't really be touched.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_ECS_COMPONENT_H_
#define GAM200_INSIGHT_ENGINE_ECS_COMPONENT_H_

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Engine/Core/Core.h"
#include "Debug/Profiling/Timer.h"
#include "Entities.h"

#include <json/json.h>
#include <memory>
#include <unordered_map>
#include <string>

namespace IS {

	/**
	 * \class IComponent
	 * \brief A base class for all components.
	 *
	 * This class provides basic serialization and deserialization methods
	 * for components, which can be overridden by derived component classes.
	 */
	class IS_API IComponent {
	public:
		/**
		 * \brief Serialize the component into a JSON format.
		 *
		 * Derived component classes can override this to provide custom
		 * serialization logic.
		 *
		 * \return A Json::Value containing the serialized data.
		 */
		virtual Json::Value Serialize() {
			//I give a default intialization so that not all components have to use this
			Json::Value empty;
			return empty;
		}

		 /**
         * \brief Deserialize the component from a JSON format.
         * 
         * Derived component classes can override this to provide custom 
         * deserialization logic.
         * 
         * \param A Json::Value containing the serialized data.
         */
		virtual void Deserialize(Json::Value) {
			//This is a empty default init so that not all components have to override
		}

		//virtual std::string GetType() const = 0;

	};

	/**
	 * \class IComponentArray
	 * \brief A base class for arrays of components.
	 *
	 * This class provides basic methods for handling entities and their
	 * associated components in a generic manner.
	 */
	class IS_API IComponentArray {
	public:
		virtual ~IComponentArray() {};

		/**
		 * \brief Called when an entity is destroyed to handle any associated components.
		 *
		 * \param entity The ID of the entity being destroyed.
		 */
		virtual void DestroyEntity(Entity entity) = 0;

		/**
		 * \brief Clone the data of a component from one entity to another.
		 *
		 * \param entity The ID of the new entity.
		 * \param old_entity The ID of the old entity to clone from.
		 * \return The ComponentType of the cloned component.
		 */
		virtual ComponentType CloneData(Entity entity, Entity old_entity) = 0;

		/**
		 * \brief Match a signature to add default components to an entity.
		 *
		 * This is useful for matching the prefab's signature to get their components
		 * 
		 * \param signature The signature to match against.
		 * \param entity The entity to which components should be added.
		 * \return The ComponentType of the added component.
		 */
		virtual ComponentType SignatureMatch(Signature signature, Entity entity)=0;

		/**
		 * \brief Set the component type for this array.
		 *
		 * \param id The ComponentType to set.
		 */
		void SetComponentType(ComponentType id) {
			mComponentType = id;
		}

		/**
		 * \brief Get the component type for this array.
		 *
		 * \return The ComponentType of this array.
		 */
		ComponentType GetComponentType() {
			return mComponentType;
		}

		virtual void ClearAllEntities() = 0;

		virtual std::shared_ptr<IComponentArray> clone() const = 0;

	private:
		// The Component Type of the array
		ComponentType mComponentType{};
	};

	/**
	 * \class ComponentArray
	 * \brief A templated class for managing components associated with entities.
	 *
	 * This class provides specific methods for handling entities and their
	 * associated components of type T. It is just a map of the component arrays.
	 *
	 * \tparam T The type of the component stored in the array.
	 */
	template<typename T>
	class IS_API ComponentArray : public IComponentArray {
	public:

		std::shared_ptr<IComponentArray> clone() const override {
			return std::make_shared<ComponentArray>(*this);
		}

		/**
		 * \brief Inserts a component for a given entity.
		 *
		 * Inserts the component at the end of the component array, and updates
		 * entity-to-index and index-to-entity maps.
		 *
		 * \param entity The ID of the entity associated with the component.
		 * \param component The component data to be inserted.
		 */
		void InsertComponentData(Entity entity, T component) {
			if (mSize >= 20000) { return; }
			// Put new entry at end and update the maps
			mComponentArray.insert({ entity,component });
			mSize++;
		}

		/**
		 * \brief Removes the component associated with a given entity.
		 *
		 * Copies the last component to the removed component's spot to maintain
		 * density, updates entity-to-index and index-to-entity maps, and decreases
		 * the size of valid entries.
		 *
		 * \param entity The ID of the entity whose component is to be removed.
		 */
		void RemoveComponentData(Entity entity) {

			mComponentArray.erase(entity);
			mSize--;
		}

		/**
		* \brief Retrieves the component associated with a given entity.
		*
		* Returns a reference to the component of the specified entity.
		*
		* \param entity The ID of the entity whose component is to be retrieved.
		* \return A reference to the component.
		*/
		T& GetComponentData(Entity entity) {

			// Return a reference to the entity's component
			return mComponentArray[entity];
		}

		/**
		* \brief Removes the component associated with a destroyed entity.
		*
		* If a component exists for the destroyed entity, it is removed.
		*
		* \param entity The ID of the entity being destroyed.
		*/
		void DestroyEntity(Entity entity) override {
			if (mComponentArray.find(entity) != mComponentArray.end()){
				// Remove the entity's component if it existed
				RemoveComponentData(entity);
			}
		}

		/**
		 * \brief Clones the data of a component from one entity to another.
		 *
		 * If the old entity has a component, the component is cloned to the new entity.
		 *
		 * \param entity The ID of the new entity.
		 * \param old_entity The ID of the old entity to clone from.
		 * \return The ComponentType of the cloned component or (MAX_COMPONENTS + 1) if no component was cloned.
		 */
		ComponentType CloneData(Entity entity,Entity old_entity) override{
			//if the old_entity exists
			if (mComponentArray.find(old_entity) != mComponentArray.end()) {
				T component = GetComponentData(old_entity);
				InsertComponentData(entity, component);
				return this->GetComponentType();
			}
			return MAX_COMPONENTS+1;
		}
		
		/**
		 * \brief Matches a signature to add default components to an entity.
		 *
		 * If the signature matches the component type, a default component is added to the entity.
		 *
		 * \param signature The signature to match against.
		 * \param entity The entity to which a default component should be added.
		 * \return The ComponentType of the added component or (MAX_COMPONENTS + 1) if no component was added.
		 */
		ComponentType SignatureMatch(Signature signature, Entity entity) {
			if (signature.test(this->GetComponentType())) {
				T component{};
				InsertComponentData(entity, component);
				return this->GetComponentType();
			}
			return MAX_COMPONENTS+1;
		}

		//clear all entities in the component
		void ClearAllEntities() override {
			mComponentArray.clear();
			mSize = 0;
		}


	private:
		/**
		 * \brief A packed array of components.
		 *
		 * The array is of generic type T and is set to a specified maximum amount,
		 * matching the maximum number of entities allowed to exist simultaneously.
		 */
		std::unordered_map<Entity, T> mComponentArray{};

		/**
		 * \brief The total size of valid entries in the array.
		 *
		 * This size increases or decreases as components are added or removed.
		 */
		size_t mSize;
	};

	/**
	 * \class ComponentManager
	 * \brief A manager class for handling and organizing all registered components.
	 *
	 * This class provides various methods to manage components associated with entities.
	 * It also maintains maps to associate component types with their respective arrays.
	 */
	class IS_API ComponentManager
	{
	public:

		/**
		 * \brief Retrieves a map of all registered component types.
		 *
		 * Returns a map linking the type name of the component to its assigned type.
		 *
		 * \return An unordered map of registered components.
		 */
		const std::unordered_map<std::string, ComponentType>& GetRegisteredComponents() const {
			return mRegisteredComponentType;
		}

		/**
		* \brief Registers a new component type.
		*
		* Adds the component type to the manager and initializes its respective component array.
		*
		* \tparam T The type of the component to be registered.
		*/
		template<typename T>	
		void RegisterComponent() {
			IS_PROFILE_FUNCTION();

			std::string type_name = T::GetType();
			// Add this component type to the component type map
			mRegisteredComponentType.insert({ type_name, mNextComponentType });
			// Create a ComponentArray pointer and add it to the component arrays map
			mComponentArrayMap.insert({ type_name, std::make_shared<ComponentArray<T>>() });
			// Add the componentType to the value
			mComponentArrayMap[type_name]->SetComponentType(mNextComponentType);
			// Increment the value so that the next component registered will be different
			++mNextComponentType;
			IS_CORE_INFO("{} component registered!", type_name);
		}

		/**
		 * \brief Retrieves the type of a specific component.
		 *
		 * Returns the component type based on its type name.
		 *
		 * \tparam T The type of the component.
		 * \return The ComponentType of the specified component.
		 */
		template<typename T>
		ComponentType GetComponentType() {
			std::string type_name = T::GetType();
			// Return this component's type - used for creating signatures
			return mRegisteredComponentType[type_name];
		}

		/**
		 * \brief Adds a component for a given entity.
		 *
		 * Inserts the component into the respective component array for the entity.
		 *
		 * \tparam T The type of the component.
		 * \param entity The ID of the entity.
		 * \param component The component data to be added.
		 */
		template<typename T>
		void AddComponent(Entity entity, T component) {
			// Add a component to the array for an entity
			GetComponentArray<T>()->InsertComponentData(entity, component);
		}

		/**
		 * \brief Removes the component associated with a given entity.
		 *
		 * Removes the component from the respective component array for the entity.
		 *
		 * \tparam T The type of the component.
		 * \param entity The ID of the entity.
		 */
		template<typename T>
		void RemoveComponent(Entity entity) {
			// Remove a component from the array for an entity
			GetComponentArray<T>()->RemoveComponentData(entity);
		}

		/**
		 * \brief Retrieves the component associated with a given entity.
		 *
		 * Returns a reference to the component of the specified entity.
		 *
		 * \tparam T The type of the component.
		 * \param entity The ID of the entity.
		 * \return A reference to the component.
		 */
		template<typename T>
		T& GetComponent(Entity entity) {
			// Get a reference to a component from the array for an entity
			return GetComponentArray<T>()->GetComponentData(entity);
		}

		/**
		 * \brief Handles the destruction of an entity.
		 *
		 * Notifies all component arrays that an entity has been destroyed.
		 * If the entity has a component, the component will be removed.
		 *
		 * \param entity The ID of the entity being destroyed.
		 */
		void EntityDestroyed(Entity entity) {
			/* Notify each component array that an entity has been destroyed
			 * If it has a component for that entity, it will remove it
			 */
			for (auto const& pair : mComponentArrayMap) {
				auto const& component = pair.second;
				component->DestroyEntity(entity);
			}
		}

		/**
		 * \brief Clones the components from one entity to another.
		 *
		 * For each component of the old entity, if it exists, it gets cloned to the new entity.
		 *
		 * \param entity The ID of the new entity.
		 * \param old_entity The ID of the old entity to clone from.
		 * \return A signature indicating which components were cloned.
		 */
		Signature CloneComponent(Entity entity, Entity old_entity) {
			ComponentType componentType;
			Signature returned_signature;
			for (auto const& pair : mComponentArrayMap) {
				auto const& component = pair.second;
				componentType = component->CloneData(entity, old_entity);
				if (componentType<=MAX_COMPONENTS) {
					returned_signature.set(componentType);
				}
				
			}
			return returned_signature;
		}

		/**
		 * \brief Matches a prefab's signature to add components to an entity.
		 *
		 * For each component in the signature, if it matches, a default component is added to the entity.
		 *
		 * \param prefab The signature of the prefab to match against.
		 * \param entity The entity to which components should be added.
		 * \return A signature indicating which components were added.
		 */
		Signature PrefabToEntity(Signature prefab, Entity entity) {
			ComponentType componentType;
			Signature returned_signature;
			for (auto const& pair : mComponentArrayMap) {
				auto const& component = pair.second;
				componentType = component->SignatureMatch(prefab, entity);
				if (componentType <= MAX_COMPONENTS) {
					returned_signature.set(componentType);
				}

			}
			return returned_signature;
		}

		// Accessor function to clear all entities.
		void ClearEntities() {
			for (auto const& pair : mComponentArrayMap) {
				auto const& component = pair.second;
				component->ClearAllEntities();
			}
		}

		/**
		 * \brief Retrieves the component array for a specific component type.
		 *
		 * Returns a pointer to the component array of the specified type.
		 *
		 * \tparam T The type of the component.
		 * \return A shared pointer to the component array.
		 */
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray() {
			std::string type_name = T::GetType();
			assert(mRegisteredComponentType.find(type_name) != mRegisteredComponentType.end() && "Component not registered before use.");
			return std::static_pointer_cast<ComponentArray<T>>(mComponentArrayMap[type_name]);
		}

		/**
		 * \brief A map linking component type names to their respective component arrays.
		 */
		std::unordered_map<std::string, std::shared_ptr<IComponentArray>> mComponentArrayMap{};

	private:
		/**
		 * \brief A map linking component type names to their assigned types.
		 */
		std::unordered_map<std::string, ComponentType> mRegisteredComponentType{};

		

		/**
		 * \brief The next component type to be assigned.
		 *
		 * This value gets incremented each time a new component type is registered.
		 */
		ComponentType mNextComponentType{};
		
	};


}

#endif //component