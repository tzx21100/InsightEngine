#include "Pch.h"
#include "EditorLayer.h"
#include "Graphics.h"
#include "CoreEngine.h"

// Dependencies
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace IS {

    bool show_another_window = false;

    EditorLayer::EditorLayer() : Layer("Editor Layer") {}

    void EditorLayer::onAttach() {
        // Attach scene viewer, import icons, open project...
        IS_CORE_DEBUG("{} attached", getName());
    }

    void EditorLayer::onDetach() {
        IS_CORE_DEBUG("{} detached", getName());
    }

    void EditorLayer::onUpdate([[maybe_unused]] float dt) {
        // some updating
    }

    void EditorLayer::onRender() {

        static bool show_dockspace = true;
        static bool opt_fullscreen = false;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        } else {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGui::Begin("EditorDockSpace", &show_dockspace, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("Editor");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        // Create Menu Bar
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();
                if (ImGui::MenuItem("Exit"))
                    InsightEngine::Instance().Exit();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window")) {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools")) {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Build")) {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Select")) {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Actor")) {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help")) {

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Other windows
        RenderInspector();
        RenderPerformanceViewer();

        ImGui::End();
    }

    void EditorLayer::RenderInspector() {
        ISGraphics::ISModel& test_model = ISGraphics::test_model;

        ImGui::Begin("Inspector");
        std::string label{ "Draw " + test_model.name };
        ImGui::Checkbox(label.c_str(), &test_model.drawing);
        ImGui::SliderFloat("Width", &test_model.scaling.x, 2.f, 2.f * WIDTH);
        ImGui::SliderFloat("Height", &test_model.scaling.y, 1.f, 2.f * HEIGHT);
        ImGui::SliderFloat("Rotation", &test_model.angle, 0.f, 360.f);
        ImGui::ColorEdit3("Color", test_model.color);
        ImGui::Checkbox("Another Window", &show_another_window);
        ImGui::End();

        if (show_another_window) {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            ImGui::End();
        }
    }

    void EditorLayer::RenderPerformanceViewer() {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Performance");
        ImGui::Text("%.0f FPS", io.Framerate);
        ImGui::Text("%.2f ms", 1000.f / io.Framerate);
        for (auto& [system, dt] : InsightEngine::Instance().systemDeltas) {
            double percent = std::round((dt / InsightEngine::Instance().systemDeltas["Engine"]) * 100.0);
            if (system != "Engine")
                ImGui::Text("%3d%% %s", static_cast<int>(percent), system.c_str());
        }
        
        ImGui::End();
    }

} // end namespace IS