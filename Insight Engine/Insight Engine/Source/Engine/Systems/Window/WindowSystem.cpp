/*!
 * \file WindowSystem.cpp
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
           Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 26-09-2023
 * \brief
 * This source file defines the implementation for class WindowSystem,
 * which encapsulates the functionalities of an application window.
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
#include "WindowSystem.h"
#include "Engine/JSON/JsonSaveLoad.h"
#include "Engine/Systems/Asset/Asset.h"

#include <sstream>
#include <iomanip>
#pragma warning(push)
#pragma warning(disable: 4005) // redefine APIENTRY and IS_ERROR macros
#include <Windows.h>
#pragma warning(pop)
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace IS {

    WindowSystem::WindowSystem() : mWindow(nullptr), mCursor(nullptr)
    {
        // Initialize GLFW library
        bool success = glfwInit();
        if (!success)
        {
            IS_CORE_CRITICAL("Failed to to initialize GLFW libary!");
        }

        LoadProperties();
        PrintProperties();
        SetWindowHints();

        // Create a window and its OpenGL context
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            mWindow = mProps.fullscreen
                ? glfwCreateWindow(mode->width, mode->height, mProps.title.c_str(), monitor, nullptr)
                : glfwCreateWindow(mProps.width, mProps.height, mProps.title.c_str(), nullptr, nullptr);
        }

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
        UseCustomCursor();
        CenterWindow();
        EnableVsync(mProps.vsync);

        // Set window size limites and aspect ratio
        GLFWmonitor* monitor = GetActiveMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowSizeLimits(mWindow, DEFAULT_PROPERTIES.width, DEFAULT_PROPERTIES.height, mode->width, mode->height);

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
        PrintProperties();
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
    GLFWwindow* WindowSystem::GetNativeWindow() const               { return mWindow; }
    std::string WindowSystem::GetWindowTitle() const                { return mProps.title; }
    void WindowSystem::GetWindowSize(int& width, int& height) const { glfwGetWindowSize(mWindow, &width, &height); }
    int WindowSystem::GetWindowWidth() const
    {
        int width, height;
        GetWindowSize(width, height);
        return width;
    }
    int WindowSystem::GetWindowHeight() const
    {
        int width, height;
        GetWindowSize(width, height);
        return height;
    }
    int WindowSystem::GetTargetFPS() const                          { return mProps.fps; }
    int* WindowSystem::GetTargetFPS()                               { return &mProps.fps; }
    bool WindowSystem::IsVSync() const                              { return mProps.vsync; }
    bool WindowSystem::IsFullScreen() const                         { return mProps.fullscreen; }

    /*                                                                    Setters
    ----------------------------------------------------------------------------- */
    void WindowSystem::SetWindowTitle(std::string const& title)
    {
        mProps.title = title;
        glfwSetWindowTitle(mWindow, title.c_str());
    }

    void WindowSystem::SetWindowSize(int width, int height)
    {
        mProps.width = width;
        mProps.height = height;

        glfwSetWindowSize(mWindow, width, height);
    }

    void WindowSystem::EnableVsync(bool enabled)
    {
        mProps.vsync = enabled;
        glfwSwapInterval(enabled ? 1 : 0);
    }

    void WindowSystem::ToggleFullScreen()
    {
        mProps.fullscreen = !mProps.fullscreen;

        GLFWmonitor* monitor = GetActiveMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        if (!mProps.fullscreen)
        {
            if (mProps.maximized)
            {
                glfwSetWindowMonitor(mWindow, nullptr, mPreviousX, mPreviousY, mPreviousWidth, mPreviousHeight, 0);
                glfwMaximizeWindow(mWindow);
                return;
            }

            int monitorX, monitorY, monitorWidth, monitorHeight;
            glfwGetMonitorWorkarea(monitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);

            int xpos = monitorX + (monitorWidth - mProps.width) / 2;
            int ypos = monitorY + (monitorHeight - mProps.height) / 2;

            glfwSetWindowMonitor(mWindow, nullptr, xpos, ypos, mProps.width, mProps.height, 0);
            return;
        }

        // Store the window size and position before going fullscreen
        StorePreviousWindowData();
        glfwSetWindowMonitor(mWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
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

    void WindowSystem::UseCustomCursor()
    {
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursor(mWindow, mCursor);
    }

    void WindowSystem::UseDefaultCursor()
    {
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void WindowSystem::HideCursor()
    {
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    void WindowSystem::ShowCursor()
    {
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    /*                                                                 Properties
    ----------------------------------------------------------------------------- */
    void WindowSystem::LoadProperties()
    {
        mIsFocused = true;
        mIsMinimized = false;

        std::string filename = "WindowProperties.json";
        Json::Value properties;
        
        // Load window properties from JSON file
        bool success = LoadJsonFromFile(properties, filename);
        if (auto const& win_props = properties["WindowProperties"];
            success && win_props["Title"].isString() &&
            win_props["Width"].isInt() && win_props["Height"].isInt() && win_props["TargetFPS"] &&
            win_props["Vsync"].isBool() && win_props["Maximized"].isBool() && win_props["Fullscreen"].isBool())
        {
            mProps.width = win_props["Width"].asInt();
            mProps.height = win_props["Height"].asInt();

            // Validate window size
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            if (mProps.width < DEFAULT_PROPERTIES.width || mProps.width > mode->width ||
                mProps.height < DEFAULT_PROPERTIES.height || mProps.height > mode->height)
            {
                mProps = DEFAULT_PROPERTIES;
                IS_CORE_INFO("Loaded window properties from \"{}\"", filename);
                return;
            }

            mProps.title      = win_props["Title"].asString();
            mProps.fps        = win_props["TargetFPS"].asInt();
            mProps.vsync      = win_props["Vsync"].asBool();
            mProps.maximized  = win_props["Maximized"].asBool();
            mProps.fullscreen = win_props["Fullscreen"].asBool();
            if (mProps.fullscreen) mProps.maximized = false;

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
        if (mProps.width == 0 || mProps.height == 0)
            return;
        std::string filepath = "WindowProperties.json";
        Json::Value properties;
        auto& win_props = properties["WindowProperties"];
        win_props["Title"]      = mProps.title;
        win_props["Width"]      = mProps.width;
        win_props["Height"]     = mProps.height;
        win_props["TargetFPS"]  = mProps.fps;
        win_props["Vsync"]      = mProps.vsync;
        win_props["Fullscreen"] = mProps.fullscreen;
        win_props["Maximized"]  = mProps.fullscreen ? false : mProps.maximized;

        // Save window propeties to JSON file
        bool success = SaveJsonToFile(properties, filepath);
        success ? IS_CORE_INFO("Successfully saved window properties to \"{}\"!", filepath) :
                  IS_CORE_ERROR("Failed to save window propeties to \"{}\"!", filepath);
    }

    void WindowSystem::PrintProperties()
    {
        std::ostringstream oss;
        oss << "Window Properties:\n"
            << "------------------------------\n"
            << std::left 
            << std::setw(12) << "Property" << "| Value\n"
            << "------------------------------\n"
            << std::setw(12) << "Title:"      << "| " << mProps.title << "\n"
            << std::setw(12) << "Width:"      << "| " << mProps.width << "\n"
            << std::setw(12) << "Height:"     << "| " << mProps.height << "\n"
            << std::setw(12) << "Target FPS:" << "| " << mProps.fps << "\n"
            << std::setw(12) << "Vsync:"      << "| " << (mProps.vsync ? "On" : "Off") << "\n"
            << std::setw(12) << "Maximized:"  << "| " << (mProps.maximized ? "Yes" : "No") << "\n"
            << std::setw(12) << "Fullscreen:" << "| " << (mProps.fullscreen ? "Yes" : "No") << "\n"
            << "------------------------------";

        IS_CORE_INFO(oss.str().c_str());
    }

    void WindowSystem::SetWindowHints()
    {
        // Specify minimum constraints in OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, mProps.maximized ? GLFW_TRUE : GLFW_FALSE);
    }

    void WindowSystem::CenterWindow()
    {
        if (mProps.fullscreen || mProps.maximized) return;

        GLFWmonitor* monitor = GetActiveMonitor();

        int monitor_x, monitor_y, monitor_width, monitor_height;
        glfwGetMonitorWorkarea(monitor, &monitor_x, &monitor_y, &monitor_width, &monitor_height);

        int xpos = monitor_x = (monitor_width - mProps.width) / 2;
        int ypos = monitor_y = (monitor_height - mProps.height) / 2;

        glfwSetWindowPos(mWindow, xpos, ypos);
    }

    void WindowSystem::StorePreviousWindowData()
    {
        glfwGetWindowSize(mWindow, &mPreviousWidth, &mPreviousHeight);
        glfwGetWindowPos(mWindow, &mPreviousX, &mPreviousY);
    }

    GLFWmonitor* WindowSystem::GetActiveMonitor()
    {
        int window_x, window_y;
        glfwGetWindowPos(mWindow, &window_x, &window_y);

        int monitorsLength;
        GLFWmonitor** monitors = glfwGetMonitors(&monitorsLength);

        for (int i = 0; i < monitorsLength; i++)
        {
            int monitorX, monitorY, monitorWidth, monitorHeight;
            glfwGetMonitorWorkarea(monitors[i], &monitorX, &monitorY, &monitorWidth, &monitorHeight);

            if (window_x >= monitorX && window_x < monitorX + monitorWidth &&
                window_y >= monitorY && window_y < monitorY + monitorHeight)
            {
                return monitors[i];
            }
        }

        return nullptr;
    }

} // end namespace IS
