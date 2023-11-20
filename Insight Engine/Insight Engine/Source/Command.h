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
        virtual bool Merge(std::shared_ptr<ICommand>) { return false; }

        void SetNoMerge() { mCanMerge = false; }
        bool CanMerge() const { return mCanMerge; }

    protected:
        bool mCanMerge = true;
    };

    template <typename T>
    class ChangeCommand final : public ICommand
    {
    public:
        ChangeCommand(T& value, T new_value) : mValue(value), mNewValue(new_value), mOldValue(value) {}

        void Execute() override
        {
            mValue = mNewValue;
        }

        void Undo() override
        {
            mValue = mOldValue;
        }

        bool Merge(std::shared_ptr<ICommand> other) override
        {
            std::shared_ptr<ChangeCommand<T>> command = std::dynamic_pointer_cast<ChangeCommand<T>>(other);
            if (command)
            {
                if (command->mValue != mValue)
                {
                    command->mNewValue = mNewValue;
                    return true;
                }
            }

            return false;
        }

    private:
        T& mValue;
        T mNewValue;
        T mOldValue;
    };

    class CreateEntityCommand final : public ICommand
    {
    public:
        CreateEntityCommand(std::string const& entity_name);

        void Execute() override;
        void Undo() override;
        Entity mEntity;

    private:
        std::string mEntityName;
    };

    class DestroyEntityCommand final : public ICommand
    {
    public:
        DestroyEntityCommand(Entity entity);

        void Execute() override;
        void Undo() override;

        static void ClearTempDirectory();

    private:
        Entity mEntity;
        std::string mFileName;
        static const std::string mTempDirectory;
        static int mDestroyedCount;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_H
