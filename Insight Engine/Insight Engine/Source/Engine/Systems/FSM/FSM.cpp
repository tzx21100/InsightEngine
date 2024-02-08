/*!
* \file FSM.cpp
* \author  Tan Zheng Xun, t.zhengxun@digipen.edu
* \par Course: CSD2401
* \date  08-02-2024
* \brief
* This header file defines the FSM functions to create scripts and attach
* them to the fsm class. This helps developers create any sort of FSM
* they wish to use during development. 
*
* \copyright
* All content (C) 2024 DigiPen Institute of Technology Singapore.
* All rights reserved.
* Reproduction or disclosure of this file or its contents without the prior written
* consent of DigiPen Institute of Technology is prohibited.
*____________________________________________________________________________*/



/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "FSM.h"
#include "../../Core/CoreEngine.h"


namespace IS {

	SimpleState CreateSimpleState(std::string script_name) {
		SimpleState new_state;
		new_state.SetSimpleState(script_name);
		return new_state;
	}


	void ChangeState::AddCondition(std::string script_name) {
		mConditionScript = CreateSimpleState(script_name);
	}

	bool ChangeState::CheckConditionsFufilled() {
		mConditionScript.Update();
		bool state = RecievedScriptCondition;
		RecievedScriptCondition = false;
		return state;
	}

	void ChangeState::SetTargetState(SimpleState state) {
		mTargetState = state;
	}

	void ChangeState::SetCurrentState(SimpleState state) {
		mCurrentState = state;
	}

	SimpleState ChangeState::GetCondition()
	{
		return mConditionScript;
	}

	SimpleState ChangeState::GetTargetState()
	{
		return mTargetState;
	}

	SimpleState ChangeState::GetCurrentState()
	{
		return mCurrentState;
	}

	std::string StateManager::GetName()
	{
		return "StateManager";
	}

	void StateManager::Initialize() {
		for (auto& entity : mEntities) {
			auto& state_component=InsightEngine::Instance().GetComponent<StateComponent>(entity);
			state_component.mCurrentState.Enter();
		}
	}

	void StateManager::Update(float) {
		for (auto& entity : mEntities) {
			auto& state_component = InsightEngine::Instance().GetComponent<StateComponent>(entity);
			state_component.mCurrentState.Update();

			if (state_component.mEntityConditions.CheckConditionsFufilled()) { 
				state_component.mCurrentState = state_component.mEntityConditions.GetTargetState(); 
			}
			
		}
	}

	void StateManager::HandleMessage(const Message& message) {
		if (message.GetType() == MessageType::DebugInfo) {
			IS_CORE_DEBUG("State Manager Running");
		}
	}

	void StateManager::ActivateScripts() {
		
	}

}
