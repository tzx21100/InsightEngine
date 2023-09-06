#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "System.h"
#include "Graphics.h"

namespace IS {
    class glfxWindow : public ParentSystem {
    public:
        glfxWindow(int new_width, int new_height, std::string const& new_title);

        //override parent
        void Update(float deltaTime) override;
        std::string getName() override;
        void Initialize() override;
        void HandleMessage(const Message& message) override;

    private:
        GLFWwindow* window = nullptr;
        int width, height;
        std::string title;
    };


}