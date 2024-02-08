/*!
* \file FSM.h
* \author  Tan Zheng Xun, t.zhengxun@digipen.edu
* \par Course: CSD2401
* \date  08-02-2024
* \brief
* This header file declares the FSM system integrated with scripts.
*
* \copyright
* All content (C) 2023 DigiPen Institute of Technology Singapore.
* All rights reserved.
* Reproduction or disclosure of this file or its contents without the prior written
* consent of DigiPen Institute of Technology is prohibited.
*____________________________________________________________________________*/
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

		/**
		 * \brief
		 *
		 * Sets the script that the fsm is currently running
		 * 
		 * \param std::string name of the script
		 */
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



		/**
		 * \brief State enter
		 *
		 * Runs the init function of the attached script.
		 *
		 */
		void Enter() 
		{
			MonoMethod* update_method = mScriptClass.GetMethod("Init", 0);
			mScriptClass.InvokeMethod(mMonoObject, update_method, nullptr);
		};

		/**
		 * \brief State update
		 *
		 * Runs the update function of the attached script.
		 *
		 */
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

		/**
		 * \brief State exit
		 *
		 * Runs the cleanup function of the attached script.
		 *
		 */
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
		/**
		 * \brief
		 *
		 * Adds a script as a condition for the FSM
		 *
		 * \param std::string name of the script
		 */
		void AddCondition(std::string script_name);

		/**
		 * \brief
		 *
		 * Sets the script that the entity will run if the Condition Script is fufilled
		 *
		 * \param std::string name of the script
		 */
		void SetTargetState(SimpleState state);

		/**
		 * \brief
		 *
		 * Sets the current state with a state of a  simple script that should contain the
		 * current running state. This is used for the Condition script to check and is not what is
		 * currently running. 
		 *
		 * \param std::string name of the script
		 */
		void SetCurrentState(SimpleState state);

		/**
		 * \brief
		 *
		 * Returns the current condition
		 *
		 */
		SimpleState GetCondition();

		/**
		 * \brief
		 *
		 *  Gets the targeted state 
		 */
		SimpleState GetTargetState();

		/**
		 * \brief
		 *
		 * Returns the current state
		 *
		 */
		SimpleState GetCurrentState();

		/**
		 * \brief
		 *
		 * Returns a boolean to check if the condition script has been fulfilled.
		 *
		 */
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
		


		/**
		 * \brief This fuction actiavates all the scripts for running.
		 */
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
