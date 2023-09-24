#include "Pch.h"

#include <mono/metadata/object.h>

namespace IS {
#define IS_ADD_INTERNAL_CALL(Name) mono_add_internal_call("IS.InternalCalls::" #Name, Name)

    static void nativeLog(MonoString* name, int param) {
        char* cstr = mono_string_to_utf8(name);
        std::string str(cstr);
        mono_free(cstr);
        std::cout << str << "," << param << std::endl;
    }

    static void nativeLog_v(glm::vec3* param) {
        auto values = *param;
        std::cout << values.x << "," << values.y << "," << values.z << std::endl;
    }

    void ScriptGlue::RegisterFunctions()
    {
        IS_ADD_INTERNAL_CALL(nativeLog);
        IS_ADD_INTERNAL_CALL(nativeLog_v);

        /*mono_add_internal_call("IS.InternalCalls::nativeLog", nativeLog);
        mono_add_internal_call("IS.InternalCalls::nativeLog_v", nativeLog_v);*/
    }
}