/*!
 * \file Input.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 *         Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This header file declares the input that abstracts from GLFW to create its
 * own inputs from keyboard and mouse. It creates an input System that can
 * be used by the developers.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
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
#include "System.h"
#include "WindowSystem.h"
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
         * \brief Gets the current mouse position in world coordinates.
         *
         * \return A pair representing the x and y coordinates of the mouse cursor.
         */
        std::pair<double, double> GetMousePosition();

        std::pair<double, double> GetPreviousMousePosition() const;

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
        void setRatio(float widthR, float heightR) {
            InsightEngine& engine = InsightEngine::Instance();
            auto const& window_sys = engine.GetSystem<WindowSystem>("Window");
            auto [width, height] = window_sys->GetWindowSize();
            //glfwGetWindowSize(window, &width, &height);
            ratio_width =  (float)width / widthR;
            ratio_height =  (float)height / heightR;
        }
        void PrintMouseWorldPos();
        void mouse_pick_entity();

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

        std::unordered_set<std::filesystem::path> payloads;

        // variables to calculate and translate the mouse position relative to the GLFW window
        float center_x = 0.f;
        float center_y = 0.f;
        float ratio_width = 1.f;
        float ratio_height = 1.f;

        double world_mouse_x = 0.0;
        double world_mouse_y = 0.0;
        double current_mouse_x = 0.0;
        double current_mouse_y = 0.0;
        double previous_mouse_x = 0.0;
        double previous_mouse_y = 0.0;

        void ProcessPayloads();

        void ProcessPayloadDirectory(std::filesystem::path const& directory);
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

        static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);

        static void FileDropCallback(GLFWwindow* window, int count, const char** paths);
    };

}
//didn't include a destructor because theres nothing to free.

#endif // GAM200_INSIGHT_ENGINE_SOURCE_INPUT_H  