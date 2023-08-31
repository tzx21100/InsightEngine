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
#include "CoreEngine.h"
#include "GLFXWindow.h"

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
    glfxWindow* gwindow=new glfxWindow(window);
    gwindow->Initialize();

    //engine init
    IS::InsightEngine engine;

    //input manager assigned to window
    InputManager* InputSys=new InputManager(window);

    engine.AddSystem(InputSys);
    engine.AddSystem(gwindow);
    //run engine (GAME LOOP)
    engine.SetFPS(80);//set fps to wtv
    engine.Run();

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