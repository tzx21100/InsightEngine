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

// Dependencies
#include <ranges>
#include <imgui.h>

namespace IS {
    Vec2 EditorLayer::mDockspacePosition{};

    EditorLayer::EditorLayer() : Layer("Editor Layer") { AddPanels(); }

    void EditorLayer::OnAttach() {
        // Attach scene viewer, import icons, open project...
        IS_CORE_DEBUG("{} attached", GetName());
    }

    void EditorLayer::OnDetach() {
        IS_CORE_DEBUG("{} detached", GetName());
    }

    void EditorLayer::OnUpdate([[maybe_unused]] float dt) {
        InsightEngine& engine = InsightEngine::Instance();
        auto const& input = engine.GetSystem<InputManager>("Input");

        // Shortcuts
        const bool ctrl_held       = input->IsKeyHeld(GLFW_KEY_LEFT_CONTROL) || input->IsKeyHeld(GLFW_KEY_RIGHT_CONTROL);
        const bool shift_held      = input->IsKeyHeld(GLFW_KEY_LEFT_SHIFT) || input->IsKeyHeld(GLFW_KEY_RIGHT_SHIFT);
        const bool alt_held        = input->IsKeyHeld(GLFW_KEY_LEFT_ALT) || input->IsKeyHeld(GLFW_KEY_RIGHT_ALT);
        const bool n_pressed       = input->IsKeyPressed(GLFW_KEY_N);
        const bool l_pressed       = input->IsKeyPressed(GLFW_KEY_L);
        const bool s_pressed       = input->IsKeyPressed(GLFW_KEY_S);
        const bool ffour_pressed   = input->IsKeyPressed(GLFW_KEY_F4);
        const bool feleven_pressed = input->IsKeyPressed(GLFW_KEY_F11);

        if (ctrl_held && n_pressed) // Ctrl+N
            NewScene();
        if (ctrl_held && l_pressed) // Ctrl+L
            mShowLoad = true;
        if (ctrl_held && s_pressed) // Ctrl+S
            SaveScene();
        if (ctrl_held && shift_held && s_pressed) // Ctrl+Shift+S
            mShowSave = true;
        if (alt_held && ffour_pressed) // Alt+F4
            ExitProgram();
        if (feleven_pressed) // F11
            ToggleFullscreen();
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

        // Start Rendering dockspace
        ImGui::Begin("EditorDockSpace", nullptr, window_flags);
        mDockspacePosition = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
        ImGui::PopStyleVar(3);

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec2 min_window_size = style.WindowMinSize;
        style.WindowMinSize = ImVec2(200.f, 200.f);

        // Enable dockspace
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("Editor");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        // Menu bar
        RenderMenuBar();

        // Render Panels
        for (auto& panel : mPanels)
            panel->RenderPanel();

        // End Rendering dockspace
        ImGui::End();

        style.WindowMinSize = min_window_size;

        // Overlay
        RenderSceneOverlay();
    }

    void EditorLayer::AddPanels() {
        mPanels.emplace_back(std::make_shared<SceneHierarchyPanel>());
        mPanels.emplace_back(std::make_shared<ScenePanel>());
        mPanels.emplace_back(std::make_shared<LogConsolePanel>());
        mPanels.emplace_back(std::make_shared<PerformancePanel>());
    }

    void EditorLayer::RenderMenuBar() {
        InsightEngine& engine = InsightEngine::Instance();

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::MenuItem("(Empty)");
                ImGui::Separator();
                if (ImGui::MenuItem("Fullscreen", "F11"))
                    ToggleFullscreen();
                ImGui::Separator();
                if (ImGui::MenuItem("Exit", "Alt+F4"))
                    ExitProgram();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                ImGui::MenuItem("(Empty)");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Scene")) {
                // New Scene
                if (ImGui::MenuItem("New Scene", "Ctrl+N"))
                    NewScene();
                // Load Test Scene
                if (ImGui::MenuItem("Load Test Scene"))
                    LoadTestScene();
                // Load Scene...
                if (ImGui::MenuItem("Load Scene...", "Ctrl+L"))
                    mShowLoad = true;
                ImGui::Separator();

                // Save Scene
                if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
                    SaveScene();
                // Save Scene As...
                if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
                    mShowSave = true;
                ImGui::Separator();

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

        if (mShowLoad)
            LoadScene();

        if (mShowSave)
            SaveSceneAs();
    }

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

    Vec2 EditorLayer::GetDockspacePosition() { return mDockspacePosition; }

    void EditorLayer::NewScene() {
        SceneHierarchyPanel::ResetSelection();
        InsightEngine::Instance().NewScene();
    }

    void EditorLayer::LoadTestScene() {
        SceneHierarchyPanel::ResetSelection();
        InsightEngine::Instance().LoadScene("testscene");
    }

    void EditorLayer::LoadScene() {
        SceneHierarchyPanel::ResetSelection();
        ImGui::OpenPopup("Load Scene...");
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::BeginPopupModal("Load Scene...", &mShowLoad, window_flags)) {
            std::string default_text = "testscene";
            char name[std::numeric_limits<char8_t>::max() + 1]{};
            auto source = default_text | std::ranges::views::take(default_text.size());
            std::ranges::copy(source, std::begin(name));

            ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText("##LoadScene", name, sizeof(name), input_flags) || ImGui::Button("Load")) {
                InsightEngine::Instance().LoadScene(name);
                mShowLoad = false;
            }

            ImGui::EndPopup();
        }
    }

    void EditorLayer::SaveScene() { InsightEngine::Instance().SaveCurrentScene("testscene"); }

    void EditorLayer::SaveSceneAs() {
        ImGui::OpenPopup("Save Scene As...");
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::BeginPopupModal("Save Scene As...", &mShowSave, window_flags)) {
            std::string default_text = "testscene";
            char name[std::numeric_limits<char8_t>::max() + 1]{};
            auto source = default_text | std::ranges::views::take(default_text.size());
            std::ranges::copy(source, std::begin(name));

            ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText("##SaveScene", name, sizeof(name), input_flags) || ImGui::Button("Save")) {
                InsightEngine::Instance().SaveCurrentScene(name);
                mShowSave = false;
            }

            ImGui::EndPopup();
        }
    }

    void EditorLayer::ToggleFullscreen() {
        InsightEngine& engine = InsightEngine::Instance();
        auto const& window = engine.GetSystem<WindowSystem>("Window");
        static bool fullscreen = window->IsFullScreen();
        fullscreen = !fullscreen;
        window->SetFullScreen(fullscreen);
        IS_CORE_DEBUG("{} mode", fullscreen ? "Fullscreen" : "Windowed");
    }

    void EditorLayer::ExitProgram() {
        InsightEngine::Instance().Exit();
    }

} // end namespace IS
