#include "Pch.h"
#include "Command.h"

namespace IS {

    const std::string DestroyEntityCommand::mTempDirectory = "Temp";
    int DestroyEntityCommand::mDestroyedCount = 0;

    CreateEntityCommand::CreateEntityCommand(std::string const& entity_name) : mEntityName(entity_name)
    {
        mCanMerge = false;
    }

    void CreateEntityCommand::Execute()
    {
        mEntity = InsightEngine::Instance().CreateEntity(mEntityName);
    }

    void CreateEntityCommand::Undo()
    {        
        InsightEngine::Instance().DeleteEntity(mEntity);
    }

    DestroyEntityCommand::DestroyEntityCommand(Entity entity) : mEntity(entity)
    {
        mCanMerge = false;
        mFileName = mTempDirectory + "\\Destroyed Entity " + std::to_string(mDestroyedCount) + ".json";
    }

    void DestroyEntityCommand::Execute()
    {
        if (!std::filesystem::exists(mTempDirectory))
        {
            std::filesystem::create_directory(mTempDirectory);
        }

        auto& engine = InsightEngine::Instance();
        engine.SaveEntityToJson(mEntity, mFileName);
        IS_CORE_DEBUG("Saved {}", mFileName);

        engine.DeleteEntity(mEntity);
        mDestroyedCount++;
    }

    void DestroyEntityCommand::Undo()
    {
        auto& engine = InsightEngine::Instance();
        Entity entity = engine.CreateEntity("Entity");
        mEntity = InsightEngine::Instance().LoadEntityFromJson(mFileName, entity);
        std::filesystem::remove(mFileName);
        mDestroyedCount--;
    }

    void DestroyEntityCommand::ClearTempDirectory()
    {
        if (std::filesystem::exists(mTempDirectory))
        {
            std::filesystem::remove_all(mTempDirectory);
        }
    }

} // end namespace IS
