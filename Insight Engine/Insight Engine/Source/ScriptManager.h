#pragma once

#include "System.h"
#include "ScriptEngine.h"

namespace IS {


    class ScriptComponent : IComponent {
    public:
        std::string mScriptName;
        ScriptClass scriptClass;
        MonoObject* instance = nullptr;
    };

    class ScriptManager :public ParentSystem {
    public:
        std::string GetName() override;
        void Initialize() override;
        void Update(float deltatime) override;

        void InitScripts();
       
    private:
        //std::unordered_map<Entity, ScriptComponent> mScriptComponents;
    };
}