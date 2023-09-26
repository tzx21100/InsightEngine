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
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
}
namespace IS {
    class ScriptEngine {
    public:
        static void Init();
        static void Shutdown();

        static void LoadAssembly(const std::filesystem::path& file_path);
    private:
        static void InitMono();
        static void ShutdownMono();
        static MonoObject* InstantiateClass(MonoClass* mono_class);

        friend class ScriptClass;
    };

    class ScriptClass
    {
    public:
        ScriptClass() = default;
        ScriptClass(const std::string& class_namespace, const std::string& class_name);
        MonoObject* Instantiate();
        MonoMethod* GetMethod(const std::string& name, int parameter_count);
        MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);


    private:
        std::string mClassNamespace;
        std::string mClassName;
        MonoClass* mMonoClass = nullptr;
    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTENGINE_H  