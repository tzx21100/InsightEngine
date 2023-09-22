#include "Pch.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace IS {

    struct ScriptEngineData
    {
        MonoDomain* root_domain = nullptr;
        MonoDomain* app_domain = nullptr;
        MonoAssembly* core_assembly = nullptr;
    };

    static ScriptEngineData* s_data;

    void ScriptEngine::Init()
    {
        s_data = new ScriptEngineData();
        InitMono();
    }

    void ScriptEngine::Shutdown()
    {
        delete s_data;
    }

    char* readBytes(const std::string& filepath, uint32_t* out_size)
    {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        if (!stream)
        {
            // Failed to open the file
            return nullptr;
        }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        uint32_t size = end - stream.tellg();

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

    MonoAssembly* loadCSharpAssembly(const std::string& assembly_path)
    {
        uint32_t file_size = 0;
        char* file_data = readBytes(assembly_path, &file_size);

        // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(file_data, file_size, 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            const char* error_message = mono_image_strerror(status);
            // Log some error message using the error_message data
            return nullptr;
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, assembly_path.c_str(), &status, 0);
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

        // Create an App Domain
        s_data->app_domain = mono_domain_create_appdomain((char*)"ISScriptRuntime", nullptr);
        mono_domain_set(s_data->app_domain, true);

        //temp
        s_data->core_assembly = loadCSharpAssembly("Resources/Scripts/net6.0/IS-ScriptCore.dll");
        printAssemblyTypes(s_data->core_assembly);

        MonoImage* assembly_image = mono_assembly_get_image(s_data->core_assembly);
        MonoClass* mono_class = mono_class_from_name(assembly_image, "IS", "Main");

        //create an obj
        MonoObject* instance = mono_object_new(s_data->app_domain, mono_class);
        mono_runtime_object_init(instance);

        //call func
        MonoMethod* printMessage_func = mono_class_get_method_from_name(mono_class, "PrintMessage", 0);
        if (printMessage_func == nullptr) {
            return;
        }
        mono_runtime_invoke(printMessage_func, instance, nullptr, nullptr);

        //call func w param
        MonoMethod* printCustomStr_func = mono_class_get_method_from_name(mono_class, "PrintCustomMessage", 1);
        // Step 1: Create a MonoString
        MonoString* monoStr = mono_string_new(s_data->app_domain, "Hello, Mono!");

        // Step 2: Allocate memory in the target process
        void* memory = malloc(sizeof(MonoString*));

        // Step 3: Write the address of the MonoString to the allocated memory
        memcpy(memory, &monoStr, sizeof(MonoString*));

        mono_runtime_invoke(printCustomStr_func, instance, &memory, nullptr);
        free(memory);
        delete []monoStr;
        delete[]instance;
        //delete[]root_domain;
        mono_jit_cleanup(root_domain);
    }

    void ScriptEngine::ShutdownMono()
    {
        
    }

    

}