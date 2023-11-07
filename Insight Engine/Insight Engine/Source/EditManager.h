#ifndef GAM200_INSIGHT_ENGINE_EDITOR_MANAGER_EDIT_MANAGER_H
#define GAM200_INSIGHT_ENGINE_EDITOR_MANAGER_EDIT_MANAGER_H

#include "Entities.h"
#include "ISMath.h"

#include <string>
#include <list>
#include <memory>

namespace IS {

    class Command
    {
    public:
        virtual void Execute() = 0;
        virtual void Undo() = 0;
    };

    class TranslateCommand : public Command
    {
    public:
        TranslateCommand(Entity entity, Vector2D new_translation) : mEntity(entity), mNewTranslation(new_translation)
        {
            auto& engine = InsightEngine::Instance();
            auto& transform = engine.GetComponent<Transform>(entity);
            mOldTranslation = transform.getWorldPosition();
        }

        void Execute() override
        {
            auto& engine = InsightEngine::Instance();
            auto& transform = engine.GetComponent<Transform>(mEntity);
            transform.setWorldPosition(mNewTranslation.x, mNewTranslation.y);
        }

        void Undo() override
        {
            auto& engine = InsightEngine::Instance();
            auto& transform = engine.GetComponent<Transform>(mEntity);
            transform.setWorldPosition(mOldTranslation.x, mOldTranslation.y);
        }

    private:
        Entity mEntity;
        Vector2D mOldTranslation;
        Vector2D mNewTranslation;
    };

    class ScaleCommand : public Command
    {
    public:
        ScaleCommand(Entity entity, Vector2D new_scale) : mEntity(entity), mNewScale(new_scale)
        {
            auto& engine = InsightEngine::Instance();
            auto& transform = engine.GetComponent<Transform>(entity);
            mOldScale = transform.getScaling();
        }

        void Execute() override
        {
            auto& engine = InsightEngine::Instance();
            auto& transform = engine.GetComponent<Transform>(mEntity);
            transform.setScaling(mNewScale.x, mNewScale.y);
        }

        void Undo() override
        {
            auto& engine = InsightEngine::Instance();
            auto& transform = engine.GetComponent<Transform>(mEntity);
            transform.setScaling(mOldScale.x, mOldScale.y);
        }

    private:
        Entity mEntity;
        Vector2D mOldScale;
        Vector2D mNewScale;
    };

    class RotateCommand : public Command
    {
    public:
        RotateCommand(Entity entity, float new_rotation) : mEntity(entity), mNewRotation(new_rotation)
        {
            auto& engine = InsightEngine::Instance();
            auto& transform = engine.GetComponent<Transform>(entity);
            mOldRotation = transform.getRotation();
        }

        void Execute() override
        {
            auto& engine = InsightEngine::Instance();
            auto& transform = engine.GetComponent<Transform>(mEntity);
            transform.setRotation(mNewRotation);
        }

        void Undo() override
        {
            auto& engine = InsightEngine::Instance();
            auto& transform = engine.GetComponent<Transform>(mEntity);
            transform.setRotation(mOldRotation);
        }

    private:
        Entity mEntity;
        float mOldRotation;
        float mNewRotation;
    };

    class CreateEntityCommand : public Command
    {
    public:
        CreateEntityCommand(Entity entity) : mEntity(entity) {}

        void Execute() override
        {
            auto& engine = InsightEngine::Instance();
        }

        void Undo() override
        {
            auto& engine = InsightEngine::Instance();
            engine.DeleteEntity(mEntity);
        }

    private:
        Entity mEntity;
    };

    class DestroyEntityComannd : public Command
    {
    public:
        DestroyEntityComannd(Entity entity) : mEntity(entity) {}

        void Execute() override
        {
            auto& engine = InsightEngine::Instance();
            engine.DeleteEntity(mEntity);
        }

        void Undo() override
        {
            auto& engine = InsightEngine::Instance();
        }

    private:
        Entity mEntity;
    };

    class EditManager
    {
    public:
        void ExecuteCommand(std::shared_ptr<Command> command);
        void Undo();
        void Redo();

    private:
        std::list<std::shared_ptr<Command>> mUndoStack;
        std::list<std::shared_ptr<Command>> mRedoStack;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_MANAGER_EDIT_MANAGER_H
