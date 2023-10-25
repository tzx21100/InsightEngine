 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"

namespace IS {

    std::string AIFSMManager::GetName() {
        return "AIFSM";
    }

    void AIFSMManager::Initialize() {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);
    }

    void AIFSMManager::Update([[maybe_unused]] float deltaTime) {
        //AIFSMManager aiFSM;

        //AIState idleState;
        //idleState.Enter = []() { std::cout << "Entering Idle State" << std::endl; };
        //idleState.Update = []() { std::cout << "Updating Idle State" << std::endl; };
        //idleState.Exit = []() { std::cout << "Exiting Idle State" << std::endl; };

        ////AIState patrolState;
        ////patrolState.Enter = []() { std::cout << "Entering Patrol State" << std::endl; };
        ////patrolState.Update = []() { std::cout << "Updating Patrol State" << std::endl; };
        ////patrolState.Exit = []() { std::cout << "Exiting Patrol State" << std::endl; };

        ////AIState chaseState;
        ////chaseState.Enter = []() { std::cout << "Entering Chase State" << std::endl; };
        ////chaseState.Update = []() { std::cout << "Updating Chase State" << std::endl; };
        ////chaseState.Exit = []() { std::cout << "Exiting Chase State" << std::endl; };

        ////// Add main states to the AI FSM.
        //aiFSM.AddState("Idle", &idleState);
        ////aiFSM.AddState("Patrol", &patrolState);
        ////aiFSM.AddState("Chase", &chaseState);

        ////// Change and update AI states.
        //aiFSM.ChangeState("Idle");
        //aiFSM.UpdateCurrentState(); // Updates the Idle state

        ////aiFSM.ChangeState("Patrol");
        ////aiFSM.UpdateCurrentState(); // Updates the Patrol state

        ////aiFSM.ChangeState("Chase");
        ////aiFSM.UpdateCurrentState(); // Updates the Chase state

        ////// Retrieve and print the current state.
        ////AIState* currentState = aiFSM.GetCurrentState();
        ////if (currentState != nullptr) {
        ////    std::cout << "Current State: " << typeid(*currentState).name() << std::endl;
        ////}
        ////else {
        ////    std::cout << "No Current State" << std::endl;
        ////}

        ////// Add substates.
        //AIState jumpState;
        //jumpState.Enter = []() { std::cout << "Entering Jump State" << std::endl; };
        //jumpState.Update = []() { std::cout << "Updating Jump State" << std::endl; };
        //jumpState.Exit = []() { std::cout << "Exiting Jump State" << std::endl; };

        //aiFSM.AddSubstate("Jump", "Idle", &jumpState);

        ////// Change and update substates.
        //aiFSM.ChangeSubstate("Jump", "Idle");
        //aiFSM.UpdateCurrentSubstate("Idle"); // Updates the Jump substate within the Idle state

        ////// Retrieve and print the current substate.
        ////AIState* currentSubstate = aiFSM.GetCurrentSubstate("Idle");
        ////if (currentSubstate != nullptr) {
        ////    std::cout << "Current Substate: " << typeid(*currentSubstate).name() << std::endl;
        ////}
        ////else {
        ////    std::cout << "No Current Substate" << std::endl;
        ////}

        ////// Remove substates.
        //aiFSM.RemoveSubstate("Jump", "Idle");
        //aiFSM.RemoveState("Idle");
    }

    void AIFSMManager::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            //IS_CORE_INFO("Handling Debug");
        }
    }

    void AIFSMManager::AddState(const std::string& state_name, AIState* state) {
        mStateList[state_name] = state;
    }

    void AIFSMManager::AddSubstate(const std::string& substate_name, const std::string& parent_state_name, AIState* substate) {
        AIState* parent_state = GetState(parent_state_name);
        if (parent_state != nullptr) {
            parent_state->substates[substate_name] = substate;
            mCurrentSubstateName = substate_name;
        }
    }

    void AIFSMManager::RemoveState(const std::string& state_name) {
        auto iter = mStateList.find(state_name);
        if (iter != mStateList.end()) {
            //delete iter->second;
            mStateList.erase(iter);
        }
    }

    void AIFSMManager::RemoveSubstate(const std::string& substate_name, const std::string& parent_state_name) {
        AIState* parent_state = GetState(parent_state_name);
        if (parent_state != nullptr) {
            auto it = parent_state->substates.find(substate_name);
            if (it != parent_state->substates.end()) {
                parent_state->substates.erase(it);
                mCurrentSubstateName = "";

            }
        }
    }

    void AIFSMManager::ChangeState(const std::string& new_state_name) {
        AIState* state = GetState(new_state_name);
        if (state != nullptr) {
            if (mCurrentState != nullptr) {
                if (mCurrentState->Exit) {
                    mCurrentState->Exit();
                }
            }
            mCurrentState = state;
            if (mCurrentState->Enter) {
                mCurrentState->Enter();
            }
        }
    }

    void AIFSMManager::ChangeStateWithCondition(const std::string& new_state_name, bool condition) {
        if (condition) {
            ChangeState(new_state_name);
        }
    }

    void AIFSMManager::UpdateCurrentState() {
        if (mCurrentState != nullptr && mCurrentState->Update) {
            mCurrentState->Update();
        }
    }

    void AIFSMManager::ChangeSubstate(const std::string& substate_name, const std::string& parent_state_name) {
        AIState* parent_state = GetState(parent_state_name);
        AIState* new_sub_state = GetSubstate(substate_name, parent_state_name);

        if (parent_state != nullptr && new_sub_state != nullptr) {
            if (mCurrentSubstate != nullptr && mCurrentSubstate != new_sub_state) {
                if (mCurrentSubstate->Exit) {
                    mCurrentSubstate->Exit();
                }
            }
            
            if (parent_state->Exit) {
                parent_state->Exit();
            }

            parent_state->substates[substate_name] = new_sub_state;
            mCurrentSubstateName = substate_name;
            mCurrentSubstate = new_sub_state;

            if (new_sub_state->Enter) {
                new_sub_state->Enter();
            }
        }
    }

    void AIFSMManager::ChangeSubstateWithCondition(const std::string& substate_name, const std::string& parent_state_name, bool condition) {
        AIState* parent_state = GetState(parent_state_name);
        if (parent_state != nullptr && condition) {
            ChangeSubstate(substate_name, parent_state_name);
        }
    }

    void AIFSMManager::UpdateCurrentSubstate(const std::string& parent_state_name) {
        AIState* parent_state = GetState(parent_state_name);
        if (parent_state != nullptr) {
            AIState* currentSubstate = parent_state->substates[mCurrentSubstateName];
            if (currentSubstate != nullptr && currentSubstate->Update) {
                currentSubstate->Update(); // Update the current substate
                
            }
            
        }
    }

    AIState* AIFSMManager::GetCurrentState() {
        return mCurrentState;
    }

    AIState* AIFSMManager::GetCurrentSubstate(const std::string& parent_state_name)
    {
        AIState* parent_state = GetState(parent_state_name);
        if (parent_state != nullptr) {
            return parent_state->substates[mCurrentSubstateName];
        }
        return nullptr;
    }

    AIState* AIFSMManager::GetState(const std::string& state_name) {
        auto iter = mStateList.find(state_name);
        if (iter != mStateList.end()) {
            return iter->second;
        }
        return nullptr;
    }

    AIState* AIFSMManager::GetSubstate(const std::string& substate_name, const std::string& parent_state_name) {
        AIState* parent_state = GetState(parent_state_name);
        if (parent_state != nullptr) {
            auto it = parent_state->substates.find(substate_name);
            if (it != parent_state->substates.end()) {
                return it->second;
            }
        }
        return nullptr;
    }

    void AIFSMManager::PushState(const std::string& state_name, const std::string& parent_state_name) {
        AIState* new_state = GetState(state_name);
        if (new_state != nullptr) {
            if (mCurrentState != nullptr) {
                if (mCurrentState->Exit) {
                    mCurrentState->Exit();
                }

                // Check if the new_state has substates, and push the current substate (if any) to the stack.
                if (!parent_state_name.empty() && mCurrentState != new_state) {
                    AIState* current_substate = mCurrentState->substates[mCurrentSubstateName];
                    if (current_substate != nullptr) {
                        mStateStack.push(current_substate);
                    }
                }
            }

            mCurrentState = new_state;

            if (mCurrentState->Enter) {
                mCurrentState->Enter();
            }
        }
    }

    void AIFSMManager::PopState(const std::string& state_name) {
        if (!mStateStack.empty()) {
            if (mCurrentState != nullptr && mCurrentState->Exit) {
                mCurrentState->Exit();
            }

            // Check if the current state has a parent state with substates and restore the substate from the stack.
            AIState* parent_state = GetState(state_name);
            if (parent_state != nullptr) {
                mCurrentState = parent_state->substates[mCurrentSubstateName];
            }

            else {
                // If no substate to restore, pop from the stack.
                mCurrentState = mStateStack.top();
                mStateStack.pop();
            }

            if (mCurrentState->Enter) {
                mCurrentState->Enter();
            }

        }
    }
}