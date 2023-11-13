#ifndef GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_H
#define GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_H

#include "ISMath.h"
#include "Entities.h"

#include <string>

namespace IS {

    class ICommand
    {
    public:
        virtual ~ICommand() = default;
        virtual void Execute() = 0;
        virtual void Undo() = 0;
    };

    class Vec2Command final : public ICommand
    {
    public:
        Vec2Command(Vec2& value, Vec2 new_value)
            : mValue(value), mNewValue(new_value), mOldValue(value) {}

        void Execute() override
        {
            mValue = mNewValue;
            IS_CORE_DEBUG("Change from ({:.2f}, {:.2f}) to ({:.2f}, {:.2f})", mOldValue.x, mOldValue.y, mValue.x, mValue.y);
        }

        void Undo() override 
        {
            mValue = mOldValue;
            IS_CORE_DEBUG("Change from ({:.2f}, {:.2f}) to ({:.2f}, {:.2f})", mNewValue.x, mNewValue.y, mValue.x, mValue.y);
        }

    private:
        Vec2& mValue;
        Vec2 mNewValue;
        Vec2 mOldValue;
    };

    class FloatCommand final : public ICommand
    {
    public:
        FloatCommand(float& value, float new_value)
            : mValue(value), mNewValue(new_value), mOldValue(value) {}

        void Execute() override
        {
            mValue = mNewValue;
            IS_CORE_DEBUG("Change from {:.2f} to {:.2f}", mOldValue, mValue);
        }

        void Undo() override 
        {
            mValue = mOldValue;
            IS_CORE_DEBUG("Change from {:.2f} to {:.2f}", mNewValue, mValue);
        }

    private:
        float& mValue;
        float mOldValue;
        float mNewValue;
    };

    class CreateEntityCommand final : public ICommand
    {
    public:
        CreateEntityCommand(Entity entity);

        void Execute() override {}
        void Undo() override {}

    private:
        Entity mEntity;
        std::string mFileName;
    };

    class DestroyEntityComannd final : public ICommand
    {
    public:
        DestroyEntityComannd(Entity entity);

        void Execute() override {}
        void Undo() override {}

    private:
        Entity mEntity;
        std::string mFileName;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_H
