#pragma once
#include "Pch.h"
#include "FSM.h"

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





}