/*!
 * \file Command.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 20-11-2023
 * \brief
 * This source file defines the implementation for class ICommmand and its
 * derived classes, which encapsulates the functionalities of a command in
 * a command pattern undo redo system.
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
