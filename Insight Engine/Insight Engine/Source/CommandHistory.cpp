#include "Pch.h"
#include "CommandHistory.h"

namespace IS {

    CommandHistory::StackType CommandHistory::mUndoStack;
    CommandHistory::StackType CommandHistory::mRedoStack;

    void CommandHistory::AddCommand(CommandType command)
    {
        // Execute the command
        command->Execute();

        // Limit stack size
        LimitStackSize(mUndoStack, MAX_COMMANDS);

        // Clear the redo stack
        mRedoStack.clear();

        // Add to the undo stack
        mUndoStack.push_back(command);

        // Check if more there is enough commands to merge
        if (mUndoStack.size() > 1)
        {
            // Check if the last and second last command can be merged
            if (CommandType second_last_command = *(std::prev(mUndoStack.end(), 2));
                mUndoStack.back()->CanMerge() && second_last_command->CanMerge())
            {
                // Merge the last and second last command
                if (mUndoStack.back()->Merge(second_last_command))
                {
                    // Remove the last command
                    mUndoStack.pop_back();
                }
            }
        }
    }

    void CommandHistory::Undo()
    {
        // Check if undo stack is not empty
        if (mUndoStack.size())
        {
            // Undo the last command
            CommandType command = mUndoStack.back();
            mUndoStack.pop_back();
            command->Undo();
            mRedoStack.push_back(command);
            IS_CORE_DEBUG("Undo undo stack : {}, redo stack : {}", mUndoStack.size(), mRedoStack.size());
        }
    }

    void CommandHistory::Redo()
    {
        // Check if redo stack is not empty
        if (mRedoStack.size())
        {
            // Redo the last command
            CommandType command = mRedoStack.back();
            mRedoStack.pop_back();
            command->Execute();
            mUndoStack.push_back(command);
            LimitStackSize(mUndoStack, MAX_COMMANDS);
            IS_CORE_DEBUG("Redo undo stack : {}, redo stack : {}", mUndoStack.size(), mRedoStack.size());
        }
    }

    void CommandHistory::SetNoMergeMostRecent(bool changed)
    {
        if (!changed)
            return;

        if (mUndoStack.size() - 1 >= 0)
        {
            mUndoStack.back()->SetNoMerge();
        }
    }

    void CommandHistory::LimitStackSize(StackType& stack, size_t max_size)
    {
        while (stack.size() > max_size)
        {
            stack.pop_front();
        }
    }

} // end namespace IS
