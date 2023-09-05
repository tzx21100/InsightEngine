#include "Pch.h"
#include "GLFXWindow.h"

#define WIDTH 1280
#define HEIGHT 720


namespace IS {

    //this will be the update for the window we can use this to register like people pressing stuff on the window
    void glfxWindow::Update([[maybe_unused]] float time)  {
        // grafix updates
        glfwPollEvents();
        ISGraphics::update();

        //grafix draws
        ISGraphics::draw();
        glfwSwapBuffers(window); // Swap front and back buffers

        //register window closing 
        if (glfwWindowShouldClose(window)) {
            Message quit = Message(MessageType::Quit);
            EventManager::Instance().Broadcast(quit);
            glfwTerminate();
        }


    }

    std::string glfxWindow::getName()  {
        return "GLFWWindowSystem";
    }

    void glfxWindow::Initialize()  {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);

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
            return ;
        }

        glfwMakeContextCurrent(window);

        // this is the default setting ...
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // Part 2: Initialize entry points to OpenGL functions and extensions
        GLenum err = glewInit();
        if (GLEW_OK != err) {
            std::cerr << "Unable to initialize GLEW - error: "
                << glewGetErrorString(err) << " abort program" << std::endl;
            return ;
        }
        if (GLEW_VERSION_4_5) {
            IS_CORE_INFO("Using glew version: ", glewGetString(GLEW_VERSION));
            IS_CORE_INFO("Driver supports OpenGL 4.5\n");
        }
        else {
            std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
            return ;
        }

        ISGraphics::init();

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

        // Dark mode, of course
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    void glfxWindow::HandleMessage(const Message& message){
        if (message.GetType() == MessageType::Collide) {
            // Handle collision logic here
            IS_CORE_INFO("Handling collision in PhysicsSystem.");
        }
    }
    glfxWindow::glfxWindow(GLFWwindow* windo) : window(windo) {}

}
