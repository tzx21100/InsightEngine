#include "Pch.h"
#include "ScriptManager.h"
#include "CoreEngine.h"


namespace IS {

    std::string ScriptManager::GetName() { return "ScriptManager"; }

    void ScriptManager::InitScripts() {
        auto& engine = InsightEngine::Instance();
        for (auto& entity : mEntities) {
            auto& scriptcomponent = engine.GetComponent<ScriptComponent>(entity);
            scriptcomponent.scriptClass=ScriptClass("IS",scriptcomponent.mScriptName);
            scriptcomponent.scriptClass.LoadMethods();
            MonoMethod* init_method = scriptcomponent.scriptClass.GetMethod("Init", 0);
            scriptcomponent.scriptClass.InvokeMethod(scriptcomponent.instance, init_method, nullptr);

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
            MonoMethod* update_method = scriptcomponent.scriptClass.GetMethod("Update", 0);
            scriptcomponent.scriptClass.InvokeMethod(scriptcomponent.instance, update_method,nullptr);
            
        }
    }
}