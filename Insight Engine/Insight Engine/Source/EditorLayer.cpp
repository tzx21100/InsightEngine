#include "Pch.h"
#include "EditorLayer.h"
#include "Graphics.h"

// Dependencies
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace IS {
    bool show_another_window = false;

    EditorLayer::EditorLayer() : Layer("Editor") {}

    void EditorLayer::onAttach() const {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        GLFWwindow* window = glfwGetCurrentContext();
        const char* glsl_version = "#version 450";
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        IS_CORE_DEBUG(getName(), " layer attached");
    }

    void EditorLayer::onDetach() const {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        IS_CORE_DEBUG(getName(), " layer detached");
    }

    void EditorLayer::onUpdate() const {
        Begin();
        onRender();
        End();
    }

    void EditorLayer::onRender() const {
        ISGraphics::ISModel& test_model = ISGraphics::test_model;

        ImGui::Begin(test_model.name.c_str());
        ImGui::Text("Control the attributes of %s.", test_model.name.c_str());
        std::string label{ "Draw " + test_model.name };
        ImGui::Checkbox(label.c_str(), &test_model.drawing);
        ImGui::SliderFloat("Width", &test_model.scaling.x, 2.f, 2.f * WIDTH);
        ImGui::SliderFloat("Height", &test_model.scaling.y, 1.f, 2.f * HEIGHT);
        ImGui::SliderFloat("Rotation", &test_model.angle, 0.f, 360.f);
        ImGui::ColorEdit3("Color", test_model.color);
        ImGui::Checkbox("Another Window", &show_another_window);
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Avg: %.3fms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        if (show_another_window) {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
    }

    void EditorLayer::Begin() const {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void EditorLayer::End() const {
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

} // end namespace IS
