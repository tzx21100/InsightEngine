/*!
 * \file Input.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu,
 *         Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This header file declares the input that abstracts from GLFW to create its
 * own inputs from keyboard and mouse. It creates an input System that can
 * be used by the developers.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_ENGINE_SYSTEMS_INPUT_H
#define GAM200_INSIGHT_ENGINE_ENGINE_SYSTEMS_INPUT_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Engine/Core/CoreEngine.h"
#include "Engine/ECS/System.h"
#include "Engine/Systems/Window/WindowSystem.h"
#include <utility>
#include <unordered_set>
#include <GLFW/glfw3.h>

namespace IS {

    class InputAffector :public IComponent {
    public:
        static std::string GetType() {
            return "InputAffector";
        }
        /*this is just an empty component
        * all the entities that have this will be called
         */

    };

    class InputManager : public ParentSystem{
    public:

        /**
         * \brief Get monitor resolution
         *
         * \return A std::pair of the resolution
         */
        std::pair<int, int> GetMonitorResolution() {
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            if (primaryMonitor == nullptr) {
                // Handle the error, maybe the monitor is not detected or GLFW is not initialized
            }

            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
            if (mode == nullptr) {
                // Handle the error, maybe the video mode is not available
            }

            int monitorWidth = mode->width;
            int monitorHeight = mode->height;

            // Now you have the monitor's resolution in monitorWidth and monitorHeight
            return std::pair<int, int>(monitorWidth, monitorHeight);
        }

        /**
         * \brief Returns the name of the InputManager system.
         *
         * \return A string representing the name of this system.
         */
        std::string GetName() override;

        /**
         * \brief Initialization logic for the InputManager.
         *
         * Currently subscribes to `DebugInfo` messages.
         */
        void Initialize() override;

        /**
         * \brief Updates the state of pressed, held, and released keys/buttons.
         *
         * Should be called once per frame.
         */
        void Update(float deltaTime) override;

        /**
         * \brief Handles incoming messages.
         *
         * Defines behavior based on the type of message received.
         */
        void HandleMessage(const Message& message) override;

        /**
         * \brief Constructor for InputManager. Sets up GLFW callbacks.
         * \param window Pointer to the window system.
         */
        InputManager(std::shared_ptr<WindowSystem> window);

        /**
         * \brief Polls for events from GLFW.
         */
        void PollEvents();

        /**
         * \brief Checks if a key is currently triggered.
         */
        bool IsKeyPressed(int glfwKeyCode) const;

        /**
         * \brief Checks if a key was just released.
         */
        bool IsKeyReleased(int glfwKeyCode) const;

        /**
         * \brief Checks if a key is currently held down.
         */
        bool IsKeyHeld(int glfwKeyCode) const;

        /**
         * \brief Checks if a mouse button is currently triggered.
         */
        bool IsMouseButtonPressed(int button) const;

        /**
         * \brief Checks if a mouse button was just released.
         */
        bool IsMouseButtonReleased(int button) const;

        /**
         * \brief Checks if a mouse button is currently held down.
         */
        bool IsMouseButtonHeld(int button) const;

        /**
         * \brief Sets the center world position for the mouse relative to the GLFW window
         */
        void setCenterPos(float posx, float posy) {
            center_x = posx;
            center_y = posy;
        }

        /**
         * \brief Sets the ratio of the 'screen' the mouse is in relative to the window
         */
        void setRatio(float widthR, float heightR)
        {
            InsightEngine& engine = InsightEngine::Instance();
            //auto const& window_sys = engine.GetSystem<WindowSystem>("Window");
            int width, height;
            engine.GetWindowSize(width, height);
            //glfwGetWindowSize(window, &width, &height);
            // ratio_width =  (float)width / widthR;
            // ratio_height =  (float)height / heightR;

            ratio_width = widthR / static_cast<float>(width);
            if (engine.IsFullScreen()) ratio_height = heightR / (static_cast<float>(height));
            else ratio_height = heightR / (static_cast<float>(height) + 32.f); // 32.f for the white title bar
        }

        double GetHeldDuration(int glfw_keycode) const
        {
            if (held_keys.count(glfw_keycode) > 0)
            {
                double held_duration = glfwGetTime() - key_pressed_time.at(glfw_keycode);
                return held_duration;
            }
            return 0.0; // Key not currently held
        }

        // for mouse dragging testing
        Vec2D previousWorldMousePos{ 0.0,0.0 };
        Vec2D currentWorldMousePos{ 0.0,0.0 };

    private:
        // window
        std::shared_ptr<WindowSystem> mWindow;

        //for keyboard keys
        std::unordered_set<int> pressed_keys;
        std::unordered_set<int> released_keys;
        std::unordered_set<int> held_keys;
        //for mouse button
        std::unordered_set<int> pressed_mouse_buttons;
        std::unordered_set<int> released_mouse_buttons;
        std::unordered_set<int> held_mouse_buttons;
        std::unordered_map<int, double> key_pressed_time; // Map to store the press time of keys

        //for file drag/drop from file explorer
        std::unordered_set<std::filesystem::path> payloads;

        // variables to calculate and translate the mouse position relative to the GLFW window
        float center_x = 0.f;
        float center_y = 0.f;
        float ratio_width = 1.f;
        float ratio_height = 1.f;

        /**
         * \brief Process payloads from windows file explorer.
         */
        void ProcessPayloads();

        /**
         * \brief Process directory payloads.
         * 
         * \param directory The path to the directory.
         */
        void ProcessPayloadDirectory(std::filesystem::path const& directory);

        /**
         * \brief Process file payloads.
         *
         * \param filepath The path to the file.
         */
        void ProcessPayloadFile(std::filesystem::path const& filepath);

        /**
         * \brief GLFW callback for key events.
         *
         * Updates the state of pressed, held, and released keys.
         */
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        /**
        * \brief GLFW callback for mouse button events.
        *
        * Updates the state of pressed, held, and released mouse buttons.
        */
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        /**
         * \brief GLFW callback for file drop events.
         *
         * \param window Pointer to window context.
         * \param count Number of files dropped.
         * \param paths Array containing the filepaths.
         */
        static void FileDropCallback(GLFWwindow* window, int count, const char** paths);
    };

}
//didn't include a destructor because theres nothing to free.

#endif // GAM200_INSIGHT_ENGINE_SOURCE_INPUT_H
