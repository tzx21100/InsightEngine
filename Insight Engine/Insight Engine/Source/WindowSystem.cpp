/*!
 * \file WindowSystem.cpp
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
           Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 *      This source file defines the implementation for class WindowSystem,
 *      which encapsulates the functionalities of an application window.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "WindowSystem.h"
#include "JsonSaveLoad.h"

namespace IS {

    // In case <properties.json> is not found, window will use default properties
    WindowSystem::WindowProperties WindowSystem::mDefaultProperties{ "Insight Engine", 1600, 900, true };

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

        // Create a windowed mode window and its OpenGL context
        mWindow = glfwCreateWindow(mProps.mWidth, mProps.mHeight, mProps.mTitle.c_str(), nullptr, nullptr);
        if (!mWindow) {
            IS_CORE_CRITICAL("Failed to create OpneGL context!");
            glfwTerminate();
        }

        // Default setting
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        EnableVsync(mProps.mVSync);

        glfwMakeContextCurrent(mWindow);
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

    void WindowSystem::BeginUpdate() { glfwPollEvents(); }

    void WindowSystem::Update(float)  {
        //register window closing 
        if (glfwWindowShouldClose(mWindow)) {
            Message quit = Message(MessageType::Quit);
            BROADCAST_MESSAGE(quit);
        }
    }

    void WindowSystem::EndUpdate() { glfwSwapBuffers(mWindow); }

    void WindowSystem::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::Collide) {
            // Handle collision logic here
        }
    }

    std::string WindowSystem::GetName() { return "Window"; }

    uint32_t WindowSystem::GetWidth() const { return mProps.mWidth; }

    uint32_t WindowSystem::GetHeight() const { return mProps.mHeight; }

    std::pair<uint32_t, uint32_t> WindowSystem::GetWindowSize() const { return std::make_pair(mProps.mWidth, mProps.mHeight); }

    std::string WindowSystem::GetWindowTitle() const { return mProps.mTitle; }

    void WindowSystem::SetWindowSize(uint32_t width, uint32_t height) {
        mProps.mWidth = width;
        mProps.mHeight = height;

        glfwSetWindowSize(mWindow, width, height);
    }

    void WindowSystem::SetWindowTitle(std::string const& title) {
        glfwSetWindowTitle(mWindow, title.c_str());
    }

    void WindowSystem::EnableVsync(bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
        mProps.mVSync = enabled;
    }

    bool WindowSystem::IsVSync() const { return mProps.mVSync; }

    void WindowSystem::LoadProperties() {
        std::string filename = "Properties/WindowProperties.json";
        Json::Value properties;
        
        // Load window properties from JSON file
        bool success = LoadJsonFromFile(properties, filename);
        if (auto const& win_props = properties["WindowProperties"];
            success && win_props["Title"].isString() && win_props["Width"].isInt() &&
            win_props["Height"].isInt() && win_props["Vsync"].isString()) {

            mProps.mTitle  = win_props["Title"].asString();
            mProps.mWidth  = win_props["Width"].asInt();
            mProps.mHeight = win_props["Height"].asInt();
            mProps.mVSync  = win_props["Vsync"].asString() == "On" ? true : false;

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
        win_props["Title"]  = mProps.mTitle.c_str();
        win_props["Width"]  = mProps.mWidth;
        win_props["Height"] = mProps.mHeight;
        win_props["Vsync"]  = mProps.mVSync ? "On" : "Off";

        // Save window propeties to JSON file
        bool success = SaveJsonToFile(properties, filepath);
        success ? IS_CORE_INFO("Successfully saved window properties to \"{}\"!", filepath) :
                  IS_CORE_ERROR("Failed to save window propeties to \"{}\"!", filepath);
    }

} // end namespace IS
