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
            uint32_t mWidth{}; ///< The width of the window.
            uint32_t mHeight{}; ///< The height of the window.
            bool mVSync{}; ///< Flag indicating whether vsync is enabled.
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

        /*!
         * \brief Gets the width of the window.
         * \return The width of the window.
         */
        uint32_t GetWidth() const;

        /*!
         * \brief Gets the height of the window.
         * \return The height of the window.
         */
        uint32_t GetHeight() const;

        /*!
         * \brief Gets the size of the window.
         * \return A pair containing the width and height of the window.
         */
        std::pair<uint32_t, uint32_t> GetWindowSize() const;

        /*!
         * \brief Gets the title of the window.
         * \return The title of the window.
         */
        std::string GetWindowTitle() const;

        /*!
         * \brief Sets the size of the window.
         * \param width The new width of the window.
         * \param height The new height of the window.
         */
        void SetWindowSize(uint32_t width, uint32_t height);

        /*!
         * \brief Sets the title of the window.
         * \param title The new title of the window.
         */
        void SetWindowTitle(std::string const& title);

        /*!
         * \brief Enables or disables vsync for the window.
         * \param enabled Flag indicating whether to enable vsync.
         */
        void EnableVsync(bool enabled);

        /*!
         * \brief Checks if vsync is enabled for the window.
         * \return True if vsync is enabled, false otherwise.
         */
        bool IsVSync() const;

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

        static WindowProperties mDefaultProperties; ///< Default window properties.
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_ENGINE_WINDOW_WINDOW_SYSTEM_H
