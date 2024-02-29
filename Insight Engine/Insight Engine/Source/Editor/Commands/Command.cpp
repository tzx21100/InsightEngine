/*!
 * \file Command.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 20-11-2023
 * \brief
 * This source file defines the implementation for class ICommmand and its
 * derived classes, which encapsulates the functionalities of a command in
 * a command pattern undo redo system.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#ifdef USING_IMGUI

#include "Command.h"
#include "Graphics/System/Sprite.h"
#include "Graphics/System/Transform.h"
#include "Engine/Scripting/ScriptManager.h"
#include "Engine/Systems/FSM/FSM.h"

namespace IS {

    const std::string ICommand::mTempDirectory = "Temp";
    int DestroyEntityCommand::mDestroyedCount = 0;
    int PrefabCommand::mPrefabCount = 0;
    int DuplicateCommand::mDupeCount = 0;

    void ICommand::ValidateTempDirectory()
    {
        if (!std::filesystem::exists(mTempDirectory))
        {
            std::filesystem::create_directory(mTempDirectory);
        }
    }

    void ICommand::ClearTempDirectory()
    {
        if (std::filesystem::exists(mTempDirectory))
        {
            std::filesystem::remove_all(mTempDirectory);
        }
    }

    CreateEntityCommand::CreateEntityCommand(std::string const& entity_name) : mEntityName(entity_name)
    {
        mCanMerge = false;
    }

    void CreateEntityCommand::Execute()
    {
        auto& engine = InsightEngine::Instance();
        auto const editor_layer = engine.GetEditorLayer();
        mEntity = engine.CreateEntity(mEntityName);
        editor_layer->SetSelectedEntity(std::make_shared<Entity>(mEntity));
    }

    void CreateEntityCommand::Undo()
    {
        auto& engine = InsightEngine::Instance();
        auto const editor_layer = engine.GetEditorLayer();
        editor_layer->ResetEntitySelection();
        engine.DeleteEntity(mEntity);
    }

    DestroyEntityCommand::DestroyEntityCommand(Entity entity) : mEntity(entity)
    {
        mCanMerge = false;
        mFileName = mTempDirectory + "\\Destroyed Entity " + std::to_string(mDestroyedCount) + ".json";
    }

    void DestroyEntityCommand::Execute()
    {
        ICommand::ValidateTempDirectory();

        auto& engine = InsightEngine::Instance();
        auto const editor_layer = engine.GetEditorLayer();
        editor_layer->ResetEntitySelection();
        engine.SaveEntityToJson(mEntity, mFileName);
        engine.DeleteEntity(mEntity);
        mDestroyedCount++;
    }

    void DestroyEntityCommand::Undo()
    {
        auto& engine = InsightEngine::Instance();
        auto const editor_layer = engine.GetEditorLayer();
        Entity entity = engine.CreateEntity("Entity");
        mEntity = engine.LoadEntityFromJson(mFileName, entity);
        editor_layer->SetSelectedEntity(std::make_shared<Entity>(mEntity));
        std::filesystem::remove(mFileName);
        mDestroyedCount--;
    }

    PrefabCommand::PrefabCommand(std::string const& prefab_name) : mPrefabName(prefab_name), mFirstTime(true)
    {
        mCanMerge = false;
        mFileName = mTempDirectory + "\\Prefab Entity " + std::to_string(mPrefabCount) + ".json";
    }

    void PrefabCommand::Execute()
    {
        auto& engine = InsightEngine::Instance();

        // Load from prefab and save to JSON
        if (mFirstTime)
        {
            auto const asset = engine.GetSystem<AssetManager>("Asset");
            Entity prefab = engine.CreateEntity("Prefab");
            mEntity = engine.LoadFromPrefab(asset->GetPrefab(mPrefabName), prefab);
            if (engine.HasComponent<Transform>(prefab))
            {
                Transform& transform = engine.GetComponent<Transform>(prefab);
                transform.world_position = { static_cast<float>(Transform::GetMousePosition().first),
                                             static_cast<float>(Transform::GetMousePosition().second) };
            }
            ValidateTempDirectory();
            engine.SaveEntityToJson(mEntity, mFileName);
        }
        // Load from JSON
        else
        {
            Entity json = engine.CreateEntity("JSON");
            mEntity = engine.LoadEntityFromJson(mFileName, json);
        }

        auto const editor_layer = engine.GetEditorLayer();
        editor_layer->SetSelectedEntity(std::make_shared<Entity>(mEntity));
        mFirstTime = false;
        mPrefabCount++;
    }

    void PrefabCommand::Undo()
    {
        auto& engine = InsightEngine::Instance();
        auto const editor_layer = engine.GetEditorLayer();
        editor_layer->ResetEntitySelection();
        engine.DeleteEntity(mEntity);
        mPrefabCount--;
    }

    TextureCommand::TextureCommand(std::string const& filename) : PrefabCommand(filename) {}

    void TextureCommand::Execute()
    {
        auto& engine = InsightEngine::Instance();
        if (std::string stem = std::filesystem::path(mPrefabName).stem().string(); mFirstTime)
        {                        
            mEntity = engine.CreateEntity(stem);
            engine.AddComponent<Sprite>(mEntity, Sprite());
            engine.AddComponent<Transform>(mEntity, Transform());
            auto& sprite = engine.GetComponent<Sprite>(mEntity);
            auto& transform = engine.GetComponent<Transform>(mEntity);
            auto const asset = engine.GetSystem<AssetManager>("Asset");
            sprite.img = *asset->GetImage(mPrefabName);

            transform.scaling = { static_cast<float>(sprite.img.width), static_cast<float>(sprite.img.height) };
            transform.world_position = { static_cast<float>(Transform::GetMousePosition().first),
                                         static_cast<float>(Transform::GetMousePosition().second) };

            ValidateTempDirectory();
            engine.SaveEntityToJson(mEntity, mFileName);
        }
        else
        {
            Entity entity = engine.CreateEntity(stem);
            mEntity = engine.LoadEntityFromJson(mFileName, entity);
        }

        auto const editor_layer = engine.GetEditorLayer();
        editor_layer->SetSelectedEntity(std::make_shared<Entity>(mEntity));
        mFirstTime = false;
        mPrefabCount++;
    }

    DuplicateCommand::DuplicateCommand(Entity original) : mOriginal(original), mFirstTime(true)
    {
        mCanMerge = false;
        mFileName = mTempDirectory + "\\Duplicated Entity " + std::to_string(mDupeCount) + ".json";
    }

    void DuplicateCommand::Execute()
    {
        auto& engine = InsightEngine::Instance();

        if (mFirstTime)
        {
            mDupe = engine.CopyEntity(mOriginal);

            ValidateTempDirectory();
            engine.SaveEntityToJson(mDupe, mFileName);
        }
        else
        {
            Entity dupe = engine.CreateEntity("Dupe");
            mDupe = engine.LoadEntityFromJson(mFileName, dupe);
        }

        auto const editor_layer = engine.GetEditorLayer();
        editor_layer->SetSelectedEntity(std::make_shared<Entity>(mDupe));

        mFirstTime = false;
        mDupeCount++;
    }

    void DuplicateCommand::Undo()
    {
        auto& engine = InsightEngine::Instance();
        auto const editor_layer = engine.GetEditorLayer();
        editor_layer->ResetEntitySelection();
        engine.DeleteEntity(mDupe);
    }

    FunctionCommand::FunctionCommand(Func const& execute, Func const& undo) : mExecute(execute), mUndo(undo) { mCanMerge = false; }

    void FunctionCommand::Execute() { mExecute(); }

    void FunctionCommand::Undo() { mUndo(); }

    AddScriptComponentCommand::AddScriptComponentCommand(Entity entity, std::string const& script) : mEntity(entity), mScriptName(script)
    {
        mCanMerge = false;
    }

    void AddScriptComponentCommand::Execute()
    {
        auto& engine = InsightEngine::Instance();
        engine.AddComponent<ScriptComponent>(mEntity, ScriptComponent());
        auto& script = engine.GetComponent<ScriptComponent>(mEntity);
        script.mScriptName = mScriptName;
    }

    void AddScriptComponentCommand::Undo()
    {
        auto& engine = InsightEngine::Instance();
        engine.RemoveComponent<ScriptComponent>(mEntity);
    }

    AddStateComponentCommand::AddStateComponentCommand(Entity entity, std::vector<std::string> const& state_scripts) : mEntity(entity), mStateScripts(state_scripts)
    {
        mCanMerge = false;
    }

    void AddStateComponentCommand::Execute()
    {
        auto& engine = InsightEngine::Instance();
        engine.AddComponent<StateComponent>(mEntity, StateComponent());

        auto& state = engine.GetComponent<StateComponent>(mEntity);
        state.mCurrentState.SetSimpleState(mStateScripts[0]);
        state.mEntityConditions.AddCondition(mStateScripts[1]);
        SimpleState temp = CreateSimpleState(mStateScripts[2]);
        state.mEntityConditions.SetCurrentState(temp);
        temp = CreateSimpleState(mStateScripts[3]);
        state.mEntityConditions.SetTargetState(temp);
    }

    void AddStateComponentCommand::Undo()
    {
        auto& engine = InsightEngine::Instance();
        engine.RemoveComponent<StateComponent>(mEntity);
    }

} // end namespace IS

#endif // USING_IMGUI
