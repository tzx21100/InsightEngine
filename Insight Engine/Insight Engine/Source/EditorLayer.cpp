/*!
 * \file EditorLayer.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This source file defines the implementation for class EditorLayer which
 * encapsulates the functionalities of a level editor layer.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "EditorLayer.h"
#include "Graphics.h"
#include "CoreEngine.h"

#include <ranges>

namespace IS {

    EditorLayer::EditorLayer() : Layer("Editor Layer"), mDockspacePosition(), mScenePanelSize(), mSceneHierarchyPanel() {}

    void EditorLayer::OnAttach() {
        // Attach scene viewer, import icons, open project...
        IS_CORE_DEBUG("{} attached", GetName());
    }

    void EditorLayer::OnDetach() {
        IS_CORE_DEBUG("{} detached", GetName());
    }

    void EditorLayer::OnUpdate([[maybe_unused]] float dt) {
        // Shortcuts
        auto input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        bool ctrl_held = input->IsKeyHeld(GLFW_KEY_LEFT_CONTROL) || input->IsKeyHeld(GLFW_KEY_RIGHT_CONTROL);
        bool shift_held = input->IsKeyHeld(GLFW_KEY_LEFT_SHIFT) || input->IsKeyHeld(GLFW_KEY_RIGHT_SHIFT);
        bool alt_held = input->IsKeyHeld(GLFW_KEY_LEFT_ALT) || input->IsKeyHeld(GLFW_KEY_RIGHT_ALT);
        //bool n_pressed = input->IsKeyPressed(GLFW_KEY_N);
        bool l_pressed = input->IsKeyPressed(GLFW_KEY_L);
        bool s_pressed = input->IsKeyPressed(GLFW_KEY_S);
        bool ffour_pressed = input->IsKeyPressed(GLFW_KEY_F4);

        //if (ctrl_held && n_pressed) // Ctrl+N
        //    NewScene();
        if (ctrl_held && l_pressed) // Ctrl+L
            show_load = true;
        if (ctrl_held && s_pressed) // Ctrl+S
            SaveScene();
        if (ctrl_held && shift_held && s_pressed) // Ctrl+Shift+S
            show_save = true;
        if (alt_held && ffour_pressed)
            InsightEngine::Instance().Exit();
    }

    void EditorLayer::OnRender() {

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGui::SetNextWindowBgAlpha(0.f);
        ImGui::Begin("EditorDockSpace", nullptr, window_flags);
        //i add in pos here to get the position of the dockspace
        mDockspacePosition = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
        ImGui::PopStyleVar(3);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("Editor");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        // Menu bar
        RenderMenuBar();

        // Other windows
        RenderScenePanel();
        RenderSceneHierarchyPanel();
        RenderPerformancePanel();
        RenderLogConsolePanel();
        RenderSceneOverlay();

        ImGui::End();
    }

    void EditorLayer::RenderMenuBar() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::MenuItem("(Empty)");
                ImGui::Separator();
                if (ImGui::MenuItem("Exit", "Alt+F4"))
                    InsightEngine::Instance().Exit();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                ImGui::MenuItem("(Empty)");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Scene")) {
                // New Scene
                /*if (ImGui::MenuItem("New Scene", "Ctrl+N"))
                    NewScene();*/
                // Load Test Scene
                if (ImGui::MenuItem("Load Test Scene"))
                    LoadTestScene();
                // Load Scene...
                if (ImGui::MenuItem("Load Scene...", "Ctrl+L"))
                    show_load = true;
                ImGui::Separator();

                // Save Scene
                if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
                    SaveScene();
                // Save Scene As...
                if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
                    show_save = true;
                ImGui::Separator();

                InsightEngine& engine = InsightEngine::Instance();
                if (ImGui::MenuItem("Create 500 Random Colors")) {
                    for (int i{}; i < 500; i++) {
                        engine.GenerateRandomEntity();
                    }
                }
                if (ImGui::MenuItem("Create 500 Random Textures")) {
                    for (int i{}; i < 500; ++i)
                        engine.GenerateRandomEntity(true);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        if (show_load)
            LoadScene();

        if (show_save)
            SaveSceneAs();
    }

    void EditorLayer::RenderScenePanel() {

        auto input = InsightEngine::Instance().GetSystem<InputManager>("Input");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
        ImGui::Begin("Scene", nullptr, flags);

        // Allow key/mouse event pass through only in this panel
        if (ImGui::IsWindowFocused()) {
            ImGuiIO& io = ImGui::GetIO();
            io.WantCaptureMouse = io.WantCaptureKeyboard = false;
        }

        ImVec2 scene_size = ImGui::GetWindowSize();
        ImVec2 scene_pos = ImGui::GetWindowPos();
        //scene pos for the input
        ImVec2 actual_scene_pos;
        actual_scene_pos.x = scene_pos.x-mDockspacePosition.x;
        actual_scene_pos.y = scene_pos.y-mDockspacePosition.y;
        input->setCenterPos(actual_scene_pos.x + scene_size.x / 2, actual_scene_pos.y + scene_size.y / 2);
        input->setRatio(scene_size.x, scene_size.y);

        // Resize framebuffer
        ImVec2 panel_size = ImGui::GetContentRegionAvail();
        if (!(mScenePanelSize.x == panel_size.x && mScenePanelSize.y == panel_size.y)) {
            ISGraphics::ResizeFramebuffer(static_cast<uint32_t>(panel_size.x), static_cast<uint32_t>(panel_size.y));
            mScenePanelSize = { panel_size.x, panel_size.y };
        }

        ImGui::Image(std::bit_cast<ImTextureID>(static_cast<uintptr_t>(ISGraphics::GetScreenTexture())),
                     ImVec2(mScenePanelSize.x, mScenePanelSize.y), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorLayer::RenderSceneHierarchyPanel() { mSceneHierarchyPanel.RenderPanel(); }

    void EditorLayer::RenderPerformancePanel() {
        ImGuiIO& io = ImGui::GetIO();
        auto font_bold = io.Fonts->Fonts[0];

        ImGui::Begin("Performance");
        if (ImGui::BeginTable("Engine", 2)) {
            ImGui::TableNextColumn();
            ImGui::PushFont(font_bold);
            ImGui::TextUnformatted("Framerate:");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::TextColored(
                io.Framerate < 15.f ? ImVec4(1.f, 0.3f, 0.2f, 1.f) : // red
                io.Framerate < 30.f ? ImVec4(1.f, .98f, 0.5f, 1.f) : // yellow
                ImVec4(1.f, 1.f, 1.f, 1.f), "%5.0f FPS", io.Framerate
            );

            ImGui::TableNextColumn();
            ImGui::PushFont(font_bold);
            ImGui::TextUnformatted(" Timestep:");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::TextColored(
                io.Framerate < 15.f ? ImVec4(1.f, 0.3f, 0.2f, 1.f) : // red
                io.Framerate < 30.f ? ImVec4(1.f, .98f, 0.5f, 1.f) : // yellow
                ImVec4(1.f, 1.f, 1.f, 1.f), "%.2f ms", 1000.f / io.Framerate
            );

            ImGui::EndTable();
        }
        ImGui::Dummy({ 5.f, 5.f });
        ImGui::Separator();
        ImGui::Dummy({ 5.f, 5.f });
        
        // Create a table for system usage
        ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_BordersH;
        if (ImGui::BeginTable("Systems", 2, flags)) {
            // Table headers
            ImGui::PushFont(font_bold);
            ImGui::TableSetupColumn("System");
            ImGui::TableSetupColumn("Usage %");
            ImGui::TableHeadersRow();
            ImGui::PopFont();

            // Table values
            for (InsightEngine& engine = InsightEngine::Instance();
                auto const& [system, dt] : engine.GetSystemDeltas()) {
                double percent = std::round((dt / engine.GetSystemDeltas().at("Engine")) * 100.0);
                if (system != "Engine") {
                    ImGui::TableNextColumn();
                    ImGui::Spacing();
                    ImGui::PushFont(font_bold);
                    ImGui::Text("%s", system.c_str());
                    ImGui::PopFont();

                    ImGui::TableNextColumn();
                    ImGui::Spacing();
                    ImGui::Text("%6d%%", static_cast<int>(percent));
                    ImGui::Spacing();
                }
            }
            // End rendering table
            ImGui::EndTable();
        }

        // End rendering window
        ImGui::End();
    }

    void EditorLayer::RenderLogConsolePanel() { Logger::GetLoggerGUI().Draw("Log Console"); }

    void EditorLayer::RenderSceneOverlay() {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize |
                                        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        InsightEngine& engine = InsightEngine::Instance();

        ImGuiIO& io = ImGui::GetIO();
        auto const& font_bold = io.Fonts->Fonts[0];

        ImGui::SetNextWindowBgAlpha(0.8f); // Translucent background
        if (ImGui::Begin("Info/Help", nullptr, window_flags)) {
            if (ImGui::BeginTable("Entities", 2)) {
                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Entities Alive:");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::Text("%d", engine.EntitiesAlive());

                // Comma separted numbers
                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted("Max Entities:");
                ImGui::PopFont();
                std::ostringstream oss;
                oss.imbue(std::locale(""));
                oss << std::fixed << MAX_ENTITIES;
                ImGui::TableNextColumn();
                ImGui::Text("%s", oss.str().c_str());

                ImGui::EndTable();
            }
            ImGui::Separator();
            ImGui::PushFont(font_bold);
            ImGui::TextUnformatted("Controls ONLY work if scene panel focused!");
            ImGui::PopFont();
            ImGui::Separator();
            ImGui::PushFont(font_bold);
            ImGui::TextUnformatted("General Controls");
            ImGui::PopFont();
            ImGui::BulletText("Press 'Tab' to toggle GUI");
            ImGui::BulletText("Press 'F11' to toggle fullscreen/windowed");
            ImGui::BulletText("Click mouse scrollwheel to spawn entity");
            ImGui::BulletText("Click right mouse button to spawn rigidbody entity");
            ImGui::Separator();
            ImGui::PushFont(font_bold);
            ImGui::TextUnformatted("Player Controls");
            ImGui::PopFont();
            ImGui::BulletText("Press 'WASD' to move in the four directions");
            ImGui::BulletText("Press 'Q' to rotate clockwise, 'E' to rotate counter-clockwise");
            ImGui::Separator();
            ImGui::PushFont(font_bold);
            ImGui::TextUnformatted("Physics Controls");
            ImGui::PopFont();
            ImGui::BulletText("Press '2' to enable draw collision boxes, '1' to disable");
            ImGui::BulletText("Press 'G' to enable gravity, 'F' to disable");
            ImGui::BulletText("Press 'Shift' + 'Enter' to freeze frame, 'Enter' to step frame");
            ImGui::Separator();
            ImGui::PushFont(font_bold);
            ImGui::TextUnformatted("Audio Controls");
            ImGui::PopFont();
            ImGui::BulletText("Press 'Z' to play sfx");
            ImGui::BulletText("Press 'X' to play music");

        }
        ImGui::End();
    }

    void EditorLayer::NewScene() {
        mSceneHierarchyPanel.ResetSelection();
        InsightEngine::Instance().NewScene();
    }

    void EditorLayer::LoadTestScene() {
        mSceneHierarchyPanel.ResetSelection();
        InsightEngine::Instance().LoadScene("testscene");
    }

    void EditorLayer::LoadScene() {
        mSceneHierarchyPanel.ResetSelection();
        ImGui::OpenPopup("Load Scene...");
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::BeginPopupModal("Load Scene...", &show_load, window_flags)) {
            std::string default_text = "testscene";
            char name[std::numeric_limits<char8_t>::max() + 1]{};
            auto source = default_text | std::ranges::views::take(default_text.size());
            std::ranges::copy(source, std::begin(name));

            ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText("##LoadScene", name, sizeof(name), input_flags) || ImGui::Button("Load")) {
                InsightEngine::Instance().LoadScene(name);
                show_load = false;
            }

            ImGui::EndPopup();
        }
    }

    void EditorLayer::SaveScene() { InsightEngine::Instance().SaveCurrentScene("testscene"); }

    void EditorLayer::SaveSceneAs() {
        ImGui::OpenPopup("Save Scene As...");
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::BeginPopupModal("Save Scene As...", &show_save, window_flags)) {
            std::string default_text = "testscene";
            char name[std::numeric_limits<char8_t>::max() + 1]{};
            auto source = default_text | std::ranges::views::take(default_text.size());
            std::ranges::copy(source, std::begin(name));

            ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText("##SaveScene", name, sizeof(name), input_flags) || ImGui::Button("Save")) {
                InsightEngine::Instance().SaveCurrentScene(name);
                show_save = false;
            }

            ImGui::EndPopup();
        }
    }

} // end namespace IS
