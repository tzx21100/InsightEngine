#include "CoreEngine.h"
#pragma once
namespace IS {


	using SceneNumber= uint8_t;

	class SceneManager {

	public:
		// Singleton
		static SceneManager& Instance() { static SceneManager instance; return instance; }

		std::unordered_map<SceneNumber, std::string>mSceneName;
		std::unordered_map<SceneNumber, Entity> mSceneEntities;
		std::unordered_map<SceneNumber, std::unordered_map<Entity, Signature>> mSceneEntitySignatures;
		std::unordered_map<SceneNumber, std::unordered_map<std::string, Entity>> mSceneEntityNames;
		std::unordered_map<SceneNumber, std::unordered_map<Entity, std::string>> mSceneEntityIds;
		std::unordered_map<SceneNumber, std::unordered_map<std::string, std::shared_ptr<IComponentArray>>> mSceneComponents; // some data structure its an unordered tree :)

		void SaveScene() {
			auto &engine= InsightEngine::Instance();
			mSceneEntities.insert({mCurrentSceneNumber,engine.EntitiesAlive()});
			std::unordered_map<std::string, std::shared_ptr<IComponentArray>> original_map= engine.mComponentManager->mComponentArrayMap;
			std::unordered_map<std::string, std::shared_ptr<IComponentArray>> cloned_map;
			for (const auto& [key, value] : original_map) {
				cloned_map[key] = value->clone();
			}
			mSceneComponents[mCurrentSceneNumber] = cloned_map;
			
			//entiites
			mSceneEntitySignatures[mCurrentSceneNumber] = engine.mEntityManager->mSignatures;
			mSceneEntityNames[mCurrentSceneNumber] = engine.mEntityManager->mEntityNames;
			mSceneEntityIds[mCurrentSceneNumber] = engine.mEntityManager->mEntityIds;
		}

		void SwitchScene(SceneNumber scene) {
			auto& engine = InsightEngine::Instance();
			engine.NewScene();
			engine.mComponentManager->mComponentArrayMap = mSceneComponents[scene];
			std::unordered_map<std::string, std::shared_ptr<IComponentArray>> original_map = engine.mComponentManager->mComponentArrayMap;
			std::unordered_map<std::string, std::shared_ptr<IComponentArray>> cloned_map;
			for (const auto& [key, value] : original_map) {
				cloned_map[key] = value->clone();
			}
			mSceneComponents[scene] = cloned_map;
			mCurrentSceneNumber = scene;
			
			//entiites
			engine.mEntityManager->mEntitiesAlive = mSceneEntities[scene];
			engine.mEntityManager->mSignatures = mSceneEntitySignatures[scene];
			engine.mEntityManager->mEntityNames = mSceneEntityNames[scene];
			engine.mEntityManager->mEntityIds = mSceneEntityIds[scene];


			engine.mSystemManager->ClearEntities();
			for (auto& i : mSceneEntitySignatures[scene]) {
				engine.mSystemManager->EntitySignatureChanged(i.first, i.second);
			}

			IS_CORE_DEBUG("{}", mSceneEntities[scene]);

		}
		
		void CreateScene(std::string filename) {
			
			//checks for duplicates
			for (auto& i : mSceneName) {
				if (i.second == filename) { IS_CORE_DEBUG("Overiding Same Scene Name"); }
			}
			auto& engine = InsightEngine::Instance();
			mCurrentSceneNumber = mSceneCount;
			mSceneName[mSceneCount] = filename;
			SaveScene();
			mSceneCount++;
			IS_CORE_DEBUG("{}", mSceneCount);
			engine.SaveCurrentScene(filename);
		}

		void LoadScene(std::string filename) {	
			auto& engine = InsightEngine::Instance();
			engine.LoadScene(filename);
		}

	private:

		SceneNumber mCurrentSceneNumber{};
		SceneNumber mSceneCount{};

		SceneManager() {};
		~SceneManager() = default;




	};



}