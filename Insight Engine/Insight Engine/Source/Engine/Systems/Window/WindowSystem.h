/*!
 * \file WindowSystem.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
           Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 26-09-2023
 * \brief
 * This header file declares the interface for class WindowSystem, which
 * encapsulates the functionalities of an application window.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_ENGINE_SYSTEMS_WINDOW_WINDOW_SYSTEM_H
#define GAM200_INSIGHT_ENGINE_ENGINE_SYSTEMS_WINDOW_WINDOW_SYSTEM_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Engine/ECS/System.h"

// Dependencies
#include <glad/glad.h> // ensure glad is included before glfw
#include <GLFW/glfw3.h>

namespace IS {

    /*!
     * \brief Structure encapsulating properties of a window.
     */
    struct WindowProperties
    {
        std::string title;
        int width;
        int height;
        int fps;
        bool vsync;
        bool maximized;
        bool fullscreen;
    };

    // In case "WindowProperties.json" is not found, window will use default properties
#ifdef USING_IMGUI
    static const WindowProperties DEFAULT_PROPERTIES{ "Insight Engine", 1600, 900, 60, false, false, false };
#else
    static const WindowProperties DEFAULT_PROPERTIES{ "Fragments", 1600, 900, 60, false, false, true };
#endif

    /*!
     * \brief The WindowSystem class manages application windows.
     *
     * \brief WindowSystem is a subclass of ParentSystem and handles window-related functionality.
     */
    class WindowSystem : public ParentSystem {
    public:
        /*!
         * \brief Constructs a WindowSystem object.
         */
        WindowSystem();

        /*!
         * \brief Destroys the WindowSystem object.
         */
        ~WindowSystem();

        /*!
         * \brief Initializes the WindowSystem.
         */
        void Initialize() override;

        /*!
         * \brief Updates the WindowSystem.
         * \param deltaTime The time difference since the last update.
         */
        void Update(float deltaTime) override;

        /*!
         * \brief Handles a message.
         * \param message The message to be handled.
         */
        void HandleMessage(const Message& message) override;

        /*!
         * \brief Gets the name of the WindowSystem.
         * \return The name of the WindowSystem.
         */
        std::string GetName() override;

        /*!
         * \brief Swaps the front and back buffers.
         */
        void SwapBuffers();

        /*|
         * \brief Gets the native GLFW window.
         * \return pointer to native window.
         */
        GLFWwindow* GetNativeWindow() const;

        /*!
         * \brief Gets the title of the window.
         * \return The title of the window.
         */
        std::string GetWindowTitle() const;

        /*!
         * \brief Gets the size of the window.
         * \param width The width of the window.
         * \param height The height of the window.
         */
        void GetWindowSize(int& width, int& height) const;

        /*!
         * \brief Gets the width of the window.
         */
        int GetWindowWidth() const;

        /*!
         * \brief Gets the height of the window.
         */
        int GetWindowHeight() const;

        int GetMonitorWidth();
        int GetMonitorHeight();

        /*!
         * \brief Gets the target FPS of the window.
         * \return The target FPS of the window.
         */
        int GetTargetFPS() const;

        /*!
         * \brief Gets the reference to target FPS of the window.
         * \return The the reference to target FPS of the window.
         */
        int* GetTargetFPS();

        /*!
         * \brief Checks if vsync is enabled for the window.
         * \return True if vsync is enabled, false otherwise.
         */
        bool IsVSync() const;

        /*!
         * \brief Checks if window is fullscreen.
         * \return True if window is fullscreen, false otherwise.
         */
        bool IsFullScreen() const;

        /*!
         * \brief Sets the title of the window.
         * \param title The new title of the window.
         */
        void SetWindowTitle(std::string const& title);

        /*!
         * \brief Sets the size of the window.
         * \param width The new width of the window.
         * \param height The new height of the window.
         */
        void SetWindowSize(int width, int height);

        /*!
         * \brief Enables or disables vsync for the window.
         * \param enabled Flag indicating whether to enable vsync. (default true)
         */
        void EnableVsync(bool enabled = true);

        /*!
         * \brief Toggles fullscreen mode for the window.
         */
        void ToggleFullScreen();

        /*!
         * \brief Checks if window is focused.
         * \return Boolean flag indicating if window is focused.
         */
        bool IsFocused() const { return mIsFocused; }

        /*!
         * \brief Checks if window is minimized.
         * \return Boolean flag indicating if window is minimized.
         */
        bool IsMinimized() const { return mIsMinimized; }

        /*!
         * \brief Show a pop-up message box using Win32 API.
         * \param message Message to be displayed.
         * \param title Title of the window.
         */
        int ShowMessageBox(std::string const& message, std::string const& title = "Message Box");

        /*!
         * \brief Get the height of the title bar of the window.
         * \return The height of the title bar of the window.
         */
        int GetTitleBarHeight();

        /*!
         * \brief Use the custom cursor for the window.
         */
        void UseCustomCursor();

        /*!
         * \brief Use default cursor for the window.
         */
        void UseDefaultCursor();

        /*!
         * \brief Hide the cursor for the window.
         */
        void HideCursor();

        /*!
         * \brief Show the cursor for the window.
         */
        void ShowCursor();

    private:
        GLFWwindow* mWindow; ///< Pointer to the GLFW window.
        GLFWcursor* mCursor; ///< Pointer to the GLFW cursor.
        WindowProperties mProps; ///< The properties of the window.
        bool mIsFocused; ///<  Boolean flag indicating if window is focused.
        bool mIsMinimized; ///< Boolean flag indicating if window is minimized.
        int mPreviousX{}; ///< Previous x-coordinate of the window.
        int mPreviousY{}; ///< Previous y-coordinate of the window.
        int mPreviousWidth{}; ///< Previous width of the window.
        int mPreviousHeight{}; ///< Previous height of the window;

        /*!
         * \brief Loads window properties.
         */
        void LoadProperties();

        /*!
         * \brief Saves window properties.
         */
        void SaveProperties();

        /*!
         * \brief Prints window properties.
         */
        void PrintProperties();

        /*!
         * \brief Set window hints for GLFW.
         */
        void SetWindowHints();

        /*!
         * \brief Center the window on the screen.
         */
        void CenterWindow();

        /*!
         * \brief Store previous window data before fullscreen and minimize.
         */
        void StorePreviousWindowData();

        /*!
         * \brief Get the active monitor of the window.
         */
        GLFWmonitor* GetActiveMonitor();

        friend class InputManager; // InputManager needs to access mWindow
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_ENGINE_WINDOW_WINDOW_SYSTEM_H
