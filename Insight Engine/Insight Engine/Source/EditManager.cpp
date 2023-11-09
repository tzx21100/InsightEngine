#include "Pch.h"
#include "EditManager.h"

namespace IS {

    TranslateCommand::TranslateCommand(Entity entity, Vector2D old_translation, Vector2D new_translation) : Command("Translate"), mEntity(entity), mNewTranslation(new_translation)
    {
        mOldTranslation = old_translation;
    }

    void TranslateCommand::Execute()
    {
        auto& engine = InsightEngine::Instance();
        auto& transform = engine.GetComponent<Transform>(mEntity);
        transform.setWorldPosition(mNewTranslation.x, mNewTranslation.y);
    }

    void TranslateCommand::Undo()
    {
        auto& engine = InsightEngine::Instance();
        auto& transform = engine.GetComponent<Transform>(mEntity);
        transform.setWorldPosition(mOldTranslation.x, mOldTranslation.y);
    }

    ScaleCommand::ScaleCommand(Entity entity, Vector2D new_scale) : Command("Scale"), mEntity(entity), mNewScale(new_scale)
    {
        auto& engine = InsightEngine::Instance();
        auto& transform = engine.GetComponent<Transform>(entity);
        mOldScale = transform.getScaling();
    }

    void ScaleCommand::Execute()
    {
        auto& engine = InsightEngine::Instance();
        auto& transform = engine.GetComponent<Transform>(mEntity);
        transform.setScaling(mNewScale.x, mNewScale.y);
    }

    void ScaleCommand::Undo()
    {
        auto& engine = InsightEngine::Instance();
        auto& transform = engine.GetComponent<Transform>(mEntity);
        transform.setScaling(mOldScale.x, mOldScale.y);
    }

    RotateCommand::RotateCommand(Entity entity, float new_rotation) : Command("Rotate"), mEntity(entity), mNewRotation(new_rotation)
    {
        auto& engine = InsightEngine::Instance();
        auto& transform = engine.GetComponent<Transform>(entity);
        mOldRotation = transform.getRotation();
    }

    void RotateCommand::Execute()
    {
        auto& engine = InsightEngine::Instance();
        auto& transform = engine.GetComponent<Transform>(mEntity);
        transform.setRotation(mNewRotation);
    }

    void RotateCommand::Undo()
    {
        auto& engine = InsightEngine::Instance();
        auto& transform = engine.GetComponent<Transform>(mEntity);
        transform.setRotation(mOldRotation);
    }

    CreateEntityCommand::CreateEntityCommand(Entity entity) : Command("Create Entity"), mEntity(entity) {}

    void CreateEntityCommand::Execute()
    {
        //auto& engine = InsightEngine::Instance();
    }

    void CreateEntityCommand::Undo()
    {
        auto& engine = InsightEngine::Instance();
        engine.DeleteEntity(mEntity);
    }

    DestroyEntityComannd::DestroyEntityComannd(Entity entity) : Command("Destroy Entity"), mEntity(entity) {}

    void DestroyEntityComannd::Execute()
    {
        auto& engine = InsightEngine::Instance();
        engine.DeleteEntity(mEntity);
    }

    void DestroyEntityComannd::Undo()
    {
        //auto& engine = InsightEngine::Instance();
    }

    void EditManager::ExecuteCommand(std::shared_ptr<Command> command)
    {
        command->Execute();
        LimitStackSize(mUndoStack, MAX_COMMANDS);
        mRedoStack.clear();
        mUndoStack.push_back(command);
    }

    void EditManager::Undo()
    {
        if (mUndoStack.size())
        {
            CommandType command = mUndoStack.back();
            mUndoStack.pop_back();
            command->Undo();
            mRedoStack.push_back(command);
        }
    }

    void EditManager::Redo()
    {
        if (mRedoStack.size())
        {
            CommandType command = mRedoStack.back();
            mRedoStack.pop_back();
            command->Execute();
            mUndoStack.push_back(command);
            LimitStackSize(mUndoStack, MAX_COMMANDS);
        }
    }

    void EditManager::LimitStackSize(StackType& stack, size_t max_size)
    {
        while (stack.size() >= max_size)
        {
            stack.pop_front();
        }
    }

} // end namespace IS
