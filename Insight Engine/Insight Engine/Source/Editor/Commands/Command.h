/*!
 * \file Command.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 20-11-2023
 * \brief
 * This header file declares the interface for class ICommmand and its
 * derived classes, which encapsulates the functionalities of a command in
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
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_H
#define GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Math/ISMath.h"
#include "Engine/ECS/Entities.h"
#include "Engine/Core/CoreEngine.h"

#include <string>

namespace IS {

    /*!
     * \class ICommand
     * \brief Interface for command objects.
     */
    class ICommand
    {
    public:
        static const std::string mTempDirectory; ///< The temporary directory for entity destruction.

        /*!
         * \brief Virtual destructor for ICommand.
         */
        virtual ~ICommand() = default;

        /*!
         * \brief Execute the command.
         */
        virtual void Execute() = 0;

        /*!
         * \brief Undo the executed command.
         */
        virtual void Undo() = 0;

        /*!
         * \brief Attempt to merge with another command.
         * \param other A shared pointer to another ICommand object.
         * \return True if the merge is successful, false otherwise.
         */
        virtual bool Merge(std::shared_ptr<ICommand> other) { return false; }

        /*!
         * \brief Set the command as non-mergeable.
         */
        void SetNoMerge() { mCanMerge = false; }

        /*!
         * \brief Check if the command can be merged.
         * \return True if the command can be merged, false otherwise.
         */
        bool CanMerge() const { return mCanMerge; }

        static void ValidateTempDirectory();

        /*!
         * \brief Clear the temporary directory used for entity destruction.
         */
        static void ClearTempDirectory();

    protected:
        bool mCanMerge = true; ///< Flag indicating whether the command can be merged.
    };

    /*!
     * \class ChangeCommand
     * \brief A command that changes the value of a given type.
     * \tparam T The type of the value to be changed.
     */
    template <typename T>
    class ChangeCommand final : public ICommand
    {
    public:
        /*!
         * \brief Constructor for ChangeCommand.
         * \param value Reference to the original value.
         * \param new_value The new value to set.
         */
        ChangeCommand(T& value, T new_value) : mValue(value), mNewValue(new_value), mOldValue(value) {}

        /*!
         * \brief Execute the change by setting the new value.
         */
        void Execute() override { mValue = mNewValue; }

        /*!
         * \brief Undo the change by restoring the original value.
         */
        void Undo() override { mValue = mOldValue; }

        /*!
         * \brief Attempt to merge with another ChangeCommand.
         * \param other A shared pointer to another ICommand object.
         * \return True if the merge is successful, false otherwise.
         */
        bool Merge(std::shared_ptr<ICommand> other) override
        {
            std::shared_ptr<ChangeCommand<T>> command = std::dynamic_pointer_cast<ChangeCommand<T>>(other);
            if (command)
            {
                if (command->mNewValue != mNewValue)
                {
                    command->mNewValue = mNewValue;
                    return true;
                }
            }

            return false;
        }

    private:
        T& mValue;   ///< Reference to the original value.
        T mNewValue; ///< The new value to set.
        T mOldValue; ///< The original value.
    };

    /*!
     * \class CreateEntityCommand
     * \brief A command for creating an entity.
     */
    class CreateEntityCommand final : public ICommand
    {
    public:
        /*!
         * \brief Constructor for CreateEntityCommand.
         * \param entity_name The name of the entity to be created.
         */
        CreateEntityCommand(std::string const& entity_name);

        /*!
         * \brief Execute the command by creating the entity.
         */
        void Execute() override;

        /*!
         * \brief Undo the command by destroying the created entity.
         */
        void Undo() override;

        Entity GetEntity() const { return mEntity; }

    private:
        Entity mEntity{};         ///< The created entity.
        std::string mEntityName{}; ///< The name of the entity to be created.
    };

    /*!
     * \class DestroyEntityCommand
     * \brief A command for destroying an entity.
     */
    class DestroyEntityCommand final : public ICommand
    {
    public:
        /*!
         * \brief Constructor for DestroyEntityCommand.
         * \param entity The entity to be destroyed.
         */
        DestroyEntityCommand(Entity entity);

        /*!
         * \brief Execute the command by destroying the entity.
         */
        void Execute() override;

        /*!
         * \brief Undo the command by recreating the destroyed entity.
         */
        void Undo() override;

    private:
        Entity mEntity;               ///< The entity to be destroyed.
        std::string mFileName;        ///< The filename associated with the destroyed entity.
        static int mDestroyedCount;    ///< Count of destroyed entities.
    };

    /*!
     * \class PrefabCommand
     * \brief Represents a command for handling prefabs.
     */
    class PrefabCommand : public ICommand
    {
    public:
        /*!
         * \brief Constructor for PrefabCommand.
         * \param prefab_name The name of the prefab.
         */
        PrefabCommand(std::string const& prefab_name);

        /*!
         * \brief Executes the prefab command.
         */
        virtual void Execute() override;

        /*!
         * \brief Undoes the prefab command.
         */
        void Undo() override;

    protected:
        Entity mEntity;             ///< The entity associated with the prefab.
        std::string mPrefabName;    ///< The name of the prefab.
        std::string mFileName;      ///< The file name associated with the prefab.
        bool mFirstTime;            ///< Flag indicating if it's the first time executing the command.
        static int mPrefabCount;    ///< Static counter for tracking the number of prefab commands.
    };

    /*!
     * \class TextureCommand
     * \brief Represents a command for handling textures, inheriting from PrefabCommand.
     */
    class TextureCommand final : public PrefabCommand
    {
    public:
        /*!
         * \brief Constructor for TextureCommand.
         * \param texture_filename The filename of the texture.
         */
        TextureCommand(std::string const& texture_filename);

        /*!
         * \brief Executes the texture command.
         */
        void Execute() override;
    };

    /*!
     * \class DuplicateCommand
     * \brief Represents a command for duplicating entities.
     */
    class DuplicateCommand final : public ICommand
    {
    public:
        /*!
         * \brief Constructor for DuplicateCommand.
         * \param original The original entity to duplicate.
         */
        DuplicateCommand(Entity original);

        /*!
         * \brief Executes the duplicate command.
         */
        void Execute() override;

        /*!
         * \brief Undoes the duplicate command.
         */
        void Undo() override;

        /*!
         * \brief Gets the duplicated entity.
         * \return The duplicated entity.
         */
        Entity GetDupe() const { return mDupe; }

    private:
        Entity mOriginal;           ///< The original entity to duplicate.
        Entity mDupe;               ///< The duplicated entity.
        std::string mFileName;      ///< The file name associated with the duplication.
        bool mFirstTime;            ///< Flag indicating if it's the first time executing the command.
        static int mDupeCount;      ///< Static counter for tracking the number of duplicate commands.
    };

    /*!
     * \class AddComponentCommand
     * \brief Represents a command for adding a component to an entity.
     * \tparam Component The type of component to add.
     */
    template <typename Component>
    class AddComponentCommand final : public ICommand
    {
    public:
        /*!
         * \brief Constructor for AddComponentCommand.
         * \param entity The entity to which the component will be added.
         */
        AddComponentCommand(Entity entity) : mEntity(entity)
        {
            mCanMerge = false;
        }

        /*!
         * \brief Executes the add component command.
         */

        void Execute() override
        {
            auto& engine = InsightEngine::Instance();
            engine.AddComponent<Component>(mEntity, Component());
        }

        /*!
         * \brief Undoes the add component command.
         */
        void Undo() override
        {
            auto& engine = InsightEngine::Instance();
            engine.RemoveComponent<Component>(mEntity);
        }

    private:
        Entity mEntity;             ///< The entity to which the component will be added.
    };

    static int mRemoveCount = 0; ///< The counter for remove component command.

    /*!
     * \class RemoveComponentCommand
     * \brief Represents a command for removing a component from an entity.
     * \tparam Component The type of component to remove.
     */
    template <typename Component>
    class RemoveComponentCommand final : public ICommand
    {
    public:
        /*!
         * \brief Constructor for RemoveComponentCommand.
         * \param entity The entity from which the component will be removed.
         */
        RemoveComponentCommand(Entity entity) : mEntity(entity)
        {
            mCanMerge = false;
            mFileName = mTempDirectory + "\\Remove Component " + std::to_string(mRemoveCount) + ".json";
        }

        /*!
         * \brief Executes the remove component command.
         */
        void Execute() override
        {
            ValidateTempDirectory();

            auto& engine = InsightEngine::Instance();
            engine.SaveEntityToJson(mEntity, mFileName);
            IS_CORE_DEBUG("{}", mFileName);
            engine.RemoveComponent<Component>(mEntity);
            mRemoveCount++;
        }

        /*!
         * \brief Undoes the remove component command.
         */
        void Undo() override
        {
            auto& engine = InsightEngine::Instance();
            mEntity = engine.LoadEntityFromJson(mFileName, mEntity);
            std::filesystem::remove(mFileName);
            IS_CORE_DEBUG("{}", mFileName);
            mRemoveCount--;
        }

    private:
        Entity mEntity;             ///< The entity from which the component will be removed.
        std::string mFileName;      ///< The file name associated with the removal.
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_COMMANDS_COMMAND_H
