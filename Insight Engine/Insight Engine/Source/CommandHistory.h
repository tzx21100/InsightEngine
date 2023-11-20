#ifndef GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_HISTORY_H
#define GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_HISTORY_H

#include "Command.h"

#include <memory>
#include <list>

namespace IS {

    class CommandHistory
    {
    public:
        using CommandType = std::shared_ptr<ICommand>;
        using StackType = std::list<CommandType>;

        static void AddCommand(CommandType command);
        static void Undo();
        static void Redo();
        static void SetNotExplicitBack(bool changed);

    private:
        static constexpr size_t MAX_COMMANDS = 20;
        static StackType mUndoStack;
        static StackType mRedoStack;

        static void LimitStackSize(StackType& stack, size_t max_size);
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_MANAGER_EDIT_MANAGER_H
