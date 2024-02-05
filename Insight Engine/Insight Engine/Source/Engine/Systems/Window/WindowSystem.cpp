/*!
 * \file WindowSystem.cpp
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
           Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This source file defines the implementation for class WindowSystem,
 * which encapsulates the functionalities of an application window.
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
#include "WindowSystem.h"
#include "Engine/JSON/JsonSaveLoad.h"
#include "Engine/Systems/Asset/Asset.h"

#pragma warning(push)
#pragma warning(disable: 4005) // redefine APIENTRY and IS_ERROR macros
#include <Windows.h>
#pragma warning(pop)
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace IS {

    // In case "WindowProperties.json" is not found, window will use default properties
    static const WindowSystem::WindowProperties DEFAULT_PROPERTIES{ "Insight Engine", 1280, 720, 60 };

    WindowSystem::WindowSystem()
    {
        LoadProperties();

        mWidthBeforeFullscreen  = mProps.mWidth;
        mHeightBeforeFullscreen = mProps.mHeight;

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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, mProps.mMaximized ? GLFW_TRUE : GLFW_FALSE);

        // Save monitor dimensions
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        mMonitorWidth = mode->width;
        mMonitorHeight = mode->height;

        // Validate window size
        if (mMonitorWidth < mProps.mWidth || mProps.mWidth < DEFAULT_PROPERTIES.mWidth ||
            mMonitorHeight < mProps.mHeight || mProps.mHeight < DEFAULT_PROPERTIES.mHeight)
        {
            mProps = DEFAULT_PROPERTIES;

            mWidthBeforeFullscreen = mProps.mWidth;
            mHeightBeforeFullscreen = mProps.mHeight;
        }

        // Create a window and its OpenGL context
        mWindow = mProps.mFullscreen
            ? glfwCreateWindow(mMonitorWidth, mMonitorHeight, mProps.mTitle.c_str(), monitor, nullptr)
            : glfwCreateWindow(mProps.mWidth, mProps.mHeight, mProps.mTitle.c_str(), nullptr, nullptr);

        // Validate window
        if (!mWindow)
        {
            IS_CORE_CRITICAL("Failed to create OpenGL context!");
            glfwTerminate();
        }

        // Set icon
        AssetManager::LoadWindowIcon(mWindow, "Assets/Icons/Game Logo.png");
        mCursor = AssetManager::LoadWindowCursor("Assets/Icons/cursor_default.png");

        // Default setting
        glfwMakeContextCurrent(mWindow);
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursor(mWindow, mCursor);

        // Center window
        if (!mProps.mFullscreen)
        {
            if (!mProps.mMaximized)
            {
                mProps.mXpos = (mMonitorWidth - mProps.mWidth) / 2;
                mProps.mYpos = (mMonitorHeight - mProps.mHeight) / 2;
            }
            glfwSetWindowPos(mWindow, mProps.mXpos, mProps.mYpos);
        }

        EnableVsync(mProps.mVSync);

        // Set window size limites and aspect ratio
        glfwSetWindowSizeLimits(mWindow, DEFAULT_PROPERTIES.mWidth, DEFAULT_PROPERTIES.mHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);

        // Load OpenGL function pointers using GLAD
        int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        if (!status)
        {
            IS_CORE_CRITICAL("Failed to initialize GLAD!");
            glfwDestroyWindow(mWindow);
            glfwTerminate();
        }

        if (!GL_VERSION_4_5)
        {
            IS_CORE_ERROR("Driver doesn't support OpenGL 4.5 - abort program");
            BROADCAST_MESSAGE(MessageType::Quit);
            return;
        }

        IS_CORE_INFO("Driver supports OpenGL 4.5");
    }

    WindowSystem::~WindowSystem()
    {
        glfwDestroyWindow(mWindow);
        glfwTerminate();
        SaveProperties();
    }

    void WindowSystem::Initialize()
    {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);

    }

    void WindowSystem::Update(float)
    {
        //register window closing 
        if (glfwWindowShouldClose(mWindow)) {
            Message quit = Message(MessageType::Quit);
            BROADCAST_MESSAGE(quit);
        }
        // Disable window resize during runtime
        glfwSetWindowAttrib(mWindow, GLFW_RESIZABLE, InsightEngine::Instance().mRuntime ? GLFW_FALSE : GLFW_TRUE);
    }

    void WindowSystem::HandleMessage(const Message&) {}

    std::string WindowSystem::GetName() { return "Window"; }

    void WindowSystem::SwapBuffers() { glfwSwapBuffers(mWindow); }

    /*                                                                    Getters
    ----------------------------------------------------------------------------- */
    GLFWwindow* WindowSystem::GetNativeWindow() const           { return mWindow; }
    int WindowSystem::GetMonitorWidth() const                   { return mMonitorWidth; }
    int WindowSystem::GetMonitorHeight() const                  { return mMonitorHeight; }
    std::pair<int, int> WindowSystem::GetMonitorSize() const    { return std::make_pair(mMonitorWidth, mMonitorHeight); }
    std::string WindowSystem::GetWindowTitle() const            { return mProps.mTitle; }
    int WindowSystem::GetWindowXpos() const                     { return mProps.mXpos; }
    int WindowSystem::GetWindowYpos() const                     { return mProps.mYpos; }
    std::pair<int, int> WindowSystem::GetWindowPos() const      { return std::make_pair(mProps.mXpos, mProps.mYpos); }
    int WindowSystem::GetWidth() const                          { return mProps.mWidth; }
    int WindowSystem::GetHeight() const                         { return mProps.mHeight; }
    std::pair<int, int> WindowSystem::GetWindowSize() const     { return std::make_pair(mProps.mWidth, mProps.mHeight); }
    int WindowSystem::GetTargetFPS() const                      { return mProps.mTargetFPS; }
    int* WindowSystem::GetTargetFPS()                           { return &mProps.mTargetFPS; }
    bool WindowSystem::IsVSync() const                          { return mProps.mVSync; }
    bool WindowSystem::IsFullScreen() const                     { return mProps.mFullscreen; }

    /*                                                                    Setters
    ----------------------------------------------------------------------------- */
    void WindowSystem::SetWindowTitle(std::string const& title)
    {
        mProps.mTitle = title;
        glfwSetWindowTitle(mWindow, title.c_str());
    }

    void WindowSystem::SetWindowPos(int xpos, int ypos)
    {
        mProps.mXpos = xpos;
        mProps.mYpos = ypos;

        glfwSetWindowPos(mWindow, xpos, ypos);
    }

    void WindowSystem::SetWindowSize(int width, int height)
    {
        mProps.mWidth = width;
        mProps.mHeight = height;

        glfwSetWindowSize(mWindow, width, height);
    }

    void WindowSystem::EnableVsync(bool enabled)
    {
        mProps.mVSync = enabled;
        glfwSwapInterval(enabled ? 1 : 0);
    }

    void WindowSystem::SetFullScreen(bool fullscreen)
    {
        mProps.mFullscreen = fullscreen;
        glfwSetWindowAttrib(mWindow, GLFW_DECORATED, fullscreen ? GLFW_FALSE : GLFW_TRUE);
        glfwSetWindowAttrib(mWindow, GLFW_MAXIMIZED, mProps.mMaximized ? GLFW_TRUE : GLFW_FALSE);

        // Windowed mode
        if (!fullscreen)
        {
            if (!mProps.mMaximized)
            {
                mProps.mXpos = (mMonitorWidth - mWidthBeforeFullscreen) / 2;
                mProps.mYpos = (mMonitorHeight - mHeightBeforeFullscreen) / 2;
            }

            glfwSetWindowMonitor(mWindow, nullptr, mProps.mXpos, mProps.mYpos, mWidthBeforeFullscreen, mHeightBeforeFullscreen, GLFW_DONT_CARE);
            return;
        }

        mWidthBeforeFullscreen = mProps.mWidth;
        mHeightBeforeFullscreen = mProps.mHeight;
        // Borderless fullscreen window mode
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(mWindow, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    void WindowSystem::ShowMessageBox(std::string const& message, std::string const& title)
    {
        HWND hwnd = glfwGetWin32Window(mWindow);
        MessageBox(hwnd, message.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
    }

    int WindowSystem::GetTitleBarHeight()
    {
        return GetSystemMetrics(SM_CYCAPTION);
    }

    void WindowSystem::LoadCustomCursor()
    {
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursor(mWindow, mCursor);
    }

    void WindowSystem::SetMaximized(bool maximized)
    { 
        mProps.mMaximized = maximized;
        if (maximized)
            glfwMaximizeWindow(mWindow);
    }

    void WindowSystem::SetMinimized() { glfwIconifyWindow(mWindow); }

    /*                                                                 Properties
    ----------------------------------------------------------------------------- */
    void WindowSystem::LoadProperties()
    {
        std::string filename = "WindowProperties.json";
        Json::Value properties;
        
        // Load window properties from JSON file
        bool success = LoadJsonFromFile(properties, filename);
        if (auto const& win_props = properties["WindowProperties"];
            success && win_props["Title"].isString() &&
            win_props["Xpos"].isInt() && win_props["Ypos"].isInt() &&
            win_props["Width"].isInt() && win_props["Height"].isInt() && win_props["TargetFPS"] &&
            win_props["Vsync"].isBool() && win_props["Maximized"].isBool() && win_props["Fullscreen"].isBool())
        {
            mProps.mTitle      = win_props["Title"].asString();
            mProps.mXpos       = win_props["Xpos"].asInt();
            mProps.mYpos       = win_props["Ypos"].asInt();
            mProps.mWidth      = win_props["Width"].asInt();
            mProps.mHeight     = win_props["Height"].asInt();
            mProps.mTargetFPS  = win_props["TargetFPS"].asInt();
            mProps.mVSync      = win_props["Vsync"].asBool();
            mProps.mMaximized  = win_props["Maximized"].asBool();
            mProps.mFullscreen = win_props["Fullscreen"].asBool();

            IS_CORE_INFO("Loaded window properties from \"{}\"", filename);
        }
        else
        {
            // Assign default properties
            mProps = DEFAULT_PROPERTIES;
            IS_CORE_INFO("Using default window properties");
        }
    }

    void WindowSystem::SaveProperties()
    {
        if (mProps.mWidth == 0 || mProps.mHeight == 0)
            return;
        std::string filepath = "WindowProperties.json";
        Json::Value properties;
        auto& win_props = properties["WindowProperties"];
        win_props["Title"]      = mProps.mTitle;
        win_props["Xpos"]       = mProps.mXpos;
        win_props["Ypos"]       = mProps.mYpos;
        win_props["Width"]      = mProps.mWidth;
        win_props["Height"]     = mProps.mHeight;
        win_props["TargetFPS"]  = mProps.mTargetFPS;
        win_props["Vsync"]      = mProps.mVSync;
        win_props["Maximized"]  = mProps.mMaximized;
        win_props["Fullscreen"] = mProps.mFullscreen;

        // Save window propeties to JSON file
        bool success = SaveJsonToFile(properties, filepath);
        success ? IS_CORE_INFO("Successfully saved window properties to \"{}\"!", filepath) :
                  IS_CORE_ERROR("Failed to save window propeties to \"{}\"!", filepath);
    }

} // end namespace IS
