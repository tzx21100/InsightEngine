    /*!
 * \file SceneManager.h
 * \author  Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
 * \brief
 * This header file declares the ScriptManager system and ScriptComponent for
 * our scripting engine to be incorporated with the ECS.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                      guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_SCENE_SCRIPTMANAGER_H
#define GAM200_INSIGHT_ENGINE_EDITOR_SCENE_SCRIPTMANAGER_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Engine/ECS/Component.h"
#include "Engine/ECS/System.h"
#include "ScriptEngine.h"

#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>

namespace IS {

    /*!
     * \class ScriptComponent
     * \brief Represents a script component in the entity-component system.
     * \details
     * This component holds information related to a script, including the script name,
     * initialization state, and the instance of the script object.
     */
    class ScriptComponent : IComponent {
    public:
        static std::string GetType() {
            return "ScriptComponent";
        }
        bool mInited = false;
        std::string mScriptName;
        ScriptClass scriptClass;
        MonoObject* instance = nullptr;

        Json::Value Serialize() {
            Json::Value scriptData;

            // Serialize mScriptName
            scriptData["ScriptName"] = mScriptName;

            return scriptData;
        }

        void Deserialize(const Json::Value& data) {
            mScriptName = data["ScriptName"].asString();
        }

    };

    /*!
     * \class ScriptManager
     * \brief A system to manage script components and interactions.
     * \details
     * This system manages the initialization, update, and interaction of script components,
     * as well as the creation and management of C# class files.
     */
    class ScriptManager :public ParentSystem {
    public:

        bool mLoadScene;

        // for sprites 
        std::array<int, 32> mTextureIndexList;
        std::stack<int> availableIndices;

        /*!
         * \brief Initializes the available texture indices.
         */
        void InitializeTextureIndices() {
            for (int i = 31; i >= 0; i--) {
                availableIndices.push(i);
            }
            availableIndices.pop();
        }

        std::string GetName() override;
        void Initialize() override;
        void Update(float deltatime) override;

        /*!
         * \brief Initializes all scripts.
         */
        void InitScripts();

        /*!
         * \brief Initializes a specific script.
         * \param scriptcomponent The script component to initialize.
         * \return True if the script is successfully initialized, false otherwise.
         */
        bool InitScript(ScriptComponent& scriptcomponent);

        /*!
         * \brief Creates a C# class file from a template.
         * \param className The name of the class.
         * \param filePath The file path where the class file will be created.
         */
        void CreateClassFile(const std::string& className, const std::string& filePath);

        /*!
         * \brief Opens a C# class file in the default application (e.g., Visual Studio Code).
         * \param class_name The name of the class file to open.
         */
        void OpenClassFile(std::string const& class_name);

    // Usage example...
    //CreateClassFile("MyClass", "MyClass.cs");

        /*!
         * \brief Gets the entity that called the script.
         * \return The entity that called the script.
         */
        Entity GetScriptEntity() {
            return mEntityScriptCaller;
        }

        float mScriptDeltaTime{};


        void ReloadAllScriptClasses();


        /*!
         * \brief Cleans up the script manager.
         */
        void CleanUp();


    private:
        Entity mEntityScriptCaller{};
        float mTargetTime = 1.f / 60.f;
        float mAddedTime = 0.f;


    };
}

#endif