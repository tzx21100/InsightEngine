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
#include "TestComponents.h"

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



int main() {

    // Initialize log
    Log::init();

    //engine get
    InsightEngine& engine = InsightEngine::Instance();

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


    //The following demonstrates the Initialize() Part of your systems
    
    //this is just to show how the new system works everthing can be deleted
    //this is 2 random components
    struct Position {
        Vector2D x, y;
    };
    struct Velocity {
        Vector2D x, y;
    };
    //register the position component
    engine.RegisterComponent<Position>();
    engine.RegisterComponent<Velocity>();
    //you can now create entities
    Entity newEntity = engine.CreateEntityWithComponents<Position>();
    //destroy entities
    if (!engine.HasComponent<Position>(newEntity)) {
        engine.DestroyEntity(newEntity);
    }
    else {
        std::cout << "TYPE DETECTED";
    }

    /* adding components to the systems
       your system is going to be made out of different components
       these components will add together to give you a signature
       we are using shared pointers for the system */
    auto InputSys = std::make_shared<InputManager>(window);
    //this function will give you the signature value (basically what your system contains)
    Signature signatureValue = engine.GenerateSignature<Position, Velocity>();
    //this function will let you add the value inside
    engine.AddSystem(InputSys, signatureValue);


    // This is the end of the Initialize() part of your systems

    /*
    I will now demostrate how a Update loop will be 
    eg. This is in a physics system's update loop
    Update( float deltatime ) {
        //loops through all Entities registered by the System this mEntities map is written in the parent class
        for (auto const& entity : mEntities) {
            //if the entity has a certain component do something with it
            if (InsightEngine::Instance().HasComponent<Position>(entity)) {
                //do something;
            }
            if (InsightEngine::Instance().HasComponent<Velocity>(entity)) {
                //do something
            }
        }

    }

    Of course, you can do something like using InsightEngine& Engine= InsightEngine::Instance() to make life easier.
    Then it will just be Engine.HasComponent<Position>(entity){
    
    }
    */

    Signature signature;
    auto mySystem = std::make_shared<glfxWindow>(window);
    engine.AddSystem(mySystem,signature);
    auto mySystem2= std::make_shared<InputManager>(window);
    engine.AddSystem(mySystem2,signature);
    auto mySystem3 = std::make_shared<ISAudio>();
    engine.AddSystem(mySystem3,signature);
    

    //run engine (GAME LOOP)
    engine.SetFPS(80);//set fps to wtv
    engine.Run();
   
    //engine stops technically don't need this since destructor is there and my engine is static
    engine.DestroyAllSystems();

    glfwTerminate();
   

    return 0;
}