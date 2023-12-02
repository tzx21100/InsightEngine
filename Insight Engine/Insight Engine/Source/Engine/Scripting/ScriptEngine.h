/*!
 * \file ScriptEngine.h
 * \author Matthew Ng, matthewdeen.ng@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This header file declares the implementation of the Script engine
 * and also the implementation of scriptclass as a friend to scriptengine.
 * The ScriptEngine class manages the interaction with C# scripting,
 * and the ScriptClass class provides a way to work with C# classes.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /* Include Guard */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTENGINE_H
#define GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTENGINE_H

#include "Engine/Core/Core.h"

/* C# Type Forward Declarations */
extern "C" {
    // Type forward declarations for Mono runtime.
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
}

namespace IS {

    /*!
     * \brief The ScriptEngine class manages interaction with C# scripting.
     *
     * This class provides functions to initialize and shutdown the Mono runtime,
     * load C# assembly files, and instantiate C# classes for scripting within the engine.
     */
    class IS_API ScriptEngine {
    public:
        /*!
         * \brief Initializes the ScriptEngine.
         *
         * This function sets up the Mono runtime and should be called before any other ScriptEngine operations.
         */
        static void Init();

        /*!
         * \brief Shuts down the ScriptEngine.
         *
         * This function releases resources associated with the Mono runtime and should be called during cleanup.
         */
        static void Shutdown();

        /*!
         * \brief Loads a C# assembly file for scripting.
         *
         * \param file_path The path to the C# assembly file to load.
         */
        static void LoadAssembly(const std::filesystem::path& file_path);

        /*!
         * \brief Initializes the Mono runtime.
         *
         * This function sets up the Mono runtime and its root domain.
         * It should be called internally during ScriptEngine initialization.
         */
        static void InitMono();

        /*!
         * \brief Shuts down the Mono runtime.
         *
         * This function releases resources associated with the Mono runtime,
         * including the application domain and the root domain.
         * It should be called internally during ScriptEngine shutdown.
         */
        static void ShutdownMono();

        /*!
         * \brief Instantiates a C# class.
         *
         * This function is used internally to create instances of C# classes.
         * It takes a MonoClass pointer representing the class to instantiate.
         *
         * \param mono_class A pointer to the MonoClass representing the C# class.
         * \return A MonoObject pointer representing the instantiated class.
         */
        static MonoObject* InstantiateClass(MonoClass* mono_class);
        static MonoObject* InstantiateClass(const std::string& class_namespace, const std::string& class_name);



        /* Friend Declaration */
        friend class ScriptClass;
    };

    /*!
     * \brief The ScriptClass class provides a way to work with C# classes.
     *
     * This class allows you to specify a C# class by its namespace and name,
     * instantiate objects of that class, and invoke methods on those objects.
     */
    class IS_API ScriptClass
    {
    public:

        ScriptClass() = default; //default ctor
        /*!
         * \brief Constructor to specify the C# class by namespace and name.
         *
         * \param class_namespace The namespace of the C# class.
         * \param class_name The name of the C# class.
         */
        ScriptClass(const std::string& class_namespace, const std::string& class_name);

        /*!
         * \brief Instantiates an object of the specified C# class.
         *
         * \return A MonoObject pointer representing the instantiated C# class object.
         */
        MonoObject* Instantiate();

        /*!
         * \brief Gets a C# method by name and parameter count.
         *
         * This function retrieves a C# method of the class based on its name and the number of parameters it takes.
         *
         * \param name The name of the C# method.
         * \param parameter_count The number of parameters the method takes.
         * \return A MonoMethod pointer representing the retrieved C# method.
         */
        MonoMethod* GetMethod(const std::string& name, int parameter_count);

        /*!
         * \brief Invokes a C# method on a C# class object.
         *
         * This function calls a C# method on a C# class object, optionally passing parameters.
         *
         * \param instance A MonoObject pointer representing the C# class object.
         * \param method A MonoMethod pointer representing the C# method to invoke.
         * \param params An array of void pointers representing method parameters (optional).
         * \return A MonoObject pointer representing the result of the method invocation.
         */
        MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

        std::string mClassNamespace; ///< The namespace of the C# class.
        std::string mClassName;      ///< The name of the C# class.
        MonoClass* mMonoClass = nullptr; ///< Pointer to the MonoClass representing the C# class.


        MonoMethod* mInitMethod = nullptr;
        MonoMethod* mUpdateMethod = nullptr;
        MonoObject* mInstance = nullptr;

    
        void LoadMethods() {
            mInitMethod = GetMethod("Init", 0);
            mUpdateMethod = GetMethod("Update", 0);
        }

        void InstantiateAndInitialize() {
            mInstance = Instantiate();
            if (mInitMethod) {
                InvokeMethod(mInstance, mInitMethod);
            }
        }

        void Update() {
            if (mUpdateMethod) {
                InvokeMethod(mInstance, mUpdateMethod);
            }
        }


    };
}

/* End of Include Guard */
#endif // GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTENGINE_H
