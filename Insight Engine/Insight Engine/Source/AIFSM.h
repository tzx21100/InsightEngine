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

        AIFSMManager() {}
        ~AIFSMManager() { mStateList.clear(); }

        void AddState(const std::string& state_name, AIState* state);
        void RemoveState(const std::string& state_name);
        void ChangeState(const std::string& new_state_name);
        void UpdateCurrentState();

        AIState* GetCurrentState();
        AIState* GetState(const std::string& name);

    private:
        //store states
        std::unordered_map<std::string, AIState*> mStateList{};
        AIState* mCurrentState{};
    };
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_AIFSM_H  