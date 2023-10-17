/* Start Header **************************************************************/
/*!
\file    ScriptGlue.cpp
\author  Matthew Ng, matthewdeen.ng@digipen.edu
\par     Course: CSD2401
\date    27-09-2023
\brief   This source file defines the implementation of the ScriptGlue class, which
         registers C++ functions to be accessible from C# scripts. It includes
         functions for logging messages and vectors.

\copyright
All content (C) 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*____________________________________________________________________________*/
/* End Header ****************************************************************/

/* includes */
#include "Pch.h"
#include <mono/metadata/object.h>

namespace IS {
    // Macro to add internal calls for C# access
#define IS_ADD_INTERNAL_CALL(Name) mono_add_internal_call("IS.InternalCalls::" #Name, Name)

    /**
     * \brief Native function to log a message with an integer parameter.
     * \param name The message to log.
     * \param param The integer parameter to log.
     */
    static void NativeLog(MonoString* name, int param) {
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str); // Convert char* to C++ string
        mono_free(c_str); // Free the allocated char*
        std::cout << str << "," << param << std::endl;
    }

    /**
     * \brief Native function to log a glm::vec3 vector.
     * \param param The glm::vec3 vector to log.
     */
    static void NativeLogVector(glm::vec3* param) {
        auto values = *param; // Get values from the vector
        std::cout << values.x << "," << values.y << "," << values.z << std::endl;
    }


    
    // registering input functions

    static bool KeyPressed(int keycode) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsKeyPressed(keycode);
    }

    static bool KeyHeld(int keycode) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsKeyHeld(keycode);
    }

    static bool KeyReleased(int keycode) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsKeyReleased(keycode);
    }

    static bool MousePressed(int mousebutton) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsMouseButtonPressed(mousebutton);
    }

    static bool MouseHeld(int mousebutton) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsMouseButtonHeld(mousebutton);
    }

    static bool MouseReleased(int mousebutton) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsMouseButtonReleased(mousebutton);
    }

    /**
     * \brief Registers C++ functions to be accessible from C# scripts.
     */
    void ScriptGlue::RegisterFunctions()
    {
        IS_ADD_INTERNAL_CALL(NativeLog);
        IS_ADD_INTERNAL_CALL(NativeLogVector);

        //input functions
        IS_ADD_INTERNAL_CALL(KeyPressed);
        IS_ADD_INTERNAL_CALL(KeyHeld);
        IS_ADD_INTERNAL_CALL(KeyReleased);
        IS_ADD_INTERNAL_CALL(MousePressed);
        IS_ADD_INTERNAL_CALL(MouseHeld);
        IS_ADD_INTERNAL_CALL(MouseReleased);

    }
}