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

        //// Define AI states as function pointers.
        //AIState patrolState;
        //patrolState.Enter = []() { std::cout << "Entering Patrol State" << std::endl; };
        //patrolState.Update = []() { std::cout << "Updating Patrol State" << std::endl; };
        //patrolState.Exit = []() { std::cout << "Exiting Patrol State" << std::endl; };

        //AIState chaseState;
        //chaseState.Enter = []() { std::cout << "Entering Chase State" << std::endl; };
        //chaseState.Update = []() { std::cout << "Updating Chase State" << std::endl; };
        //chaseState.Exit = []() { std::cout << "Exiting Chase State" << std::endl; };

        //// Create and add AI states.
        //aiFSM.AddState("Patrol", &patrolState);
        //aiFSM.AddState("Chase", &chaseState);

        //// Change and update AI states.
        //aiFSM.ChangeState("Patrol");
        //aiFSM.UpdateCurrentState();

        ///*aiFSM.ChangeState("Chase");
        //aiFSM.UpdateCurrentState();*/

        //// Retrieve and print the current state.
        //AIState* currentState = aiFSM.GetCurrentState();
        //if (currentState != nullptr) {
        //    std::cout << "Current State: " << currentState << std::endl;
        //}
        //else {
        //    std::cout << "No Current State" << std::endl;
        //}

        ////// Retrieve and print a specific state.
        ////AIState* patrolState = aiFSM.GetState("Patrol");
        ////if (patrolState != nullptr) {
        ////    std::cout << "Patrol State: " << typeid(*patrolState).name() << std::endl;
        ////}
        ////else {
        ////    std::cout << "Patrol State not found" << std::endl;
        ////}

        //// Remove AI states and clean up.
        //aiFSM.RemoveState("Patrol");
        ////aiFSM.RemoveState("Chase");
    }

    void AIFSMManager::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            //IS_CORE_INFO("Handling Debug");
        }
    }

    void AIFSMManager::AddState(const std::string& state_name, AIState* state){
        mStateList[state_name] = state;
    }

    void AIFSMManager::RemoveState(const std::string& state_name){
        auto iter = mStateList.find(state_name);
        if (iter != mStateList.end()) {
            //delete iter->second;
            mStateList.erase(iter);
        }
    }

    void AIFSMManager::ChangeState(const std::string& new_state_name){
        auto iter = mStateList.find(new_state_name);
        if (iter != mStateList.end()) {
            if (mCurrentState != nullptr) {
                if (mCurrentState->Exit) {
                    mCurrentState->Exit();
                }
            }
            mCurrentState = iter->second;
            if (mCurrentState->Enter) {
                mCurrentState->Enter();
            }
        }
    }

    void AIFSMManager::UpdateCurrentState(){
        if (mCurrentState != nullptr && mCurrentState->Update) {
            mCurrentState->Update();
        }
    }

    AIState* AIFSMManager::GetCurrentState() {
        return mCurrentState;
    }

    AIState* AIFSMManager::GetState(const std::string& state_name) {
        auto iter = mStateList.find(state_name);
        if (iter != mStateList.end()) {
            return iter->second;
        }
        return nullptr;
    }


}