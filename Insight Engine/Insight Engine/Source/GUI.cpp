#include "Pch.h"
#include "GUI.h"
#include "EditorLayer.h"

// Dependencies
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace IS {

    GUISystem::GUISystem() {}

    GUISystem::~GUISystem() {
        Terminate();
    }

    void GUISystem::Initialize() {
        // Initialize log
        Log::init();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        // Set default font
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/fonts/Roboto/Roboto-Regular.ttf", 16.f);

        // Setup Dear ImGui style

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColors();

        // Setup Platform/Renderer backends
        GLFWwindow* window = glfwGetCurrentContext();
        const char* glsl_version = "#version 450";
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        PushLayer(new EditorLayer());
    }

    void GUISystem::Update(float delta_time) {


        Begin();
        layers.Update(delta_time);
        layers.Render();
        End();
    }

    void GUISystem::Terminate() {
        layers.clearStack();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GUISystem::HandleMessage([[maybe_unused]] Message const& message) {

    }

    std::string GUISystem::getName() {
        return "GUI";
    }

    void GUISystem::Begin() {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GUISystem::End() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void GUISystem::PushLayer(Layer* layer) {
        layers.pushLayer(layer);
        layer->onAttach();
    }

    void GUISystem::PushOverlay(Layer* overlay) {
        layers.pushOverlay(overlay);
        overlay->onAttach();
    }

    void GUISystem::PopLayer(Layer* layer) {
        layers.popLayer(layer);
        layer->onDetach();
    }

    void GUISystem::PopOverlay(Layer* overlay) {
        layers.popOverlay(overlay);
        overlay->onDetach();
    }

    void GUISystem::SetDarkThemeColors() const {
        auto& style = ImGui::GetStyle();
        auto& colors = style.Colors;

        // Colors
        //------------------------------------------------------------------------
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Scrollbar
        colors[ImGuiCol_SliderGrab] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };
        colors[ImGuiCol_SliderGrabActive] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };

        // Check Mark
        colors[ImGuiCol_CheckMark] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };

        // Resize Grip
        colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };
        colors[ImGuiCol_ResizeGripActive] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };

        // Dockspace
        colors[ImGuiCol_DockingPreview] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };

        // Menubar
        colors[ImGuiCol_MenuBarBg] = ImVec4{ .1137f, .1137f, .1137f, 1.f };
    }

} // end namespace IS
