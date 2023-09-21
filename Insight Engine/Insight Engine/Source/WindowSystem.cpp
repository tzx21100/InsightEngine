#include "Pch.h"
#include "WindowSystem.h"
#include "JsonSaveLoad.h"

namespace IS {

    WindowSystem::WindowProperties WindowSystem::WindowProperties::default_properties{ "Insight Engine", 1600, 900, true };

    WindowSystem::WindowSystem() {
        LoadProperties();

        // Initialize GLFW library
        if (!glfwInit())
            IS_CORE_CRITICAL("Failed to to initialize libary!");

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

        // All the stuff below should be handled by graphics
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
       // ISGraphics::init();
    }

    //this will be the update for the window we can use this to register like people pressing stuff on the window
    void WindowSystem::Update(float)  {
        //register window closing 
        if (glfwWindowShouldClose(window)) {
            Message quit = Message(MessageType::Quit);
            BROADCAST_MESSAGE(quit);
        }

        //ISGraphics::update(time);

        //grafix draws
       // ISGraphics::draw();
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
        Json::Value properties{};
        
        // Load window properties from JSON file
        if (LoadJsonFromFile(properties, filename)) {
            auto const& win_props = properties["WindowProperties"];
            if (win_props.isMember("Title") && win_props["Title"].isString()) {
                props.title = win_props["Title"].asString();
                IS_CORE_DEBUG("Using json window title");
            } else {
                props.title = WindowProperties::default_properties.title;
                IS_CORE_DEBUG("Using default title");
            }
            if (win_props.isMember("Width") && win_props["Width"].isInt()) {
                props.width = win_props["Width"].asInt();
                IS_CORE_DEBUG("Using json window width");
            } else {
                props.width = WindowProperties::default_properties.width;
                IS_CORE_DEBUG("Using default width");
            }
            if (win_props.isMember("Height") && win_props["Height"].isInt()) {
                props.height = win_props["Height"].asInt();
                IS_CORE_DEBUG("Using json window height");
            } else {
                props.height = WindowProperties::default_properties.height;
                IS_CORE_DEBUG("Using default height");
            }
            if (win_props.isMember("Vsync") && win_props["Vsync"].isString()) {
                props.vsync = win_props["Vsync"].asString() == "On" ? true : false;
                IS_CORE_DEBUG("Using json window vsync");
            } else {
                props.vsync = WindowProperties::default_properties.vsync;
                IS_CORE_DEBUG("Using default vsync");
            }

            IS_CORE_INFO("Loaded window properties from JSON");

        } else { // Assign default properties
            props = WindowProperties::default_properties;
            IS_CORE_INFO("Loaded default window properties");
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
        
        SaveJsonToFile(properties, filepath) ? IS_CORE_INFO("Successfully saved window properties to JSON!") :
                                               IS_CORE_ERROR("Failed to save window propeties to JSON!");
    }

} // end namespace IS
