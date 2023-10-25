/*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_AIFSM_H
#define GAM200_INSIGHT_ENGINE_SOURCE_AIFSM_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "System.h"
#include "AIState.h"

namespace IS {
    class AIFSMManager: public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string GetName() override;
        void HandleMessage(const Message& message) override;

        AIFSMManager():mCurrentSubstateName("") {}
        ~AIFSMManager() { mStateList.clear(); }

        void AddState(const std::string& state_name, AIState* state);
        void AddSubstate(const std::string& substate_name, const std::string& parent_state_name, AIState* substate);

        void RemoveState(const std::string& state_name);
        void RemoveSubstate(const std::string& substate_name, const std::string& parent_state_name);

        void ChangeState(const std::string& new_state_name);
        void ChangeStateWithCondition(const std::string& new_state_name, bool condition);
        void UpdateCurrentState();

        void ChangeSubstate(const std::string& substate_name, const std::string& parent_state_name);
        void ChangeSubstateWithCondition(const std::string& substate_name, const std::string& parent_state_name, bool condition);
        void UpdateCurrentSubstate(const std::string& parent_state_name);

        AIState* GetCurrentState();
        AIState* GetCurrentSubstate(const std::string& parent_state_name);

        AIState* GetState(const std::string& state_name);
        AIState* GetSubstate(const std::string& substate_name, const std::string& parent_state_name);

        //allow for AI to preserve memory of states/actions
        void PushState(const std::string& state_name, const std::string& parent_state_name = "");
        void PopState(const std::string& state_name);

    private:
        //store states
        std::unordered_map<std::string, AIState*> mStateList{};
        AIState* mCurrentState{};
        AIState* mCurrentSubstate{};
        //std::string mCurrentStateName;
        std::string mCurrentSubstateName;

        //stack to allow memory of previous states (easy pushing and going back to states)
        std::stack<AIState*> mStateStack{};
    };
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_AIFSM_H  