#include "Pch.h"
#include "WindowSystem.h"
#include "JsonSaveLoad.h"

namespace IS {

    // In case <properties.json> is not found, window will use default properties
    WindowSystem::WindowProperties WindowSystem::WindowProperties::default_properties{ "Insight Engine", 1600, 900, true };

    WindowSystem::WindowSystem() {
        LoadProperties();

        // Initialize GLFW library
        bool success = glfwInit();
        if (!success)
            IS_CORE_CRITICAL("Failed to to initialize glew libary!");

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
        window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
        if (!window) {
            IS_CORE_CRITICAL("Failed to create OpneGL context!");
            glfwTerminate();
        }

        // Default setting
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        EnableVsync(props.vsync);

        glfwMakeContextCurrent(window);
    }

    WindowSystem::~WindowSystem() {
        glfwDestroyWindow(window);
        glfwTerminate();
        SaveProperties();
    }

    void WindowSystem::Initialize() {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);

    }

    void WindowSystem::BeginUpdate() {
        glfwPollEvents();
    }

    void WindowSystem::Update(float)  {
        //register window closing 
        if (glfwWindowShouldClose(window)) {
            Message quit = Message(MessageType::Quit);
            BROADCAST_MESSAGE(quit);
        }
    }

    void WindowSystem::EndUpdate() {
        glfwSwapBuffers(window);
    }

    void WindowSystem::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::Collide) {
            // Handle collision logic here
        }
    }

    std::string WindowSystem::getName()  {
        return "Window";
    }

    uint32_t WindowSystem::GetWidth() const {
        return props.width;
    }

    uint32_t WindowSystem::GetHeight() const {
        return props.height;
    }

    std::string WindowSystem::GetWindowTitle() const {
        return props.title;
    }

    void WindowSystem::SetWindowSize(uint32_t width, uint32_t height) {
        props.width = width;
        props.height = height;

        glfwSetWindowSize(window, width, height);
    }

    void WindowSystem::SetWindowTitle(std::string const& title) {
        props.title = title;

        glfwSetWindowTitle(window, title.c_str());
    }

    void WindowSystem::EnableVsync(bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
        props.vsync = enabled;
    }

    bool WindowSystem::IsVSync() const {
        return props.vsync;
    }

    void WindowSystem::LoadProperties() {
        std::string filename = "properties.json";
        Json::Value properties;
        
        // Load window properties from JSON file
        bool success = LoadJsonFromFile(properties, filename);
        if (auto const& win_props = properties["WindowProperties"];
            success && win_props["Title"].isString() && win_props["Width"].isInt() &&
            win_props["Height"].isInt() && win_props["Vsync"].isString()) {

            props.title  = win_props["Title"].asString();
            props.width  = win_props["Width"].asInt();
            props.height = win_props["Height"].asInt();
            props.vsync  = win_props["Vsync"].asString() == "On" ? true : false;

            IS_CORE_INFO("Loaded window properties from <{}>", filename);
        } else { // Assign default properties
            props = WindowProperties::default_properties;
            IS_CORE_INFO("Using default window properties");
        }
    }

    void WindowSystem::SaveProperties() {
        std::string filepath = "properties.json";
        Json::Value properties;
        auto& win_props = properties["WindowProperties"];
        win_props["Title"]  = props.title.c_str();
        win_props["Width"]  = props.width;
        win_props["Height"] = props.height;
        win_props["Vsync"]  = props.vsync ? "On" : "Off";

        // Save window propeties to JSON file
        bool success = SaveJsonToFile(properties, filepath);
        success ? IS_CORE_INFO("Successfully saved window properties to <{}>!", filepath) :
                  IS_CORE_ERROR("Failed to save window propeties to <{}>!", filepath);
    }

} // end namespace IS
