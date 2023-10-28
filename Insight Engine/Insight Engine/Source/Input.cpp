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
#include "Editor.h"

namespace IS {

    std::string InputManager::GetName() { return "Input"; }

    void InputManager::Initialize() {
        // Subscirbe to messages
        Subscribe(MessageType::DebugInfo);

        GLFWwindow* native_window = mWindow->GetNativeWindow();
        glfwSetWindowUserPointer(native_window, this); // Set InputManager as user pointer
        glfwSetKeyCallback(native_window, KeyCallback);
        glfwSetMouseButtonCallback(native_window, MouseButtonCallback);

        // Window size callback
        glfwSetWindowSizeCallback(native_window, [](GLFWwindow* window, int width, int height) {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));
            
            // Store non-fullscreen window size (top-left of window)
            if (!input.mWindow->IsFullScreen())
                input.mWindow->SetWindowSize(width, height);
        });

        // Window position callback
        glfwSetWindowPosCallback(native_window, [](GLFWwindow* window, int xpos, int ypos) {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));

            // Store non-fullscreen window position (top-left of window)
            if (!input.mWindow->IsFullScreen())
                input.mWindow->SetWindowPos(xpos, ypos);
        });

        // Window maximize callback
        glfwSetWindowMaximizeCallback(native_window, [](GLFWwindow* window, int maximized) {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));

            if (maximized) {
                IS_CORE_DEBUG("Window was maximized.");
                input.mWindow->SetMaximized();
            } else {
                IS_CORE_DEBUG("Window was restored from maximized.");
                input.mWindow->SetMaximized(false);
            }
        });

        // Mouse scroll callback
        glfwSetScrollCallback(native_window, []([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double xoffset, double yoffset)
        {
            // Do nothing if it's using scene camera
            if (Camera::mActiveCamera == CAMERA_TYPE_SCENE)
                return;

            auto& engine = InsightEngine::Instance();
            auto editor = engine.GetSystem<Editor>("Editor");
            if (editor->GetEditorLayer()->IsScenePanelFocused())
                ISGraphics::cameras[Camera::mActiveCamera].ZoomCamera(static_cast<float>(yoffset));
        });
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


    InputManager::InputManager(std::shared_ptr<WindowSystem> window) {
        mWindow = window;
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

    std::pair<double, double> InputManager::GetMousePosition() const {

        double newX = (current_mouse_x - center_x) * ratio_width + ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().x;
        double newY = (center_y - current_mouse_y) * ratio_height + ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().y;  // Negate to make y-axis point upwards

        //InsightEngine& engine = InsightEngine::Instance();
        //auto const& window_sys = engine.GetSystem<WindowSystem>("Window");
        //auto [width, height] = window_sys->GetWindowSize();

        //newX += width / 2.f;
        //newY -= height / 2.f;


        // IS_CORE_DEBUG("{}, {}", newX, newY);

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

    void InputManager::mouse_pick_entity() {
       /* double xPos, yPos;
        glfwGetCursorPos(mWindow->GetNativeWindow(), &xPos, &yPos);
        std::cout << "xPos: " << xPos << "yPos: " << yPos << std::endl;*/

        double xPos, yPos;
        glfwGetCursorPos(mWindow->GetNativeWindow(), &xPos, &yPos);


        double newX = (xPos - center_x) * ratio_width + ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().x;
        double newY = (center_y - yPos) * ratio_height + ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().y;  // Negate to make y-axis point upwards

        InsightEngine& engine = InsightEngine::Instance();
        auto const& window_sys = engine.GetSystem<WindowSystem>("Window");
        auto [width, height] = window_sys->GetWindowSize();

        newX += width / 2.f;
        newY += height / 2.f;

        if (engine.mUsingGUI) ISGraphics::mFramebuffer->Bind(); // bind fb

        int entityID{};
        // Read the entityID value at the specified pixel coordinates
        glReadBuffer(GL_COLOR_ATTACHMENT1);
        glReadPixels(static_cast<GLint>(xPos), static_cast<GLint>(yPos), 1, 1, GL_RED_INTEGER, GL_INT, &entityID);
        //GLuint entityIDR = entityID.x;
        //std::cout << entityID << std::endl;
        IS_CORE_DEBUG("Entity ID Picked: {}", entityID);

        if (engine.mUsingGUI) ISGraphics::mFramebuffer->Unbind();
    }

} // end namespace IS
