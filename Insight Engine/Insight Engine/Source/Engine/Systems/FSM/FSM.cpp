#pragma once
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
