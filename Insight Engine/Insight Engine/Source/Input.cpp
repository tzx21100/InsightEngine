/*!
 * \file Input.cpp
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu,
 *         Guo Yiming, yiming.guo@digipen.edu
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
            if (!(input.mWindow->IsFullScreen() || width == 0 || height == 0))
            {
                input.mWindow->SetWindowSize(width, height);
                IS_CORE_DEBUG("Width : {}, Height : {}", width, height);
            }
        });

        // Window position callback
        glfwSetWindowPosCallback(native_window, [](GLFWwindow* window, int xpos, int ypos)
        {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));

            // Store non-fullscreen window position (top-left of window)
            if (!input.mWindow->IsFullScreen())
                input.mWindow->SetWindowPos(xpos, ypos);
        });

        // Window maximize callback
        glfwSetWindowMaximizeCallback(native_window, [](GLFWwindow* window, int maximized)
        {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));
            input.mWindow->SetMaximized(maximized ? true : false);
        });

        // Accept file payload
        glfwSetDropCallback(native_window, FileDropCallback);

        // Minimize window if ALT+TAB / CTRL+ALT+DEL
        glfwSetWindowFocusCallback(native_window, [](GLFWwindow* window, int focused)
        {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));
            input.mWindow->SetFocused(focused ? true : false);
            if (!focused && InsightEngine::Instance().mRuntime)
            {
                input.mWindow->SetMinimized();
            }
        });

        // Minimize window callback
        glfwSetWindowIconifyCallback(native_window, [](GLFWwindow* window, int iconified)
        {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));
            input.mWindow->SetMinimized(iconified ? true : false);
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

        // process file payloads
        ProcessPayloads();

        // clear the file payloads after processing
        payloads.clear();
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

    void InputManager::ProcessPayloads()
    {
        for (auto const& payload : payloads)
        {
            ProcessPayloadDirectory(payload);
        }
    }

    void InputManager::ProcessPayloadDirectory(std::filesystem::path const& directory)
    {
        if (!std::filesystem::is_directory(directory))
        {
            ProcessPayloadFile(directory);
            return;
        }

        for (auto const& entry : std::filesystem::directory_iterator(directory))
        {
            std::filesystem::path path = std::filesystem::relative(entry);
            const bool is_directory = entry.is_directory();
            is_directory ? ProcessPayloadDirectory(path) : ProcessPayloadFile(path);
        }
    }

    void InputManager::ProcessPayloadFile(std::filesystem::path const& filepath)
    {
        auto& scene_manager = SceneManager::Instance();
        auto& engine = InsightEngine::Instance();
        auto asset = engine.GetSystem<AssetManager>("Asset");
        std::string const& filename = std::filesystem::relative(filepath).string();
        std::filesystem::path const& extension = filepath.extension();

        if (extension == ".insight") // Scene
        {
            scene_manager.LoadScene(filename);
        }
        else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")  // Texture
        { 
            asset->LoadImage(filename);
        }
        else if (extension == ".MP3" || extension == ".WAV" || extension == ".wav" || extension == ".mp3") // Audio
        { 
            asset->LoadAudio(filename);
        }
        else if (extension == ".json") // Prefab
        {
            asset->LoadPrefab(filename);
        }
        else // Unsupported file type
        {
            std::string message = std::format("File type \"{}\" not supported!", extension.string());
            std::string title = "Unsupported File Type";
            mWindow->ShowMessageBox(message, title);
        }
    }

    void InputManager::KeyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            inputManager->pressed_keys.insert(key);
            inputManager->released_keys.erase(key);
            inputManager->key_pressed_time[key] = glfwGetTime();
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

    void InputManager::FileDropCallback(GLFWwindow* window, int count, const char** paths)
    {
        InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        for (int i{}; i < count; ++i)
        {
            std::filesystem::path path = std::filesystem::relative(paths[i]);
            input->payloads.emplace(path);
        }
    }

} // end namespace IS
