/*!
 * \file CommandHistory.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 20-11-2023
 * \brief
 * This header file declares the interface for class CommandHistory
 * which encapsulates the functionalities of a command history in
 * a command pattern undo redo system.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_HISTORY_H
#define GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_HISTORY_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Command.h"

#include <memory>
#include <list>

namespace IS {

    /*!
     * \class CommandHistory
     * \brief A class for managing the history of executed commands.
     */
    class CommandHistory
    {
    public:
        using CommandType = std::shared_ptr<ICommand>; ///< Type alias for shared pointers to ICommand objects.
        using StackType = std::list<CommandType>; ///< Type alias for a stack of ICommand objects.

        /*!
         * \brief Add a command to the command history.
         * \param command The command to be added.
         */
        static void AddCommand(CommandType command);

        /*!
         * \brief Add a command to the command history.
         * \tparam CommandT The type of ICommand objects to be added.
         * \tparam Args The types of additional parameters for the command constructor.
         * \param args Additional parameters for the command constructor (variadic parameter).
         */
        template <typename CommandT, typename... Args>
        static void AddCommand(Args&&... args)
        {
            CommandType command = std::make_shared<CommandT>(std::forward<Args>(args)...);

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

        /*!
         * \brief Undo the most recent command in the history.
         */
        static void Undo();

        /*!
         * \brief Redo the previously undone command.
         */
        static void Redo();

        /*!
         * \brief Set whether the most recent command can be merged with subsequent commands.
         * \param changed True if the most recent command should not be mergeable, false otherwise.
         */
        static void SetNoMergeMostRecent(bool changed);

        static void Clear();

    private:
        static constexpr size_t MAX_COMMANDS = 50; ///< The maximum number of commands to retain in the history.
        static StackType mUndoStack; ///< The stack for undoing commands.
        static StackType mRedoStack; ///< The stack for redoing undone commands.

        /*!
         * \brief Limit the size of the given stack to the specified maximum size.
         * \param stack The stack to be limited in size.
         * \param max_size The maximum size to be enforced for the stack.
         */
        static void LimitStackSize(StackType& stack, size_t max_size);
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_MANAGER_EDIT_MANAGER_H
