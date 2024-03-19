/*!
 * \file Input.cpp
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu,
 *         Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 26-09-2023
 * \brief
 * This source file defines the input that abstracts from GLFW to create its
 * own inputs from keyboard and mouse. It creates an input System that can
 * be used by the developers.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Input.h"

#include "Engine/Systems/Asset/Asset.h"
#include "Graphics/Core/Graphics.h"
#include "Scene/SceneManager.h"
#include "Editor/Utils/FileUtils.h"


namespace IS {

    std::string InputManager::GetName() { return "Input"; }

    static void controllerCallBack(int jid, int event1)
    {
        if (jid != GLFW_JOYSTICK_1)
            return;
        INPUT_MANAGER->mControllerConnected = event1 == GLFW_CONNECTED;
    }

    void InputManager::Initialize() {
        // Subscirbe to messages
        Subscribe(MessageType::DebugInfo);

        GLFWwindow* native_window = mWindow->GetNativeWindow();
        glfwSetWindowUserPointer(native_window, this); // Set InputManager as user pointer
        glfwSetKeyCallback(native_window, KeyCallback);
        glfwSetJoystickCallback(controllerCallBack);
        glfwSetMouseButtonCallback(native_window, MouseButtonCallback);

        // Mouse scroll callback
        glfwSetScrollCallback(native_window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));
            input.mMouseScrollXOffset = xoffset;
            input.mMouseScrollYOffset = yoffset;
            IS_CORE_DEBUG("X: {}, Y: {}", input.mMouseScrollXOffset, input.mMouseScrollYOffset);
        });

        // Window size callback
        glfwSetWindowSizeCallback(native_window, [](GLFWwindow* window, int width, int height) {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));
            
            // Store non-fullscreen window size (top-left of window)
            if (input.mWindow->mProps.fullscreen || input.mWindow->mProps.maximized || !input.mWindow->mIsFocused)
                return;

            input.mWindow->SetWindowSize(width, height);
            IS_CORE_DEBUG("Width : {}, Height : {}", width, height);
        });

        // Window maximize callback
        glfwSetWindowMaximizeCallback(native_window, [](GLFWwindow* window, int maximized)
        {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));
            input.mWindow->mProps.maximized = maximized ? true : false;
            input.mWindow->StorePreviousWindowData();
            if (!maximized)
            {
                input.mWindow->SetWindowSize(input.mWindow->mProps.width, input.mWindow->mProps.height);
            }
        });

        // Accept file payload
        glfwSetDropCallback(native_window, FileDropCallback);

        // Minimize window if ALT+TAB / CTRL+ALT+DEL
        glfwSetWindowFocusCallback(native_window, [](GLFWwindow* window, int focused)
        {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));
            input.mWindow->mIsFocused = focused ? true : false;
            if (!focused && InsightEngine::Instance().mRuntime)
            {
                glfwIconifyWindow(window);
            }
        });

        glfwSetWindowIconifyCallback(native_window, [](GLFWwindow* window, int iconified)
        {
            InputManager& input = *(static_cast<InputManager*>(glfwGetWindowUserPointer(window)));
            input.mWindow->mIsMinimized = iconified ? true : false;
            if (!iconified)
            {
                if (!input.mWindow->IsFullScreen())
                {
                    glfwSetWindowMonitor(window, nullptr, input.mWindow->mPreviousX, input.mWindow->mPreviousY,
                                         input.mWindow->mPreviousWidth, input.mWindow->mPreviousHeight, 0);
                }
                else
                {
                    GLFWmonitor* monitor = input.mWindow->GetActiveMonitor();
                    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                }
                //glfwRestoreWindow(window);

                int width, height;
                input.mWindow->GetWindowSize(width, height);
                IS_CORE_DEBUG("Window Width : {}, Window Height : {}", width, height);
            }
        });
    }

    void InputManager::Update([[maybe_unused]] float deltaTime)
    {

        mReleasedControllerKeys.clear();

        // Important comments. Do not delete.
        if (true) {

            if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
                if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) { //>+O
                    //record key inputs                         -|
                    GLFWgamepadstate a;
                    if (glfwGetGamepadState(GLFW_JOYSTICK_1, &a)) {
                        for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
                        {
                            if (a.buttons[i] == GLFW_PRESS) {                        // --------  |
                                if (mPressedControllerKeys.count(i) > 0) {//       |   |
                                    mHeldControllerKeys.insert(i); //              |   |
                                    mPressedControllerKeys.erase(i);//             ~~~~~
                                }
                                else {
                                    mPressedControllerKeys.insert(i);
                                }
                            }
                            else {
                                mReleasedControllerKeys.insert(i);
                                mHeldControllerKeys.erase(i);
                                mPressedControllerKeys.erase(i);
                            }

                        }

                    }
                }
            }

        }



        //keyboard
        for (auto const& key : mHeldKeys) {
            mPressedKeys.erase(key);
        }
        for (auto const& key : mPressedKeys) {
            mHeldKeys.insert(key);
        }

        //mouse
        for (auto const& button : mHeldMouseButtons) {
            mPressedMouseButtons.erase(button);
        }
        for (auto const& button : mPressedMouseButtons) {
            mHeldMouseButtons.insert(button);
        }

        // process file payloads
        ProcessPayloads();

        // clear the file payloads after processing
        mPayloads.clear();
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

    void InputManager::PollEvents()
    {
        glfwPollEvents();
    }

    bool InputManager::IsKeyPressed(int glfwKeyCode) const {
        return mPressedKeys.count(glfwKeyCode) > 0;
    }

    bool InputManager::IsKeyReleased(int glfwKeyCode) const {
        return mReleasedKeys.count(glfwKeyCode) > 0;
    }

    bool InputManager::IsKeyHeld(int glfwKeyCode) const {
        return mHeldKeys.count(glfwKeyCode) > 0;
    }

    bool InputManager::IsControllerKeyPressed(int glfwKeyCode) const {
        return mPressedControllerKeys.count(glfwKeyCode) > 0;
    }

    bool InputManager::IsControllerKeyReleased(int glfwKeyCode) const {
        return mReleasedControllerKeys.count(glfwKeyCode) > 0;
    }

    bool InputManager::IsControllerKeyHeld(int glfwKeyCode) const {
        return mHeldControllerKeys.count(glfwKeyCode) > 0;
    }

    bool InputManager::IsMouseButtonPressed(int button) const {
        return mPressedMouseButtons.count(button) > 0;
    }

    bool InputManager::IsMouseButtonReleased(int button) const {
        return mReleasedMouseButtons.count(button) > 0;
    }

    bool InputManager::IsMouseButtonHeld(int button) const {
        return mHeldMouseButtons.count(button) > 0;
    }

    void InputManager::ProcessPayloads()
    {
        for (auto const& payload : mPayloads)
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

    void InputManager::ProcessPayloadFile([[maybe_unused]] std::filesystem::path const& filepath)
    {
#ifdef USING_IMGUI
        auto& scene_manager = SceneManager::Instance();
        auto& engine = InsightEngine::Instance();
        auto asset = engine.GetSystem<AssetManager>("Asset");
        std::string const& relative_filepath = std::filesystem::relative(filepath).string();
        std::filesystem::path const& extension = filepath.extension();
        std::ostringstream oss; // used to construct new filepath
        std::string directory;

        if (extension == ".insight") // Scene
        {
            directory = AssetManager::SCENE_DIRECTORY;
            FileUtils::FileMakeCopy(relative_filepath, directory);
            oss << directory << "/" << filepath.filename().string();
            std::string new_filepath = oss.str();
            scene_manager.LoadScene(new_filepath);
        }
        else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")  // Texture
        {
            directory = AssetManager::TEXTURE_DIRECTORY;
            FileUtils::FileMakeCopy(relative_filepath, directory);
            oss << directory << "/" << filepath.filename().string();
            std::string new_filepath = oss.str();
            asset->WorkerLoadImageData(new_filepath);
        }
        else if (extension == ".MP3" || extension == ".WAV" || extension == ".wav" || extension == ".mp3") // Audio
        {
            directory = AssetManager::SOUND_DIRECTORY;
            FileUtils::FileMakeCopy(relative_filepath, directory);
            oss << directory << "/" << filepath.filename().string();
            std::string new_filepath = oss.str();
            asset->LoadAudio(new_filepath);
        }
        else if (extension == ".json") // Prefab
        {
            directory = AssetManager::PREFAB_DIRECTORY;
            FileUtils::FileMakeCopy(relative_filepath, directory);
            oss << directory << "/" << filepath.filename().string();
            std::string new_filepath = oss.str();
            asset->LoadPrefab(new_filepath);
        }
        else if (extension == ".vert" || extension == ".frag") // Shader
        {
            directory = AssetManager::SHADER_DIRECTORY;
            FileUtils::FileMakeCopy(relative_filepath, directory);
            oss << directory << "/" << filepath.filename().string();
            std::string new_filepath = oss.str();
            asset->LoadShader(new_filepath);
        }
        else if (extension == ".txt") // Particle
        {
            directory = AssetManager::PARTICLE_DIRECTORY;
            FileUtils::FileMakeCopy(relative_filepath, directory);
            oss << directory << "/" << filepath.filename().string();
            std::string new_filepath = oss.str();
            asset->LoadParticleFromFile(new_filepath);
        }
        else // Unsupported file type
        {
            std::string message = std::format("File type \"{}\" not supported!", extension.string());
            std::string title = "Unsupported File Type";
            mWindow->ShowMessageBox(message, title);
        }
#endif
    }

    void InputManager::KeyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            inputManager->mPressedKeys.insert(key);
            inputManager->mReleasedKeys.erase(key);
            inputManager->mKeyPressTime[key] = glfwGetTime();
        }
        if (action == GLFW_RELEASE) {
            inputManager->mPressedKeys.erase(key);
            inputManager->mReleasedKeys.insert(key);
            inputManager->mHeldKeys.erase(key); // Remove from held_keys when released
        }
        
    }

    void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            inputManager->mPressedMouseButtons.insert(button);
            inputManager->mReleasedMouseButtons.erase(button);
        }
        if (action == GLFW_RELEASE) {
            inputManager->mPressedMouseButtons.erase(button);
            inputManager->mReleasedMouseButtons.insert(button);
            inputManager->mHeldMouseButtons.erase(button); // Remove from held_mouse_buttons when released
        }
    }

    void InputManager::FileDropCallback(GLFWwindow* window, int count, const char** paths)
    {
        InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        for (int i{}; i < count; ++i)
        {
            std::filesystem::path path = std::filesystem::relative(paths[i]);
            input->mPayloads.emplace(path);
        }
    }

} // end namespace IS
