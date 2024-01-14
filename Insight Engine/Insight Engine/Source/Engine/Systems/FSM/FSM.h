



#pragma once
#include "../../ECS/System.h"
#include <functional>
#include "../../ECS/Component.h"

/*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_ECS_FSM_H_
#define GAM200_INSIGHT_ENGINE_ECS_FSM_H_


namespace IS {
	/*
	This is a finite state machine class for all future FSM to follow 
	*/
	class SimpleState {

	public:
		std::function<void()> Enter;
		std::function<void()> Update;
		std::function<void()> Exit;

		SimpleState():Enter(),Update(),Exit() {};

		SimpleState(std::function<void()> enter, std::function<void()> update, std::function<void()> exit)
			: Enter(enter), Update(update), Exit(exit) {}

	};
	/*
	This is a class to mark conditions in which a state should change 
	*/
	class ChangeState {

	public:
		void AddCondition(std::function<bool()>);
		void SetTargetState(SimpleState state);
		void SetCurrentState(SimpleState state);
		bool CheckConditionsFufilled();

	private:
		std::vector<std::function<bool()>> mConditionList;
		SimpleState mTargetState;
		SimpleState mCurrentState;
		int mConditionSize;

	};

	/*c
		Checks for state conditions
	*/
	class StateManager : public ParentSystem {
	public:
		/**
		 * \brief Returns the name of the InputManager system.
		 *
		 * \return A string representing the name of this system.
		 */
		std::string GetName() override;

		/**
		 * \brief Initialization logic for the InputManager.
		 *
		 * Currently subscribes to `DebugInfo` messages.
		 */
		void Initialize() override;

		/**
		 * \brief Updates the state of pressed, held, and released keys/buttons.
		 *
		 * Should be called once per frame.
		 */
		void Update(float deltaTime) override;

		/**
		 * \brief Handles incoming messages.
		 *
		 * Defines behavior based on the type of message received.
		 */
		void HandleMessage(const Message& message) override;
		



	};

	class StateComponent : public IComponent {
	public:

		static std::string GetType() { return "StateComponent"; }
		//each entity should hold their own list of change state
		std::vector<ChangeState> mEntityConditions;
		SimpleState mCurrentState;

	};
	
	// helper functions
	SimpleState CreateNewState(std::function<void()> enter, std::function<void()> update, std::function<void()> exit);

}


#endif GAM200_INSIGHT_ENGINE_ECS_COMPONENT_H_
