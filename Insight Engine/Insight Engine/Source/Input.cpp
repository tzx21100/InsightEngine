//pch has to go to the top of every cpp
#include "Pch.h"
#include "Input.h"
namespace IS {

    std::string InputManager::getName() {
        return "InputManager";
    }

    void InputManager::Initialize() {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);
    }

    void InputManager::Update(float deltaTime) {

        //test for Input System will make it with held keys etc later
        if (this->IsKeyPressed(GLFW_KEY_A)) {
            std::cout << "A IS PRESSED";
        }
        //other logic
        m_PressedKeys.clear();
        m_ReleasedKeys.clear();

    }

    void InputManager::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            std::cout << "Handling Debug" << std::endl;
        }
    }


    InputManager::InputManager(GLFWwindow* window) : m_Window(window) {
        glfwSetWindowUserPointer(window, this); // Set InputManager as user pointer
        glfwSetKeyCallback(window, KeyCallback);
    }

    bool InputManager::IsKeyPressed(int glfwKeyCode) const {
        return m_PressedKeys.count(glfwKeyCode) > 0;
    }

    bool InputManager::IsKeyReleased(int glfwKeyCode) const {
        return m_ReleasedKeys.count(glfwKeyCode) > 0;
    }

    bool InputManager::IsKeyHeld(int glfwKeyCode) const {
        return IsKeyPressed(glfwKeyCode) && !IsKeyReleased(glfwKeyCode);
    }

    bool InputManager::IsMouseButtonPressed(int button) const {
        return glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
    }

    std::pair<double, double> InputManager::GetMousePosition() const {
        double xPos, yPos;
        glfwGetCursorPos(m_Window, &xPos, &yPos);
        return { xPos, yPos };
    }


    void InputManager::KeyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            std::cout << "KeyCallback: Key " << key << " Action " << action << std::endl;
            inputManager->m_PressedKeys.insert(key);
        }
        if (action == GLFW_RELEASE) {
            inputManager->m_PressedKeys.erase(key);
            inputManager->m_ReleasedKeys.insert(key);
        }
    }
}