#pragma once
#include "../../ECS/System.h"
#include "../../ECS/Component.h"
#include <functional>
#include "../../Scripting/ScriptGlue.h"
#include "../../Scripting/ScriptEngine.h"

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

		//Adding script to simple state
		void SetSimpleState(std::string script_name) {
			mScriptName = script_name;
			mScriptClass = ScriptClass("IS",mScriptName);
			mMonoObject = mScriptClass.Instantiate();
			if (mMonoObject != nullptr) {
				mScriptClass.LoadMethods();
				MonoMethod* init_method =mScriptClass.GetMethod("Init", 0);
				mScriptClass.InvokeMethod(mMonoObject, init_method, nullptr);
				mHasScript = true;
			}
		}


		void Enter() 
		{
			MonoMethod* update_method = mScriptClass.GetMethod("Init", 0);
			mScriptClass.InvokeMethod(mMonoObject, update_method, nullptr);
		};
		void Update() 
		{
			if (!mHasScript && mScriptName != "") {
				SetSimpleState(mScriptName);
			}

			if (mHasScript) {
				MonoMethod* update_method = mScriptClass.GetMethod("Update", 0);
				mScriptClass.InvokeMethod(mMonoObject, update_method, nullptr);
			}
		};
		void Exit() 
		{
			MonoMethod* update_method = mScriptClass.GetMethod("CleanUp", 0);
			mScriptClass.InvokeMethod(mMonoObject, update_method, nullptr);
		};

		std::string mScriptName = "";
		ScriptClass mScriptClass;
		MonoObject* mMonoObject = nullptr;
		bool mHasScript =false;
		bool mConditionRecieved = false;

	};

	// functions to create SimpleState
	SimpleState CreateSimpleState(std::string script_name);

	/*
	This is a class to mark conditions in which a state should change 
	*/
	class ChangeState {

	public:
		void AddCondition(std::string script_name);
		void SetTargetState(SimpleState state);
		void SetCurrentState(SimpleState state);
		SimpleState GetCondition();
		SimpleState GetTargetState();
		SimpleState GetCurrentState();
		bool CheckConditionsFufilled();

	private:
		SimpleState mConditionScript;
		SimpleState mTargetState;
		SimpleState mCurrentState;

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
		

		void ActivateScripts();

	};

	class StateComponent : public IComponent {
	public:

		static std::string GetType() { return "StateComponent"; }
		//each entity should hold their own list of change state
		ChangeState mEntityConditions;
		SimpleState mCurrentState;

	};
	



}


#endif GAM200_INSIGHT_ENGINE_ECS_COMPONENT_H_
