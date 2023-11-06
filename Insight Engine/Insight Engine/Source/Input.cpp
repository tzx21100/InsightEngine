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
        glfwSetCursorPosCallback(native_window, MousePositionCallback);

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

        // Accept file payload
        glfwSetDropCallback(native_window, FileDropCallback);
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

    std::pair<double, double> InputManager::GetMousePosition() {
        previousWorldMousePos = currentWorldMousePos;
        
        double newX = (current_mouse_x - center_x) * ratio_width + ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().x;
        double newY = (center_y - current_mouse_y) * ratio_height + ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().y;  // Negate to make y-axis point upwards
        // IS_CORE_DEBUG("{}, {}", newX, newY);

        currentWorldMousePos = { static_cast<float>(newX), static_cast<float>(newY) };
        //current_mouse_x = newX;
        //current_mouse_y = newY;

       /* if (currentWorldMousePos.x != previousWorldMousePos.x) {
            std::cout << "change!" << std::endl;
        }*/

        return { newX, newY };
    }

    std::pair<double, double> InputManager::GetPreviousMousePosition() const {
        return { previous_mouse_x, previous_mouse_y };
    }

    void InputManager::PrintMouseWorldPos() {
        auto& engine = InsightEngine::Instance();
        auto const window = engine.GetSystem<WindowSystem>("Window");

        auto const editor = engine.GetSystem<Editor>("Editor");
        auto const editor_layer = editor->GetEditorLayer();
        auto const& viewport_size = editor_layer->GetViewportSize();

        Camera& cameraInUse = ISGraphics::cameras[Camera::mActiveCamera];
        double SHw = editor_layer->GetHierarchyPanel().x; // change to scene hierarchy panel width
        double LCh = editor_layer->GetConsolePanelSize().y; // change to log console height

        // step 1: get screen coords
        //double mouseX{}, mouseY{};
        //glfwGetCursorPos(window->GetNativeWindow(), &mouseX, &mouseY);
        //IS_CORE_DEBUG("screen mouse pos: {}, {}", mouseX, mouseY);
        //IS_CORE_DEBUG("screen mouse pos: {:.2f}, {:.2f}", current_mouse_x, current_mouse_y);

        // step 2: translate to cartesian
        double cartesianMouseX{ current_mouse_x - (engine.GetWindowWidth() / 2) }, cartesianMouseY{ current_mouse_y + ((engine.GetWindowHeight()) / 2) }; // 40 is the height of window title
        //IS_CORE_DEBUG("cartesian screen mouse pos: {:.2f}, {:.2f}", cartesianMouseX, cartesianMouseY);

        // step 3: normalize to VP coords
        double VPMouseX{ cartesianMouseX - SHw }, VPMouseY{ cartesianMouseY - LCh };
        //IS_CORE_DEBUG("VP mouse pos: {:.2f}, {:.2f}", VPMouseX, VPMouseY);

        // step 4: scale to cam dim
        double WorldMouseX{ VPMouseX / viewport_size.x * cameraInUse.GetCamDim().x }, WorldMouseY{ VPMouseY / viewport_size.y * cameraInUse.GetCamDim().y };
        
        // step 5: translate to camera pos
        WorldMouseX += cameraInUse.GetCamPos().x;
        WorldMouseY += cameraInUse.GetCamPos().y;
        //IS_CORE_DEBUG("World mouse pos: {:.2f}, {:.2f}", WorldMouseX, WorldMouseY);

        world_mouse_x = WorldMouseX;
        world_mouse_y = WorldMouseY;

        //IS_CORE_DEBUG("Current mouse pos: {}, {}", current_mouse_x, current_mouse_y); //0 ,0 to 1919, 1016
        //double newX = (current_mouse_x - center_x) * ratio_width + ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().x;
        //double newY = (center_y - current_mouse_y) * ratio_width + ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().y;  // Negate to make y-axis point upwards

        //InsightEngine& engine = InsightEngine::Instance();
        //auto const& window_sys = engine.GetSystem<WindowSystem>("Window");
        //auto [width, height] = window_sys->GetWindowSize();

        //newX += width / 2.f;
        //newY -= height / 2.f;

       

       /* int winW, winH;
        glfwGetFramebufferSize(window, &winW, &winH);*/

        //WindowSystem::GetWidth();

        //IS_CORE_DEBUG("world mouse pos: {}, {}", engine.GetWindowWidth(), engine.GetWindowHeight());
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

        // Scene file
        if (extension == ".insight") { scene_manager.LoadScene(filename); }
        // Texture file
        else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") { asset->LoadImage(filename); }
        // Audio file
        else if (extension == ".MP3" || extension == ".WAV" || extension == ".wav" || extension == ".mp3") {  asset->LoadAudio(filename);  }
        // Prefab file
        else if (extension == ".json") { asset->LoadPrefab(filename); }
        else { IS_CORE_WARN("File type \"{}\" not supported!", extension.string()); }
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

    void InputManager::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        input->current_mouse_x = xpos;
        input->current_mouse_y = ypos;
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
        //IS_CORE_DEBUG("Entity ID Picked: {}", entityID);

        if (engine.mUsingGUI) ISGraphics::mFramebuffer->Unbind();
    }

} // end namespace IS
