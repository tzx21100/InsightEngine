/*!
 * \file WindowSystem.cpp
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
           Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 *      This source file defines the implementation for class WindowSystem,
 *      which encapsulates the functionalities of an application window.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "WindowSystem.h"
#include "JsonSaveLoad.h"

#include <glad/glad.h>

namespace IS {

    // In case "WindowProperties.json" is not found, window will use default properties
    WindowSystem::WindowProperties WindowSystem::mDefaultProperties{ "Insight Engine", 1600, 900, true, false };

    WindowSystem::WindowSystem() {
        LoadProperties();

        // Initialize GLFW library
        bool success = glfwInit();
        if (!success)
            IS_CORE_CRITICAL("Failed to to initialize GLFW libary!");

        // Specify minimum constraints in OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

        // Create a window and its OpenGL context
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        mMonitorWidth = mode->width;
        mMonitorHeight = mode->height;

        mWindow = glfwCreateWindow(mProps.mFullscreen ? mMonitorWidth : mProps.mWidth,
                                   mProps.mFullscreen ? mMonitorHeight : mProps.mHeight,
                                   mProps.mTitle.c_str(),
                                   mProps.mFullscreen ? monitor : nullptr, nullptr);

        if (!mWindow) {
            IS_CORE_CRITICAL("Failed to create OpneGL context!");
            glfwTerminate();
        }

        // Set initial window position
        glfwSetWindowPos(mWindow, 100, 100);

        // Default setting
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        EnableVsync(mProps.mVSync);

        glfwMakeContextCurrent(mWindow);

        // Load OpenGL function pointers using GLAD
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (!status) {
            IS_CORE_CRITICAL("Failed to initialize GLAD!");
            glfwDestroyWindow(mWindow);
            glfwTerminate();
        }

        if (GL_VERSION_4_5) {
            IS_CORE_INFO("Driver supports OpenGL 4.5");
        } else {
            IS_CORE_ERROR("Driver doesn't support OpenGL 4.5 - abort program");
            BROADCAST_MESSAGE(MessageType::Quit); // idk
        }
    }

    WindowSystem::~WindowSystem() {
        glfwDestroyWindow(mWindow);
        glfwTerminate();
        SaveProperties();
    }

    void WindowSystem::Initialize() {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);

    }

    void WindowSystem::Update(float)  {
        //register window closing 
        if (glfwWindowShouldClose(mWindow)) {
            Message quit = Message(MessageType::Quit);
            BROADCAST_MESSAGE(quit);
        }
    }

    void WindowSystem::SwapBuffers() { glfwSwapBuffers(mWindow); }

    void WindowSystem::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::Collide) {
            // Handle collision logic here
        }
    }

    std::string WindowSystem::GetName() { return "Window"; }

    uint32_t WindowSystem::GetWidth() const { return mProps.mWidth; }

    uint32_t WindowSystem::GetHeight() const { return mProps.mHeight; }

    std::pair<uint32_t, uint32_t> WindowSystem::GetWindowSize() const { return std::make_pair(mProps.mWidth, mProps.mHeight); }

    uint32_t WindowSystem::GetMonitorWidth() const { return mMonitorWidth; }

    uint32_t WindowSystem::GetMonitorHeight() const { return mMonitorHeight; }

    std::pair<uint32_t, uint32_t> WindowSystem::GetMonitorSize() const { return std::make_pair(mMonitorWidth, mMonitorHeight); }

    std::string WindowSystem::GetWindowTitle() const { return mProps.mTitle; }

    void WindowSystem::SetWindowSize(uint32_t width, uint32_t height) {
        mProps.mWidth = width;
        mProps.mHeight = height;

        glfwSetWindowSize(mWindow, width, height);
    }

    void WindowSystem::SetWindowTitle(std::string const& title) {
        mProps.mTitle = title;
        glfwSetWindowTitle(mWindow, title.c_str());
    }

    void WindowSystem::EnableVsync(bool enabled) {
        mProps.mVSync = enabled;
        glfwSwapInterval(enabled ? 1 : 0);
    }

    bool WindowSystem::IsVSync() const { return mProps.mVSync; }

    GLFWwindow* WindowSystem::GetNativeWindow() const { return mWindow; }

    void WindowSystem::SetFullScreen(bool fullscreen) {
        mProps.mFullscreen = fullscreen;

        // Windowed mode
        if (!fullscreen) {
            glfwSetWindowMonitor(mWindow, NULL, 100, 100, mProps.mWidth, mProps.mHeight, GLFW_DONT_CARE);
            return;
        }

        // Fullscreen mode
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(mWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    bool WindowSystem::IsFullScreen() const { return mProps.mFullscreen; }

    void WindowSystem::LoadProperties() {
        std::string filename = "Properties/WindowProperties.json";
        Json::Value properties;
        
        // Load window properties from JSON file
        bool success = LoadJsonFromFile(properties, filename);
        if (auto const& win_props = properties["WindowProperties"];
            success && win_props["Title"].isString() && win_props["Width"].isUInt() &&
            win_props["Height"].isUInt() && win_props["Vsync"].isBool() && win_props["Fullscreen"].isBool()) {

            mProps.mTitle  = win_props["Title"].asString();
            mProps.mWidth  = win_props["Width"].asUInt();
            mProps.mHeight = win_props["Height"].asUInt();
            mProps.mVSync  = win_props["Vsync"].asBool();
            mProps.mFullscreen = win_props["Fullscreen"].asBool();

            IS_CORE_INFO("Loaded window properties from \"{}\"", filename);
        } else { // Assign default properties
            mProps = mDefaultProperties;
            IS_CORE_INFO("Using default window properties");
        }
    }

    void WindowSystem::SaveProperties() {
        std::string filepath = "Properties/WindowProperties.json";
        Json::Value properties;
        auto& win_props = properties["WindowProperties"];
        win_props["Title"]  = mProps.mTitle;
        win_props["Width"]  = mProps.mWidth;
        win_props["Height"] = mProps.mHeight;
        win_props["Vsync"]  = mProps.mVSync;
        win_props["Fullscreen"] = mProps.mFullscreen;

        // Save window propeties to JSON file
        bool success = SaveJsonToFile(properties, filepath);
        success ? IS_CORE_INFO("Successfully saved window properties to \"{}\"!", filepath) :
                  IS_CORE_ERROR("Failed to save window propeties to \"{}\"!", filepath);
    }

} // end namespace IS
