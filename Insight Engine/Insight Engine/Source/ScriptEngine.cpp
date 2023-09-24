#include "Pch.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace IS {

    namespace Utils {
        //move to asset manager?
        static  char* readBytes(const std::filesystem::path& filepath, uint32_t* out_size)
        {
            std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

            if (!stream)
            {
                // Failed to open the file
                return nullptr;
            }

            std::streampos end = stream.tellg();
            stream.seekg(0, std::ios::beg);
            uint32_t size = static_cast<uint32_t>(end - stream.tellg());

            if (size == 0)
            {
                // File is empty
                return nullptr;
            }

            char* buffer = new char[size];
            stream.read((char*)buffer, size);
            stream.close();

            *out_size = size;
            return buffer;
        }

        static  MonoAssembly* loadMonoAssembly(const std::filesystem::path& assembly_path)
        {
            uint32_t file_size = 0;
            char* file_data = readBytes(assembly_path.c_str(), &file_size);

            // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
            MonoImageOpenStatus status;
            MonoImage* image = mono_image_open_from_data_full(file_data, file_size, 1, &status, 0);

            if (status != MONO_IMAGE_OK)
            {
                const char* error_message = mono_image_strerror(status);
                // Log some error message using the error_message data
                IS_CORE_ERROR("Mono: {}", error_message);
                return nullptr;
            }

            std::string str = assembly_path.string();
            MonoAssembly* assembly = mono_assembly_load_from_full(image, str.c_str(), &status, 0);
            mono_image_close(image);

            // Don't forget to free the file data
            delete[] file_data;

            return assembly;
        }

        void printAssemblyTypes(MonoAssembly* assembly)
        {
            MonoImage* image = mono_assembly_get_image(assembly);
            const MonoTableInfo* type_definitions_table = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
            int32_t num_types = mono_table_info_get_rows(type_definitions_table);

            for (int32_t i = 0; i < num_types; i++)
            {
                uint32_t cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row(type_definitions_table, i, cols, MONO_TYPEDEF_SIZE);

                const char* namespaces = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
                const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

                printf("%s.%s\n", namespaces, name);
            }
        }
    }

    struct ScriptEngineData
    {
        MonoDomain* root_domain = nullptr;
        MonoDomain* app_domain = nullptr;
        MonoAssembly* core_assembly = nullptr;
        MonoImage* core_assembly_img = nullptr;
        ScriptClass entity_class;
    };

    static ScriptEngineData* s_data;

   
    void ScriptEngine::Init()
    {
        s_data = new ScriptEngineData();
        InitMono();
        LoadAssembly("Resources/Scripts/net6.0/IS-ScriptCore.dll");
        ScriptGlue::RegisterFunctions();

        //get and init class
        s_data->entity_class = ScriptClass("IS", "Entity");
        MonoObject* instance = s_data->entity_class.Instantiate();

        //EXAMPLES
        //call func
        MonoMethod* printMessage_func = s_data->entity_class.GetMethod("PrintMessage", 0);
        s_data->entity_class.InvokeMethod(instance, printMessage_func, nullptr);

        //call func w str param
        MonoString* monoString = mono_string_new(s_data->app_domain, "Hello World from C++!");
        MonoMethod* printCustomMessageFunc = s_data->entity_class.GetMethod("PrintCustomMessage", 1);
        void* stringParam = monoString;
        s_data->entity_class.InvokeMethod(instance, printCustomMessageFunc, &stringParam);

        
    }

    void ScriptEngine::Shutdown()
    {
        ShutdownMono();
        delete s_data;
    }


    



    void ScriptEngine::InitMono()
    {
        mono_set_assemblies_path("libraries/mono/lib/4.5");

        MonoDomain* root_domain = mono_jit_init("ISJITRuntime");
        if (root_domain == nullptr)
        {
            // Maybe log some error here
            return;
        }

        // Store the root domain pointer
        s_data->root_domain = root_domain;
        
        
       
    }

    void ScriptEngine::ShutdownMono()
    {
        //fixed domain unload lel
        mono_domain_unload(s_data->app_domain);
        s_data->app_domain = nullptr;

        //need assert - pls close by cmd prompt cuz idk
        mono_jit_cleanup(s_data->root_domain);
        s_data->root_domain = nullptr;

    }

    void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
    {
        // Create an App Domain
        s_data->app_domain = mono_domain_create_appdomain((char*)"ISScriptRuntime", nullptr);
        mono_domain_set(s_data->app_domain, true);

        s_data->core_assembly = Utils::loadMonoAssembly(filepath);
        s_data->core_assembly_img = mono_assembly_get_image(s_data->core_assembly);

        //printAssemblyTypes(s_data->core_assembly);
    }

    MonoObject* ScriptEngine::InstantiateClass(MonoClass* mono_class) {
        MonoObject* instance = mono_object_new(s_data->app_domain, mono_class);
        mono_runtime_object_init(instance);
        return instance;
    }

    ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className)
        : m_ClassNamespace(classNamespace), m_ClassName(className)
    {
        m_MonoClass = mono_class_from_name(s_data->core_assembly_img, classNamespace.c_str(), className.c_str());
    }

    MonoObject* ScriptClass::Instantiate()
    {
        return ScriptEngine::InstantiateClass(m_MonoClass);
    }

    MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
    {
        return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
    }
    MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
    {
        return mono_runtime_invoke(method, instance, params, nullptr);

    }
}