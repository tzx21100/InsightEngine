#pragma once

#include "System.h"
#include "ScriptEngine.h"
#include <iostream>
#include <fstream>
#include <string>

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

        //all scripts
        void InitScripts();
        //specific script
        void InitScript(ScriptComponent& scriptcomponent);

    // Function to create a C# class file from a template
        void CreateClassFile(const std::string& className, const std::string& filePath) {
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
            }
            else {
                IS_CORE_DEBUG("Script file failed to be created, unable to open file for writing");
            }
        }
        //end of create file

    // Usage example...
    //CreateClassFile("MyClass", "MyClass.cs");



    };
}