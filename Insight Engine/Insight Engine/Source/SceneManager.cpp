/*!
 * \file SceneManager.cpp
 * \author  Tan Zheng Xun, t.zhengxun@digipen.edu
			Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-10-2023
 * \brief
 * This source file defines the implementation for class SceneManager,
 * which encapsulates the functionalities of a scene manager, which manages
 * the scenes in a game.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "SceneManager.h"

namespace IS {
	
	SceneManager& SceneManager::Instance() { static SceneManager instance; return instance; }

	void SceneManager::CreateScene(std::string const& scene_filename)
	{
		// Construct filepath
		std::filesystem::path filepath(scene_filename);
		std::string const& filename = filepath.stem().string();

		// Checks for if scene has already been loaded
		{
			auto found = std::find_if(mSceneNames.begin(), mSceneNames.end(), [filename](std::pair<SceneID, std::string> const& pair)
			{
				auto const& [scene_id, scene_name] = pair;
				return scene_name == filename;
			});

			if (found != mSceneNames.end())
			{
				IS_CORE_WARN("Scene {} already added!", filename);
				return;
			}
		}

		// Set as active scene
		mActiveSceneID = mSceneCount;
		mSceneNames[mSceneCount] = filename;

		SaveScene();
		SaveSceneToFile();

		mSceneCount++;
		IS_CORE_DEBUG("Current number of scenes : {}", mSceneCount);
		IS_CORE_DEBUG("Scene {} created successfully!", filename);
	}

	void SceneManager::LoadScene(std::string const& scene_filename)
	{

		// Construct filepath
		std::filesystem::path filepath(scene_filename);
		std::string const& filename = filepath.stem().string();

		// Checks for if scene has already been loaded
		{
			auto found = std::find_if(mSceneNames.begin(), mSceneNames.end(), [filename](std::pair<SceneID, std::string> const& pair)
			{
				auto const& [scene_id, scene_name] = pair;
				return scene_name == filename;
			});

			if (found != mSceneNames.end())
			{
				IS_CORE_WARN("Scene {} already added!", filename);
				return;
			}
		}

		InsightEngine::Instance().LoadScene(scene_filename);
		CreateScene(scene_filename);
	}

	void SceneManager::SaveScene()
	{
		// Instance of game engine
		auto& engine = InsightEngine::Instance();
		ECSMap original_map, cloned_map;
		mSceneEntities.insert_or_assign(mActiveSceneID, engine.EntitiesAlive());
		original_map = engine.mComponentManager->mComponentArrayMap;
		
		for (auto const& [name, components] : original_map)
			cloned_map[name] = components->clone();

		mSceneComponents[mActiveSceneID] = cloned_map;

		{
			mSceneEntitySignatures[mActiveSceneID] = engine.mEntityManager->mSignatures;
			mSceneEntityNames[mActiveSceneID] = engine.mEntityManager->mEntityNames;
			mSceneEntityIds[mActiveSceneID] = engine.mEntityManager->mEntityIds;
		}
	}

	void SceneManager::SaveSceneAs(std::string const& scene_filename)
	{
		mSceneNames[mActiveSceneID] = scene_filename;
		SaveScene();
	}

	void SceneManager::SwitchScene(SceneID scene_id)
	{
		// Nothing to do if scene already active
		if (scene_id == mActiveSceneID)
			return;

		std::string old_scene = mSceneNames[mActiveSceneID];
		// Instance of game engine
		auto& engine = InsightEngine::Instance();
		ECSMap original_map, cloned_map;

		// Create a new scene
		engine.NewScene();

		// Clone ECS
		engine.mComponentManager->mComponentArrayMap = mSceneComponents[scene_id];
		original_map = engine.mComponentManager->mComponentArrayMap;

		for (auto const& [name, components] : original_map)
			cloned_map[name] = components->clone();

		mSceneComponents[scene_id] = cloned_map;
		mActiveSceneID = scene_id;

		// Update engine with scene entities
		engine.mEntityManager->mEntitiesAlive	= mSceneEntities[scene_id];
		engine.mEntityManager->mSignatures		= mSceneEntitySignatures[scene_id];
		engine.mEntityManager->mEntityNames		= mSceneEntityNames[scene_id];
		engine.mEntityManager->mEntityIds		= mSceneEntityIds[scene_id];
		engine.mSystemManager->ClearEntities();

		// Update ECS signatures
		for (auto const& [entity, signature] : mSceneEntitySignatures[scene_id])
			engine.mSystemManager->EntitySignatureChanged(entity, signature);

		IS_CORE_DEBUG("Switch from \"{}\" to scene \"{}\"", old_scene, mSceneNames[scene_id]);
	}

	void SceneManager::RunSceneFunction(std::function<void(SceneID)> SceneFunc)
	{
		for (auto const& [scene_id, scene_name] : mSceneNames)
			SceneFunc(scene_id);
	}

	void SceneManager::RunEntityFunction(SceneID scene_id, std::function<void(Entity, std::string)> EntityFunc)
	{
		for (auto const& [entity, name] : mSceneEntityIds[scene_id])
			EntityFunc(entity, name);
	}

	// Accessors
	const char* SceneManager::GetSceneName(SceneID scene_id) const { return mSceneNames.at(scene_id).c_str(); }
	int SceneManager::GetEntityCount(SceneID scene_id) const { return mSceneEntities.at(scene_id); }
	int SceneManager::GetActiveEntityCount() const { return mSceneEntities.empty() ? 0 : mSceneEntities.at(mActiveSceneID); }
	SceneID SceneManager::GetActiveScene() const { return mActiveSceneID; }
	const char* SceneManager::GetActiveSceneName() const { return mSceneNames.empty() ? "No Scene Loaded" : mSceneNames.at(mActiveSceneID).c_str(); }
	SceneID SceneManager::GetSceneCount() const { return mSceneCount; }

	void SceneManager::AddEntity(const char* name)
	{
		auto& engine = InsightEngine::Instance();
		engine.CreateEntity(name);
		SaveScene();
	}

	void SceneManager::AddRandomEntity()
	{
		auto& engine = InsightEngine::Instance();

		engine.GenerateRandomEntity();
		SaveScene();
	}

	void SceneManager::CloneEntity(Entity entity)
	{
		auto& engine = InsightEngine::Instance();

		engine.CopyEntity(entity);
		SaveScene();
	}

	void SceneManager::DeleteEntity(Entity entity)
	{
		auto& engine = InsightEngine::Instance();

		engine.DeleteEntity(entity);
	}

	void SceneManager::SaveSceneToFile() { InsightEngine::Instance().SaveCurrentScene(mSceneNames[mActiveSceneID]); }

} // end namespace IS
