#include "Pch.h"
#include "EditManager.h"

namespace IS {

    void EditManager::ExecuteCommand(std::shared_ptr<Command> command)
    {
        command->Execute();
        mUndoStack.push_back(command);
        mRedoStack.clear();
    }

    void EditManager::Undo()
    {
        if (mUndoStack.size())
        {
            std::shared_ptr<Command> command = mUndoStack.back();
            mUndoStack.pop_back();
            command->Undo();
            mRedoStack.push_back(command);
        }
    }

    void EditManager::Redo()
    {
        if (mRedoStack.size())
        {
            std::shared_ptr<Command> command = mRedoStack.back();
            mRedoStack.pop_back();
            command->Execute();
            mUndoStack.push_back(command);
        }
    }

} // end namespace IS
