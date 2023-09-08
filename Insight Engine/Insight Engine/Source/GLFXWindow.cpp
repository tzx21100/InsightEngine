#include "Pch.h"
#include "GLFXWindow.h"

namespace IS {

    //this will be the update for the window we can use this to register like people pressing stuff on the window
    void glfxWindow::Update(float time)  {
        ISGraphics::update(time);

        //grafix draws
        ISGraphics::draw();

        //register window closing 
        if (glfwWindowShouldClose(window)) {
            Message quit = Message(MessageType::Quit);
            EventManager::Instance().Broadcast(quit);
        }
    }

    std::string glfxWindow::getName()  {
        return "GLFWWindowSystem";
    }

    void glfxWindow::Initialize()  {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);
        ISGraphics::init();
    }

    void glfxWindow::HandleMessage(const Message& message){
        if (message.GetType() == MessageType::Collide) {
            // Handle collision logic here
            IS_CORE_INFO("Handling collision in PhysicsSystem.");
        }
    }

    glfxWindow::glfxWindow(int new_width, int new_height, std::string const& new_title)
        : width(new_width), height(new_height), title(new_title) {
        // Initialize GLFW library
        IS_CORE_ASSERT_MESG(glfwInit(), "Failed to to initialize libary!");

        // Specify minimum constraints in OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

        // Create a windowed mode window and its OpenGL context
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window) {
            IS_CORE_ERROR("Failed to create OpneGL context!");
            glfwTerminate();
        }

        // Make the current window's context
        glfwMakeContextCurrent(window);

        // Default setting
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // Initialize entry points to OpenGL functions and extensions        
        if (GLenum err = glewInit(); GLEW_OK != err) {
            std::ostringstream error;
            error << glewGetErrorString(err);
            IS_CORE_ERROR("Unable to initialize GLEW - error: {} - abort program", error.str());
            std::exit(EXIT_FAILURE);
        }
        if (GLEW_VERSION_4_5) {
            std::ostringstream glew_version;
            glew_version << glewGetString(GLEW_VERSION);
            IS_CORE_INFO("Using glew version: {}", glew_version.str());
            IS_CORE_INFO("Driver supports OpenGL 4.5");
        } else {
            IS_CORE_ERROR("Driver doesn't support OpenGL 4.5 - abort program");
            std::exit(EXIT_FAILURE);
        }
    }

} // end namespace IS
