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
        std::string getName() override;
        void Initialize() override;
        void HandleMessage(const Message& message) override;

        InputManager();

        bool IsKeyPressed(int glfwKeyCode) const;
        bool IsKeyReleased(int glfwKeyCode) const;
        bool IsKeyHeld(int glfwKeyCode) const;

        bool IsMouseButtonPressed(int button) const;
        std::pair<double, double> GetMousePosition() const;

        //window
        GLFWwindow* window;

        //this is to instatiate only ONE engine
        static InputManager& Instance() {
            static InputManager instance;
            return instance;
        }

    private:
        std::unordered_set<int> pressed_keys;
        std::unordered_set<int> released_keys;

        //this is to store held keys and make it unique
        std::unordered_set<int> held_keys;

        // GLFW key callback function
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    };

}
//didn't include a destructor because theres nothing to free.
//will make this a child of the System class once its done

#endif // GAM200_INSIGHT_ENGINE_SOURCE_INPUT_H  