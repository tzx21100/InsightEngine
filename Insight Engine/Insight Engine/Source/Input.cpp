/*!
 * \file Input.cpp
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This source file defines the input that abstracts from GLFW to create its
 * own inputs from keyboard and mouse. It creates an input System that can
 * be used by the developers.
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
#include "Input.h"

namespace IS {

    std::string InputManager::GetName() {
        return "Input";
    }

    void InputManager::Initialize() {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);
    }

    void InputManager::Update([[maybe_unused]] float deltaTime) {
        // poll for mouse/keyboard events before any update
        glfwPollEvents();

        //keyboard
        for (auto const& key : held_keys) {
            pressed_keys.erase(key);
        }
        for (auto const& key : pressed_keys) {
            held_keys.insert(key);
        }

        //mouse
        for (auto const& button : held_mouse_buttons) {
            pressed_mouse_buttons.erase(button);
        }
        for (auto const& button : pressed_mouse_buttons) {
            held_mouse_buttons.insert(button);
        }
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
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
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
        return pressed_mouse_buttons.count(button) > 0;
    }

    bool InputManager::IsMouseButtonReleased(int button) const {
        return released_mouse_buttons.count(button) > 0;
    }

    bool InputManager::IsMouseButtonHeld(int button) const {
        return held_mouse_buttons.count(button) > 0;
    }


    //our world pos 0,0 is in the center
    std::pair<double, double> InputManager::GetMousePosition() const {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        double newX = (xPos - center_x)*ratio_width;
        double newY = (center_y - yPos)*ratio_height;  // Negate to make y-axis point upwards

        return { newX, newY };
    }

    void InputManager::KeyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            inputManager->pressed_keys.insert(key);
            inputManager->released_keys.erase(key);
        }
        if (action == GLFW_RELEASE) {
            inputManager->pressed_keys.erase(key);
            inputManager->released_keys.insert(key);
            inputManager->held_keys.erase(key); // Remove from held_keys when released
        }
        
    }

    void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            inputManager->pressed_mouse_buttons.insert(button);
            inputManager->released_mouse_buttons.erase(button);
        }
        if (action == GLFW_RELEASE) {
            inputManager->pressed_mouse_buttons.erase(button);
            inputManager->released_mouse_buttons.insert(button);
            inputManager->held_mouse_buttons.erase(button); // Remove from held_mouse_buttons when released
        }
    }

}