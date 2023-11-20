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
        virtual bool Merge(ICommand*) = 0;

        void SetMerge() { mCanMerge = false; }
        bool CanMerge() const { return mCanMerge; }

    protected:
        bool mCanMerge = true;
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

        bool Merge(ICommand* other) override
        {
            Vec2Command* command = dynamic_cast<Vec2Command*>(other);
            if (command)
            {
                if (&command->mValue == &mValue)
                {
                    command->mNewValue = mNewValue;
                    return true;
                }
            }

            return false;
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

        bool Merge(ICommand* other) override
        {
            FloatCommand* command = dynamic_cast<FloatCommand*>(other);
            if (command)
            {
                if (&command->mValue == &mValue)
                {
                    command->mNewValue = mNewValue;
                    return true;
                }
            }

            return false;
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
        bool Merge(ICommand*) override {}

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
        bool Merge(ICommand*) override {}

    private:
        Entity mEntity;
        std::string mFileName;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_H
