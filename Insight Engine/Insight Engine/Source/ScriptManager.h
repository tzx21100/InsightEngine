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
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
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
#include "System.h"
#include "ScriptEngine.h"
#include <iostream>
#include <fstream>
#include <string>

namespace IS {


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

    class ScriptManager :public ParentSystem {
    public:

        // for sprites 
        std::array<int, 32> mTextureIndexList;
        std::stack<int> availableIndices;

        // Initialize the available indices
        void InitializeTextureIndices() {
            for (int i = 31; i >= 0; i--) {
                availableIndices.push(i);
            }
            availableIndices.pop();
        }

        std::string GetName() override;
        void Initialize() override;
        void Update(float deltatime) override;

        //all scripts
        void InitScripts();
        //specific script
        bool InitScript(ScriptComponent& scriptcomponent);

        // Function to create a C# class file from a template
        void CreateClassFile(const std::string& className, const std::string& filePath);

        // Function to open a C# class file in default application (i.e. Visual Studio Code)
        void OpenClassFile(std::string const& class_name);

    // Usage example...
    //CreateClassFile("MyClass", "MyClass.cs");

        //accessor funciton to get entity calling script
        Entity GetScriptEntity() {
            return mEntityScriptCaller;
        }

        float mScriptDeltaTime{};

        void CleanUp();


    private:
        Entity mEntityScriptCaller{};


    };
}

#endif