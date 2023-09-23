/*
I'm creating a Input class to abstract from GLFW so that its easier to call 
and check for keyboard & mouse inputs.
*/

#ifndef GAM200_INSIGHT_ENGINE_SOURCE_INPUT_H
#define GAM200_INSIGHT_ENGINE_SOURCE_INPUT_H

#include <GLFW/glfw3.h>
#include <utility>
#include <unordered_set>
#include "System.h"

namespace IS {

    class InputAffector  {

        /*this is just an empty component
        * all the entities that have this will be called
         */

    };



    class InputManager :public ParentSystem{
    public:
        //override parent
        void Update(float deltaTime) override;
        std::string GetName() override;
        void Initialize() override;
        void HandleMessage(const Message& message) override;

        InputManager();

        bool IsKeyPressed(int glfwKeyCode) const;
        bool IsKeyReleased(int glfwKeyCode) const;
        bool IsKeyHeld(int glfwKeyCode) const;

        bool IsMouseButtonPressed(int button) const;
        bool IsMouseButtonReleased(int button) const;
        bool IsMouseButtonHeld(int button) const;

        std::pair<double, double> GetMousePosition() const;

        //window
        GLFWwindow* window;

        void setCenterPos(float posx, float posy) {
            center_x = posx;
            center_y = posy;
        }
        void setRatio(float widthR, float heightR) {
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            ratio_width =  (float)width / widthR;
            ratio_height =  (float)height / heightR;
        }

    private:

        //for keyboard keys
        std::unordered_set<int> pressed_keys;
        std::unordered_set<int> released_keys;
        std::unordered_set<int> held_keys;
        //for mouse button
        std::unordered_set<int> pressed_mouse_buttons;
        std::unordered_set<int> released_mouse_buttons;
        std::unordered_set<int> held_mouse_buttons;


        float center_x = 0.f;
        float center_y = 0.f;
        float ratio_width = 1.f;
        float ratio_height = 1.f;
        // GLFW key callback function
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        //GLFW mouse callback function
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    };

}
//didn't include a destructor because theres nothing to free.
//will make this a child of the System class once its done

#endif // GAM200_INSIGHT_ENGINE_SOURCE_INPUT_H  