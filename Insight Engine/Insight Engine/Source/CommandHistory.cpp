#include "Pch.h"
#include "CommandHistory.h"

namespace IS {

    CommandHistory::StackType CommandHistory::mUndoStack;
    CommandHistory::StackType CommandHistory::mRedoStack;

    void CommandHistory::AddCommand(CommandType command)
    {
        command->Execute();
        LimitStackSize(mUndoStack, MAX_COMMANDS);
        mRedoStack.clear();
        mUndoStack.push_back(command);
    }

    void CommandHistory::Undo()
    {
        if (mUndoStack.size())
        {
            CommandType command = mUndoStack.back();
            mUndoStack.pop_back();
            command->Undo();
            mRedoStack.push_back(command);
            IS_CORE_DEBUG("Undo undo stack : {}, redo stack : {}", mUndoStack.size(), mRedoStack.size());
        }
    }

    void CommandHistory::Redo()
    {
        if (mRedoStack.size())
        {
            CommandType command = mRedoStack.back();
            mRedoStack.pop_back();
            command->Execute();
            mUndoStack.push_back(command);
            LimitStackSize(mUndoStack, MAX_COMMANDS);
            IS_CORE_DEBUG("Redo undo stack : {}, redo stack : {}", mUndoStack.size(), mRedoStack.size());
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
