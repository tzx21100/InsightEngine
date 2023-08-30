/*
I'm creating a Input class to abstract from GLFW so that its easier to call 
and check for keyboard & mouse inputs.
*/

#ifndef GAM200_INSIGHT_ENGINE_SOURCE_INPUT_H
#define GAM200_INSIGHT_ENGINE_SOURCE_INPUT_H

#include <GLFW/glfw3.h>
#include <utility>
#include <unordered_set>

class InputManager {
public:
    InputManager(GLFWwindow* window);

    bool IsKeyPressed(int glfwKeyCode) const;
    bool IsKeyReleased(int glfwKeyCode) const;
    bool IsKeyHeld(int glfwKeyCode) const;

    bool IsMouseButtonPressed(int button) const;
    std::pair<double, double> GetMousePosition() const;

    void Update(); // Call this once per frame to update key states

private:
    GLFWwindow* m_Window;
    std::unordered_set<int> m_PressedKeys;
    std::unordered_set<int> m_ReleasedKeys;

    // GLFW key callback function
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

};
//didn't include a destructor because theres nothing to free.
//will make this a child of the System class once its done

#endif // GAM200_INSIGHT_ENGINE_SOURCE_INPUT_H  