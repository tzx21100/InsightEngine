/*!
 * \file WindowSystem.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
           Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 *      This header file declares the interface for class WindowSystem, which
 *      encapsulates the functionalities of an application window.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ENGINE_WINDOW_WINDOW_SYSTEM_H
#define GAM200_INSIGHT_ENGINE_SOURCE_ENGINE_WINDOW_WINDOW_SYSTEM_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "System.h"

// Dependencies
#include <glad/glad.h> // ensure glad is included before glfw
#include <GLFW/glfw3.h>

namespace IS {

    /*!
     * \brief The WindowSystem class manages application windows.
     *
     * \brief WindowSystem is a subclass of ParentSystem and handles window-related functionality.
     */
    class WindowSystem : public ParentSystem {
    public:
        /*!
         * \brief Structure encapsulating properties of a window.
         */
        struct WindowProperties {
            std::string mTitle; ///< The title of the window.
            int mXpos{}; ///< The x-position of the window.
            int mYpos{}; ///< The y-position of the window.
            int mWidth{}; ///< The width of the window.
            int mHeight{}; ///< The height of the window.
            int mTargetFPS{}; ///< The target FPS of the window.
            bool mVSync{}; ///< Flag indicating whether vsync is enabled.
            bool mMaximized{}; ///< Flag indicating whether window is maximzed.
            bool mFullscreen{}; ///< Flag indicating whether window is fullscreen.

            WindowProperties() = default;
            WindowProperties(std::string const& title, int width, int height, int fps, bool vsync = true, bool maximized = false, bool fullscreen = false)
                : mTitle(title), mXpos(), mYpos(), mWidth(width), mHeight(height), mTargetFPS(fps), mVSync(vsync), mMaximized(maximized), mFullscreen(fullscreen) {}
        };

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
         * \brief Gets the width of the monitor.
         * \return The width of the monitor.
         */
        int GetMonitorWidth() const;

        /*!
         * \brief Gets the height of the monitor.
         * \return The height of the monitor.
         */
        int GetMonitorHeight() const;

        /*!
         * \brief Gets the size of the monitor.
         * \return A pair containing the width and height of the monitor.
         */
        std::pair<int, int> GetMonitorSize() const;

        /*!
         * \brief Gets the title of the window.
         * \return The title of the window.
         */
        std::string GetWindowTitle() const;

        /*!
         * \brief Gets the xpos of the window.
         * \return The xpos of the window.
         */
        int GetWindowXpos() const;

        /*!
         * \brief Gets the ypos of the window.
         * \return The ypos of the window.
         */
        int GetWindowYpos() const;

        /*!
         * \brief Gets the position of the window.
         * \return The position of the window.
         */
        std::pair<int, int> GetWindowPos() const;

        /*!
         * \brief Gets the width of the window.
         * \return The width of the window.
         */
        int GetWidth() const;

        /*!
         * \brief Gets the height of the window.
         * \return The height of the window.
         */
        int GetHeight() const;

        /*!
         * \brief Gets the size of the window.
         * \return A pair containing the width and height of the window.
         */
        std::pair<int, int> GetWindowSize() const;

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
         * \brief Sets the position of the window.
         * \param xpos The new xpos of the window.
         * \param ypos The new ypos of the window.
         */
        void SetWindowPos(int xpos, int ypos);

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
         * \brief Set window to maximized.
         * \param maximized if true, normal otherwise. (default true)
         */
        void SetMaximized(bool maximized = true);

        /*!
         * \brief Set window to minimized.
         */
        void SetMinimized();

        /*!
         * \brief Set window to fullscreen/windowed
         * \param fullscreen fullscreen if true, windowed otherwise. (default true)
         */
        void SetFullScreen(bool fullscreen = true);

        /*!
         * \brief Loads window properties.
         */
        void LoadProperties();

        /*!
         * \brief Saves window properties.
         */
        void SaveProperties();

    private:
        GLFWwindow* mWindow = nullptr; ///< Pointer to the GLFW window.
        WindowProperties mProps; ///< The properties of the window.
        int mMonitorWidth; ///< The width of the monitor.
        int mMonitorHeight; ///< The height of the monitor.
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_ENGINE_WINDOW_WINDOW_SYSTEM_H
