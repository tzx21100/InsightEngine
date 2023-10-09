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

    }

    void AIFSMManager::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            //IS_CORE_INFO("Handling Debug");
        }
    }

    /*void AIFSMManager::AddState(const std::string& state_name, AIState* state){
        mStateList[state_name] = state;
    }

    void AIFSMManager::RemoveState(const std::string& state_name){
        auto iter = mStateList.find(state_name);
        if (iter != mStateList.end()) {
            delete iter->second;
            mStateList.erase(iter);
        }
    }

    void AIFSMManager::ChangeState(const std::string& new_state_name){
        auto iter = mStateList.find(new_state_name);
        if (iter != mStateList.end()) {
            if (mCurrentState != nullptr) {
                mCurrentState->Exit();
            }
            mCurrentState = iter->second;
            mCurrentState->Enter();
        }
    }

    void AIFSMManager::UpdateCurrentState(){
        if (mCurrentState != nullptr) {
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
    }*/


}