#pragma once

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
        std::string GetName() override;
        void Initialize() override;
        void Update(float deltatime) override;

        //all scripts
        void InitScripts();
        //specific script
        void InitScript(ScriptComponent& scriptcomponent);

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