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
#include "Matrix3x3.h"
#include "Vector2D.h"

using namespace IS;

void PrintVector(char* txt, const Vec2& pVec0)
{
    printf("%s:\t%f, %f\n", txt, pVec0.x, pVec0.y);
}

void PrintMtx33(char* pString, Mtx33& pMtx)
{
    printf("%s:\n", pString);
    printf("%8.5f %8.5f %8.5f\n", pMtx.m00, pMtx.m01, pMtx.m02);
    printf("%8.5f %8.5f %8.5f\n", pMtx.m10, pMtx.m11, pMtx.m12);
    printf("%8.5f %8.5f %8.5f\n", pMtx.m20, pMtx.m21, pMtx.m22);
}

float CompareMtx33(const Mtx33& pSrc, const Mtx33& pDst)
{
    float d = 0.0f;

    for (long i = 0; i < 9; i++)
        d += (float)fabs((double)pSrc.m[i] - (double)pDst.m[i]);

    return d;
}

float CompareVec2(const Vec2& pSrc, const Vec2& pDst)
{
    float d = 0.0f;

    d += (float)fabs((double)pSrc.x - (double)pDst.x);
    d += (float)fabs((double)pSrc.y - (double)pDst.y);

    return d;
};


int main(void)
{
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

    // Before asking GLFW to create an OpenGL context, we specify the minimum constraints
    // in that context:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

    if (!window) {
        std::cerr << "GLFW unable to create OpenGL context - abort program\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    // this is the default setting ...
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Part 2: Initialize entry points to OpenGL functions and extensions
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Unable to initialize GLEW - error: "
            << glewGetErrorString(err) << " abort program" << std::endl;
        return false;
    }
    if (GLEW_VERSION_4_5) {
        std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
    }
    else {
        std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
        return false;
    }

    ISGraphics::init();
    //engine init
    IS::InsightEngine* engine= new IS::InsightEngine();

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //input manager assigned to window
    InputManager inputManager(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // grafix updates
        glfwPollEvents();
        ISGraphics::update();

        //grafix draws
        ISGraphics::draw();
        glfwSwapBuffers(window); // Swap front and back buffers

        //engine can run here
        //run engine
        
        //test for Input System will make it with held keys etc later
        if (inputManager.IsKeyPressed(GLFW_KEY_A)) {
            std::cout << "A IS PRESSED";
        }

        inputManager.Update();

        
    }
    engine->AddSystem(InputSys);
    engine->AddSystem(gwindow);
    //run engine (GAME LOOP)
    engine->SetFPS(80);//set fps to wtv
    engine->Run();

    //engine stops
    engine->DestroyAllSystems();

    glfwTerminate();
    return 0;
}