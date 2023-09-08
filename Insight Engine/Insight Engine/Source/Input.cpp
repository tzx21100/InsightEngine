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

        //this is an example
        //if (IsKeyHeld(GLFW_KEY_A)) { std::cout << "A"; }

    }

    void InputManager::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            IS_CORE_INFO("Handling Debug");
        }
    }


    InputManager::InputManager() {
        window = glfwGetCurrentContext();
        glfwSetWindowUserPointer(window, this); // Set InputManager as user pointer
        glfwSetKeyCallback(window, KeyCallback);
    }

    bool InputManager::IsKeyPressed(int glfwKeyCode) const {
        return pressed_keys.count(glfwKeyCode) > 0;
    }

    bool InputManager::IsKeyReleased(int glfwKeyCode) const {
        return released_keys.count(glfwKeyCode) > 0;
    }

    bool InputManager::IsKeyHeld(int glfwKeyCode) const {
        return held_keys.count(glfwKeyCode) > 0;
    }

    bool InputManager::IsMouseButtonPressed(int button) const {
        return glfwGetMouseButton(window, button) == GLFW_PRESS;
    }

    std::pair<double, double> InputManager::GetMousePosition() const {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return { xPos, yPos };
    }


    void InputManager::KeyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            IS_CORE_TRACE("Key {} Action {}", key, action);
            inputManager->pressed_keys.insert(key);
            inputManager->held_keys.insert(key); // Add to held_keys when pressed
        }
        if (action == GLFW_RELEASE) {
            inputManager->pressed_keys.erase(key);
            inputManager->released_keys.insert(key);
            inputManager->held_keys.erase(key); // Remove from held_keys when released
        }
    }

}