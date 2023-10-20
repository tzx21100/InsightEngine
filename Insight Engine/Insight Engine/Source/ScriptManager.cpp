#include "Pch.h"
#include "ScriptManager.h"
#include "CoreEngine.h"

#pragma warning(push)
#pragma warning(disable: 4005) // redefine APIENTRY && IS_WARN
#include <Windows.h> // for ShellExecute()
#pragma warning(pop)

namespace IS {

    std::string ScriptManager::GetName() { return "ScriptManager"; }

    void ScriptManager::InitScript(ScriptComponent &scriptcomponent) {
        scriptcomponent.scriptClass = ScriptClass("IS", scriptcomponent.mScriptName);
        scriptcomponent.instance = scriptcomponent.scriptClass.Instantiate();
        if (scriptcomponent.instance != nullptr) {
            scriptcomponent.scriptClass.LoadMethods();
            MonoMethod* init_method = scriptcomponent.scriptClass.GetMethod("Init", 0);
            scriptcomponent.scriptClass.InvokeMethod(scriptcomponent.instance, init_method, nullptr);
        }
    }

    void ScriptManager::InitScripts() {
        auto& engine = InsightEngine::Instance();
        for (auto& entity : mEntities) {
            auto& scriptcomponent = engine.GetComponent<ScriptComponent>(entity);
            //init all scripts 
            mEntityScriptCaller = entity;
            InitScript(scriptcomponent);
        }
    }

    void ScriptManager::Initialize() {

    }

    bool Init_scripts = false;
    void ScriptManager::Update([[maybe_unused]] float deltaTime) {
        if (!Init_scripts) { InitScripts(); Init_scripts = true; }
        auto& engine = InsightEngine::Instance();
        for (auto& entity: mEntities) {
            auto& scriptcomponent = engine.GetComponent<ScriptComponent>(entity);
            if (scriptcomponent.instance != nullptr) {
                mEntityScriptCaller = entity;
                MonoMethod* update_method = scriptcomponent.scriptClass.GetMethod("Update", 0);
                scriptcomponent.scriptClass.InvokeMethod(scriptcomponent.instance, update_method, nullptr);
            }
        }
    }

    void ScriptManager::CreateClassFile(const std::string& className, const std::string& filePath) {
        // Define the template for the C# class
        std::string classTemplate = R"(using System.Runtime.CompilerServices;
namespace IS
{
    class {CLASS_NAME}
    {
        static public void Init(){

        }

        static public void Update(){
            
        }

    }
}
)";

        // Replace placeholder in the template with the actual class name
        size_t pos;
        while ((pos = classTemplate.find("{CLASS_NAME}")) != std::string::npos) {
            classTemplate.replace(pos, 12, className);
        }

        // Write the resulting string to a file
        std::ofstream outFile(filePath);
        if (outFile.is_open()) {
            outFile << classTemplate;
            outFile.close();
            IS_CORE_DEBUG("Script file created sucessfully");
        } else {
            IS_CORE_DEBUG("Script file failed to be created, unable to open file for writing");
        }
    } //end of create file

    void ScriptManager::OpenClassFile(std::string const& class_name) {
        std::string filepath = class_name + ".cs";
        std::string directory = "..\\IS-ScriptCore\\Source";

        // Open .cs file in default program
        ShellExecute(NULL, NULL, filepath.c_str(), NULL, directory.c_str(), SW_SHOW);
        IS_CORE_DEBUG("Script File: {}", filepath);
    }
}