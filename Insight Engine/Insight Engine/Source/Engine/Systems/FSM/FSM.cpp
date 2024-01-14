#pragma once
#include "Pch.h"
#include "FSM.h"
#include "../../Core/CoreEngine.h"

namespace IS {

	void ChangeState::AddCondition(std::function<bool()> func) {

		mConditionList.emplace_back(func);
		mConditionSize++;

	}

	bool ChangeState::CheckConditionsFufilled() {
		int size=0;
		for (int i = 0; i < mConditionSize; i++) {
			if (mConditionList[i]() == true) {
				size++;
			}
		}
		if (size == mConditionSize) {
			return true;
		}
		return false;
	}

	void ChangeState::SetTargetState(SimpleState state) {
		mTargetState = state;
	}

	void ChangeState::SetCurrentState(SimpleState state) {
		mCurrentState = state;
	}

	SimpleState ChangeState::GetTargetState() {
		return mTargetState;
	}

	std::string StateManager::GetName()  {
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
			for (auto& statchange : state_component.mEntityConditions) {
				if (statchange.CheckConditionsFufilled()) { state_component.mCurrentState = statchange.GetTargetState(); }
			}
		}
	}

	void StateManager::HandleMessage(const Message& message) {
		if (message.GetType() == MessageType::DebugInfo) {
			IS_CORE_DEBUG("State Manager Running");
		}
	}

	// The following are helper functions to create States

	SimpleState CreateNewState(std::function<void()> enter, std::function<void()> update, std::function<void()> exit) {
		return SimpleState(enter, update, exit);
	}
}
