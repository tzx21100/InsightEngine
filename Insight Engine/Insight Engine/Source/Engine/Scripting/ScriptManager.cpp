/*!
 * \file ScriptManager.cpp
 * \author  Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
 * \brief
 * This source file defines the simple system for calling all Init,Update and CleanUps
 * of the Scripts that designers use. It also defines the file that gets created for scripts
 * and it's format.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "ScriptManager.h"
#include "Engine/Core/CoreEngine.h"
#include "Editor/Utils/FileUtils.h"

namespace IS {

    std::string ScriptManager::GetName() { return "ScriptManager"; }

    bool ScriptManager::InitScript(ScriptComponent& scriptcomponent) {
        scriptcomponent.scriptClass = ScriptClass("IS", scriptcomponent.mScriptName);
        scriptcomponent.instance = scriptcomponent.scriptClass.Instantiate();
        if (scriptcomponent.instance != nullptr) {
            scriptcomponent.scriptClass.LoadMethods();
            MonoMethod* init_method = scriptcomponent.scriptClass.GetMethod("Init", 0);
            scriptcomponent.scriptClass.InvokeMethod(scriptcomponent.instance, init_method, nullptr);
            return true;
        }
        return false;
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
        InitializeTextureIndices();
    }

    void ScriptManager::Update([[maybe_unused]] float deltaTime) {


        if (InsightEngine::Instance().mRuntime == false) { return; }
        mScriptDeltaTime = deltaTime;
        auto& engine = InsightEngine::Instance();
        for (auto& entity : mEntities) {
            mEntityScriptCaller = entity;
            auto& scriptcomponent = engine.GetComponent<ScriptComponent>(entity);
            if (scriptcomponent.mInited == false) { if (InitScript(scriptcomponent)) { scriptcomponent.mInited = true; } }
            if (scriptcomponent.instance != nullptr) {
                MonoMethod* update_method = scriptcomponent.scriptClass.GetMethod("Update", 0);
                scriptcomponent.scriptClass.InvokeMethod(scriptcomponent.instance, update_method, nullptr);

            }
            if (mLoadScene) {
                mLoadScene = false;
                break;
            }
            if (InsightEngine::Instance().mRuntime == false) {
                break;
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
        
        static public void CleanUp(){

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
        }
        else {
            IS_CORE_DEBUG("Script file failed to be created, unable to open file for writing");
        }
    } //end of create file

    void ScriptManager::OpenClassFile(std::string const& class_name) {
        std::string filepath = class_name;
        std::string directory = "..\\Insight Engine\\Assets\\Scripts";
#ifdef USING_IMGUI
        // Open .cs file in default program
        FileUtils::OpenFileFromDefaultApp(filepath.c_str(), directory.c_str());
#endif // USING_IMGUI
        IS_CORE_DEBUG("Script File: {}", filepath);
    }

    void ScriptManager::CleanUp() {
        for (auto& entity : mEntities) {
            auto& scriptcomponent = InsightEngine::Instance().GetComponent<ScriptComponent>(entity);
            if (scriptcomponent.instance == nullptr) { return; }
            mEntityScriptCaller = entity;
            MonoMethod* cleanup = scriptcomponent.scriptClass.GetMethod("CleanUp", 0);
            scriptcomponent.scriptClass.InvokeMethod(scriptcomponent.instance, cleanup, nullptr);


        }
    }

    void ScriptManager::ReloadAllScriptClasses() {
        for (auto& entity : mEntities) {
            auto& scriptcomponent = InsightEngine::Instance().GetComponent<ScriptComponent>(entity);
            scriptcomponent.scriptClass.Unload();
        }

        InitScripts();
    }

}