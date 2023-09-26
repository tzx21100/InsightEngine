/* Start Header **************************************************************/
/*!
\file	ScriptEngine.h
\author Matthew

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header ****************************************************************/

#ifndef GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTENGINE_H
#define GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTENGINE_H

extern "C" {
    //type forwarding
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
}

namespace IS {

    class ScriptEngine {
    public:
        static void Init(); //acts as a ctor
        static void Shutdown(); //acts as a dtor

        static void LoadAssembly(const std::filesystem::path& file_path); //loads c# dll file

    private:
        static void InitMono(); //sets root domain
        static void ShutdownMono(); //frees app&root doain
        static MonoObject* InstantiateClass(MonoClass* mono_class); //helps set the instance to mEntityClass

        friend class ScriptClass;
    };

    class ScriptClass
    {
    public:
        ScriptClass() = default;
        ScriptClass(const std::string& class_namespace, const std::string& class_name); //sets member var
        MonoObject* Instantiate(); //calls InstantiateClass
        MonoMethod* GetMethod(const std::string& name, int parameter_count); //returns c# method if exists
        MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr); //calls c# code if exists


    private:
        std::string mClassNamespace;
        std::string mClassName;
        MonoClass* mMonoClass = nullptr;
    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTENGINE_H  