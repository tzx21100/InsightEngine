/* Start Header **************************************************************/
/*!
 * \file AIFSM.h
 * \author Matthew Ng, matthewdeen.ng@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
 * \brief
 * Declaration of the AIFSMManager class for managing AI states.
 *
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 */
 /* End Header ****************************************************************/
 /*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_AIFSM_H
#define GAM200_INSIGHT_ENGINE_SOURCE_AIFSM_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Engine/ECS/System.h"
#include "AIState.h"

namespace IS {
    /*!
     * \brief The AIFSMManager class manages AI states.
     *
     * The AIFSMManager class provides function to add, remove, change and store states.
     * NOTE: Substates are a little buggy, so its wip.
     */
    class AIFSMManager: public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string GetName() override;
        void HandleMessage(const Message& message) override;

        /*!
         * \brief Default constructor for the AIFSMManager class.
         * 
         * Initializes current substate name to null
         */
        AIFSMManager(): mCurrentSubstateName("") {}

        /*!
         * \brief Default destructor for the AIFSMManager class.
         * 
         * Clears states from list
         */
        ~AIFSMManager() { mStateList.clear(); }

        /*!
         * @brief Add a new state.
         *
         * @param state_name The name of the state to be added.
         * @param state The AIState object representing the state.
         */
        void AddState(const std::string& state_name, AIState* state);

        /*!
         * @brief Add a substate to an existing parent state.
         *
         * @param substate_name The name of the substate to be added.
         * @param parent_state_name The name of the parent state.
         * @param substate The AIState object representing the substate.
         */
        void AddSubstate(const std::string& substate_name, const std::string& parent_state_name, AIState* substate);

        /**
         * @brief Remove a state.
         *
         * @param state_name The name of the state to be removed.
         */
        void RemoveState(const std::string& state_name);

        /*!
         * @brief Remove a substate from an existing parent state.
         *
         * @param substate_name The name of the substate to be removed.
         * @param parent_state_name The name of the parent state.
         */
        void RemoveSubstate(const std::string& substate_name, const std::string& parent_state_name);

        /*!
         * @brief Change the current state to a new state.
         *
         * @param new_state_name The name of the new state.
         */
        void ChangeState(const std::string& new_state_name);

        /*!
         * @brief Change the current state to a new state based on a condition.
         *
         * @param new_state_name The name of the new state.
         * @param condition The condition that determines the state change.
         */
        void ChangeStateWithCondition(const std::string& new_state_name, bool condition);

        /*!
         * @brief Update the current state
         */
        void UpdateCurrentState();

        /*!
         * @brief Change the current substate within a parent state.
         *
         * @param substate_name The name of the substate to be activated.
         * @param parent_state_name The name of the parent state.
         */
        void ChangeSubstate(const std::string& substate_name, const std::string& parent_state_name);

        /*!
         * @brief Change the current substate within a parent state based on a condition.
         *
         * @param substate_name The name of the substate to be activated.
         * @param parent_state_name The name of the parent state.
         * @param condition The condition that determines the substate change.
         */
        void ChangeSubstateWithCondition(const std::string& substate_name, const std::string& parent_state_name, bool condition);

        /*!
         * @brief Update the current substate within a parent state in the AI controller.
         *
         * @param parent_state_name The name of the parent state.
         */
        void UpdateCurrentSubstate(const std::string& parent_state_name);

        /*!
         * @brief Get the current state.
         *
         * @return A pointer to the current AIState object representing the state.
         */
        AIState* GetCurrentState();

        /*!
         * @brief Get the current substate within a parent state.
         *
         * @param parent_state_name The name of the parent state.
         * @return A pointer to the current AIState object representing the substate.
         */
        AIState* GetCurrentSubstate(const std::string& parent_state_name);

        /*!
         * @brief Get a state by name.
         *
         * @param state_name The name of the state to retrieve.
         * @return A pointer to the AIState object representing the state.
         */
        AIState* GetState(const std::string& state_name);

        /*!
         * @brief Get a substate by name within a parent state.
         *
         * @param substate_name The name of the substate to retrieve.
         * @param parent_state_name The name of the parent state.
         * @return A pointer to the AIState object representing the substate.
         */
        AIState* GetSubstate(const std::string& substate_name, const std::string& parent_state_name);


        /*!
         * @brief Push a new state onto the state stack, preserving the previous state. (not used/ wip)
         *
         * @param state_name The name of the state to push onto the stack.
         * @param parent_state_name (Optional) The name of the parent state if it is a substate.
         */
        void PushState(const std::string& state_name, const std::string& parent_state_name = "");

        /**
         * @brief Pop a state from the state stack, restoring the previous state. (not used/ wip)
         *
         * @param state_name The name of the state to pop from the stack.
         */
        void PopState(const std::string& state_name);

    private:
        //store states
        std::unordered_map<std::string, AIState*> mStateList{};

        AIState* mCurrentState{};
        AIState* mCurrentSubstate{};

        std::string mCurrentSubstateName;

        //stack to allow memory of previous states (easy pushing and going back to states)
        std::stack<AIState*> mStateStack{};
    };
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_AIFSM_H  