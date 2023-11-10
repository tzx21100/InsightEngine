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
        Command(std::string const& name) : mName(name) {}
        std::string GetName() const { return mName; }

    private:
        virtual void Execute() = 0;
        virtual void Undo() = 0;

        friend class EditManager;

    protected:
        std::string mName;
    };

    class TranslateCommand : public Command
    {
    public:
        TranslateCommand(Entity entity, Vector2D old_translation, Vector2D new_translation);

    private:
        Entity mEntity;
        Vector2D mOldTranslation;
        Vector2D mNewTranslation;

        void Execute() override;
        void Undo() override;
    };

    class ScaleCommand : public Command
    {
    public:
        ScaleCommand(Entity entity, Vector2D new_scale);
        ScaleCommand(Entity entity, Vector2D, Vector2D new_scale);

    private:
        Entity mEntity;
        Vector2D mOldScale;
        Vector2D mNewScale;

        void Execute() override;
        void Undo() override;
    };

    class RotateCommand : public Command
    {
    public:
        RotateCommand(Entity entity, float, float new_rotation);

    private:
        Entity mEntity;
        float mOldRotation;
        float mNewRotation;

        void Execute() override;
        void Undo() override;
    };

    class CreateEntityCommand : public Command
    {
    public:
        CreateEntityCommand(Entity entity);

    private:
        Entity mEntity;
        std::string mFileName;

        void Execute() override;
        void Undo() override;
    };

    class DestroyEntityComannd : public Command
    {
    public:
        DestroyEntityComannd(Entity entity);

    private:
        Entity mEntity;
        std::string mFileName;

        void Execute() override;
        void Undo() override;
    };

    class EditManager
    {
    public:
        using CommandType = std::shared_ptr<Command>;
        using StackType = std::list<CommandType>;

        void ExecuteCommand(std::shared_ptr<Command> command);
        void Undo();
        void Redo();

    //private:
        const size_t MAX_COMMANDS = 20;
        StackType mUndoStack;
        StackType mRedoStack;

        void LimitStackSize(StackType& stack, size_t max_size);
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_MANAGER_EDIT_MANAGER_H
