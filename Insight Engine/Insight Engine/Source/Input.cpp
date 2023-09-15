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

    void InputManager::Update([[maybe_unused]] float deltaTime) {

        //this is an example
        //if (IsKeyHeld(GLFW_KEY_A)) { std::cout << "A"; }

    }

    void InputManager::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            //IS_CORE_INFO("Handling Debug");
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

    //our world pos 0,0 is in the center
    std::pair<double, double> InputManager::GetMousePosition() const {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        double centerX = static_cast<double>(width) / 2.0;
        double centerY = static_cast<double>(height) / 2.0;

        // Translate to new coordinates
        double newX = xPos - centerX;
        double newY = centerY - yPos;  // Negate to make y-axis point upwards

        return { newX, newY };
    }

    void InputManager::KeyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        //std::string key_name = glfwGetKeyName(key, scancode) ? glfwGetKeyName(key, scancode) : "code: " + std::to_string(key);
        if (action == GLFW_PRESS) {
            //IS_CORE_TRACE("Key {} Pressed", key_name);
            inputManager->pressed_keys.insert(key);
            inputManager->held_keys.insert(key); // Add to held_keys when pressed
        }
        if (action == GLFW_RELEASE) {
            //IS_CORE_TRACE("Key {} Released", key_name);
            inputManager->pressed_keys.erase(key);
            inputManager->released_keys.insert(key);
            inputManager->held_keys.erase(key); // Remove from held_keys when released
        }
    }

}