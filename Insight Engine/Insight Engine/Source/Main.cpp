///////////////////////////////////////////////////////////////////////////////////////
//
//	Main
//	The main entry point for the game--everything starts here.
//	
//	Authors:  Tan Zheng Xun
//	Copyright 2023, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
//pch has to go to the top of every cpp
#include "Pch.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WIDTH 1280
#define HEIGHT 720


#include "Graphics.h"
#include "Input.h"
#include "CoreEngine.h"
#include "GLFXWindow.h"

using namespace IS;

int main() {
    // Initialize log
    Log::init();

    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfxWindow* gwindow=new glfxWindow(window);

    //engine get
    InsightEngine& engine = InsightEngine::Instance();

    //input manager assigned to window
    InputManager* InputSys=new InputManager(window);
    ISAudio* AudioSys = new ISAudio();
   
    engine.AddSystem(InputSys);
    engine.AddSystem(gwindow);
    engine.AddSystem(AudioSys);

    //run engine (GAME LOOP)
    engine.SetFPS(80);//set fps to wtv
    engine.Run();
   
    //engine stops technically don't need this since destructor is there and my engine is static
    engine.DestroyAllSystems();

    glfwTerminate();
   

    return 0;
}