/*!
 * \file ScriptEngine.cpp
 * \author Matthew Ng, matthewdeen.ng@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This source file implements the ScriptEngine class and related functionality for interacting with C# scripting.
 * The ScriptEngine class manages the interaction with C# scripting, and the ScriptClass class provides a way to work with C# classes.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*includes*/
#include "Pch.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>


namespace IS {


 

    namespace Utils {
        //helps read bytes of assembly
        static  char* ReadBytes(const std::filesystem::path& file_path, uint32_t* out_size)
        {
            std::ifstream stream(file_path, std::ios::binary | std::ios::ate);

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

        //loads assembly 
        static  MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assembly_path)
        {
            uint32_t file_size = 0;
            char* file_data = ReadBytes(assembly_path.c_str(), &file_size);

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

            // free the file data
            delete[] file_data;

            return assembly;
        }

        //for debugging - prints all c# classes/namespaces
        void PrintAssemblyTypes(MonoAssembly* assembly)
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

    struct ScriptEngineData //stores the needed pointers for ease of use
    {
        MonoDomain* mRootDomain = nullptr;
        MonoDomain* mAppDomain = nullptr;
        MonoAssembly* mCoreAssembly = nullptr;
        MonoImage* mCoreAssemblyImage = nullptr;
        ScriptClass mEntityClass;
    };

    static ScriptEngineData* s_data;

   
    void ScriptEngine::Init()
    {
        s_data = new ScriptEngineData();
        InitMono();
        LoadAssembly("Resources/Scripts/net6.0/IS-ScriptCore.dll");
        ScriptGlue::RegisterFunctions(); //register cpp func
      
    }

    void ScriptEngine::Shutdown()
    {
        ShutdownMono(); //frees memory
        delete s_data; //deletes unused pointers and class
    }

    void ScriptEngine::InitMono()
    {
        mono_set_assemblies_path("libraries/mono/lib/4.5");

        MonoDomain* root_domain = mono_jit_init("ISJITRuntime"); //init runtime
        if (root_domain == nullptr)
        {
            // Maybe log some error here
            return;
        }

        // Store the root domain pointer
        s_data->mRootDomain = root_domain;


    }

    void ScriptEngine::ShutdownMono()
    {
        mono_domain_set(mono_get_root_domain(), false);

        mono_domain_unload(s_data->mAppDomain);
        //s_data->appDomain = nullptr;

        if (s_data->mRootDomain)
        {
            mono_jit_cleanup(s_data->mRootDomain);
        }

    }

    void ScriptEngine::LoadAssembly(const std::filesystem::path& file_path)
    {
        // Create an App Domain
        s_data->mAppDomain = mono_domain_create_appdomain((char*)"ISScriptRuntime", nullptr);
        mono_domain_set(s_data->mAppDomain, true);

        s_data->mCoreAssembly = Utils::LoadMonoAssembly(file_path);
        s_data->mCoreAssemblyImage = mono_assembly_get_image(s_data->mCoreAssembly);

        //debug purposes
        //PrintAssemblyTypes(s_data->mCoreAssembly);
    }

    //added this for entities to be able to instantiate
    MonoObject* ScriptEngine::InstantiateClass(const std::string& class_namespace, const std::string& class_name) {
        ScriptClass scriptClass(class_namespace, class_name);
        return scriptClass.Instantiate();
    }

    MonoObject* ScriptEngine::InstantiateClass(MonoClass* mono_class) {
        if (mono_class == nullptr) { return nullptr; }
        MonoObject* instance = mono_object_new(s_data->mAppDomain, mono_class);
        mono_runtime_object_init(instance);
        return instance;
    }

    MonoObject* ScriptClass::Instantiate()
    {
        return ScriptEngine::InstantiateClass(mMonoClass);
    }

    ScriptClass::ScriptClass(const std::string& class_namespace, const std::string& class_name) //sets the mono class 
        : mClassNamespace(class_namespace), mClassName(class_name)
    {
        mMonoClass = mono_class_from_name(s_data->mCoreAssemblyImage, class_namespace.c_str(), class_name.c_str());
    }

    MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
    {
        return mono_class_get_method_from_name(mMonoClass, name.c_str(), parameterCount);
    }

    MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
    {
        return mono_runtime_invoke(method, instance, params, nullptr);

    }




    //recompile assemblies
    void RecompileAssembly(const std::vector<std::string>& sourcePaths, const std::string& outputPath) {
        std::ostringstream commandStream;
        commandStream << "csc -out:" << outputPath;

        for (const auto& path : sourcePaths) {
            commandStream << " " << path;
        }

        std::string command = commandStream.str();
        system(command.c_str());
    }


    static std::atomic<bool> isHotReloading = false;
    void ScriptEngine::HotReload()
    {
       
        IS_CORE_DEBUG("HOT RELOADED");
        std::cout << "HOT RELOADED";

        // Unload the current app domain    
        if (s_data->mAppDomain)
        {
            mono_domain_unload(s_data->mAppDomain);
            s_data->mAppDomain = nullptr;
        }

        // Recompile the script/assembly
        std::vector<std::string> files = { "../IS-ScriptCore/DefaultScripts/*", "Assets/Scripts/*" };
        RecompileAssembly(files, "Resources/Scripts/net6.0/IS-ScriptCore.dll");



        // Create a new app domain
        s_data->mAppDomain = mono_domain_create_appdomain((char*)"ISScriptRuntime", nullptr);
        mono_domain_set(s_data->mAppDomain, true);

        // Load the new assembly
        const std::filesystem::path newAssemblyPath = "Resources/Scripts/net6.0/IS-ScriptCore.dll";
        s_data->mCoreAssembly = Utils::LoadMonoAssembly(newAssemblyPath);
        s_data->mCoreAssemblyImage = mono_assembly_get_image(s_data->mCoreAssembly);

    }



}