#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "System.h"
#include "Graphics.h"

namespace IS {
    class glfxWindow : public ParentSystem {
    public:
        glfxWindow(GLFWwindow* windo);

        //override parent
        void Update(float deltaTime) override;
        std::string getName() override;
        void Initialize() override;
        void HandleMessage(const Message& message) override;

    private:
        GLFWwindow* window;
    };


}