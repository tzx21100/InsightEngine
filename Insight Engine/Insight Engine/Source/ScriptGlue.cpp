/* Start Header **************************************************************/
/*!
\file	ScriptGlue.cpp
\author Matthew

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header ****************************************************************/

#include "Pch.h"

#include <mono/metadata/object.h>

namespace IS {
#define IS_ADD_INTERNAL_CALL(Name) mono_add_internal_call("IS.InternalCalls::" #Name, Name)

    static void NativeLog(MonoString* name, int param) {
        char* c_str = mono_string_to_utf8(name);
        std::string str(c_str);
        mono_free(c_str);
        std::cout << str << "," << param << std::endl;
    }

    static void NativeLogVector(glm::vec3* param) {
        auto values = *param;
        std::cout << values.x << "," << values.y << "," << values.z << std::endl;
    }

    void ScriptGlue::RegisterFunctions()
    {
        IS_ADD_INTERNAL_CALL(NativeLog);
        IS_ADD_INTERNAL_CALL(NativeLogVector);
    }
}