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
#include "EditorUtils.h"
#include "Graphics.h"
#include "CoreEngine.h"
#include "AssetBrowserPanel.h"

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
        const bool o_pressed       = input->IsKeyPressed(GLFW_KEY_O);
        const bool s_pressed       = input->IsKeyPressed(GLFW_KEY_S);
        const bool ffour_pressed   = input->IsKeyPressed(GLFW_KEY_F4);
        const bool feleven_pressed = input->IsKeyPressed(GLFW_KEY_F11);

        if (ctrl_held && n_pressed) { NewScene(); }                     // Ctrl+N
        if (ctrl_held && o_pressed) { mShowLoad = true; }               // Ctrl+O
        if (ctrl_held && s_pressed) { SaveScene(); }                    // Ctrl+S
        if (ctrl_held && shift_held && s_pressed) { mShowSave = true; } // Ctrl+Shift+S
        if (alt_held && ffour_pressed) { ExitProgram(); }               // Alt+F4
        if (feleven_pressed) { ToggleFullscreen(); }                    // F11
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

        style.WindowMinSize = min_window_size;

        ImGui::End(); // end dockspace
    }

    void EditorLayer::AddPanels() {
        mPanels.emplace_back(std::make_shared<PhysicsControlPanel>());
        mSceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>();
        mPanels.emplace_back(mSceneHierarchyPanel);
        mPanels.emplace_back(std::make_shared<PerformancePanel>());
        mPanels.emplace_back(std::make_shared<ScenePanel>(mSceneHierarchyPanel->GetSelectedEntity()));
        mPanels.emplace_back(std::make_shared<AssetBrowserPanel>());
        mPanels.emplace_back(std::make_shared<LogConsolePanel>());
    }

    void EditorLayer::RenderMenuBar() {
        InsightEngine& engine = InsightEngine::Instance();
        std::string fullscreen_label = engine.IsFullScreen() ? "Exit Fullscreen" : "Enter Fullscreen";

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                // New File
                if (ImGui::BeginMenu("New")) {
                    // Load New Scene
                    if (ImGui::MenuItem("Scene", "Ctrl+N")) { NewScene(); }
                    // Create New Script
                    if (ImGui::MenuItem("Script...")) { mShowNewScript = true; }
                    ImGui::EndMenu();
                } // end menu New

                // Open File
                if (ImGui::BeginMenu("Open")) {
                    // Open Scene
                    if (ImGui::MenuItem("Scene...", "Ctrl+O")) { mShowLoad = true; }
                    // Open Script
                    if (ImGui::MenuItem("Script...")) { mShowOpenScript = true; }
                    ImGui::EndMenu();
                } // end menu Open

                ImGui::Separator();

                // Save File
                if (ImGui::MenuItem("Save Scene", "Ctrl+S")) { SaveScene(); }
                // Save Scene As...
                if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) { mShowSave = true; }

                ImGui::Separator();

                // Toggle fullscreen mode
                if (ImGui::MenuItem(fullscreen_label.c_str(), "F11")) { ToggleFullscreen(); }
                    
                ImGui::Separator();

                // Exit current program
                if (ImGui::MenuItem("Exit", "Alt+F4")) { ExitProgram(); }

                ImGui::EndMenu();
            } // end menu File

            if (ImGui::BeginMenu("Edit")) {
                ImGui::MenuItem("(Empty)");
                ImGui::EndMenu();
            } // end menu Edit

            bool open_testing = ImGui::BeginMenu("Testing");

            // Tooltip
            if (!open_testing && ImGui::BeginItemTooltip()) {
                static const float MAX_WIDTH = 200.f;
                ImGui::PushTextWrapPos(MAX_WIDTH); // Set a maximum width of 200 pixels.
                ImGui::TextUnformatted("These options are for testing purposes, not part of the engine iteself.");
                ImGui::PopTextWrapPos(); // Reset text wrap width to default.
                ImGui::EndTooltip();
            }

            // Render menu Testing
            if (open_testing) {
                if (ImGui::BeginMenu("Create Entity")) {
                    // Create random entities without texture
                    if (ImGui::BeginMenu("Random Color")) {
                        if (ImGui::MenuItem("100"))    { for (int i{}; i < 100; ++i)    { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("500"))    { for (int i{}; i < 500; ++i)    { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("1,000"))  { for (int i{}; i < 1'000; ++i)  { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("5,000"))  { for (int i{}; i < 5'000; ++i)  { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("10,000")) { for (int i{}; i < 10'000; ++i) { engine.GenerateRandomEntity(); } }
                        ImGui::EndMenu();
                    } // end menu Random Color

                    // Create random entities with texture
                    if (ImGui::BeginMenu("Random Texture")) {
                        if (ImGui::MenuItem("100"))    { for (int i{}; i < 100; ++i)    { engine.GenerateRandomEntity(true); } }
                        if (ImGui::MenuItem("500"))    { for (int i{}; i < 500; ++i)    { engine.GenerateRandomEntity(true); } }
                        if (ImGui::MenuItem("1,000"))  { for (int i{}; i < 1'000; ++i)  { engine.GenerateRandomEntity(true); } }
                        if (ImGui::MenuItem("5,000"))  { for (int i{}; i < 5'000; ++i)  { engine.GenerateRandomEntity(true); } }
                        if (ImGui::MenuItem("10,000")) { for (int i{}; i < 10'000; ++i) { engine.GenerateRandomEntity(true); } }
                        ImGui::EndMenu();
                    } // end menu Random Texture

                    ImGui::EndMenu();
                } // end menu Create Entity

                ImGui::EndMenu();
            } // end menu Testing

            ImGui::EndMenuBar();
        }

        if (mShowLoad) { OpenScene(); }
        if (mShowSave) { SaveSceneAs(); }
        if (mShowNewScript) { NewScript(); }
        if (mShowOpenScript) { OpenScript(); }
    }

    Vec2 EditorLayer::GetDockspacePosition() { return mDockspacePosition; }

    void EditorLayer::NewScene() { mSceneHierarchyPanel->ResetSelection(); InsightEngine::Instance().NewScene(); }

    void EditorLayer::LoadTestScene() { mSceneHierarchyPanel->ResetSelection(); InsightEngine::Instance().LoadScene("testscene"); }

    void EditorLayer::OpenScene() {
        mSceneHierarchyPanel->ResetSelection();
        ImGui::OpenPopup("Open Scene...");
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::BeginPopupModal("Open Scene...", &mShowLoad, window_flags)) {
            std::string default_text = "testscene";
            char name[std::numeric_limits<char8_t>::max() + 1]{};
            auto source = default_text | std::ranges::views::take(default_text.size());
            std::ranges::copy(source, std::begin(name));

            ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            bool enter_pressed = ImGui::InputText("##OpenScene", name, sizeof(name), input_flags);
            ImGui::SameLine();
            bool button_pressed = ImGui::Button("Open");
            if (enter_pressed || button_pressed) {
                InsightEngine::Instance().LoadScene(name);
                mShowLoad = false;

                IS_CORE_TRACE("Selected Entity reference count: {}", mSceneHierarchyPanel->GetSelectedEntity().use_count());
            }

            ImGui::EndPopup(); // end popup Open Scene...
        }
    }

    void EditorLayer::OpenScene(const char* path) {
        std::string name = path;
        std::string prefix = "Scene\\";
        if (name.substr(0, prefix.length()) == prefix)
            name.erase(0, prefix.length());

        InsightEngine::Instance().LoadScene(name);
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

            ImGui::EndPopup(); // end popup Save Scene As...
        }
    }

    void EditorLayer::NewScript() {
        auto& engine = InsightEngine::Instance();

        ImGui::OpenPopup("New Script...");
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::BeginPopupModal("New Script...", &mShowNewScript, window_flags)) {
            std::string default_text = "NewScript";
            char name[std::numeric_limits<char8_t>::max() + 1]{};
            auto source = default_text | std::ranges::views::take(default_text.size());
            std::ranges::copy(source, std::begin(name));

            ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText("##NewScript", name, sizeof(name), input_flags) || ImGui::Button("Create")) {
                engine.CreateGameScript(name);
                engine.OpenGameScript(name);
                mShowNewScript = false;
            }

            ImGui::EndPopup(); // end popup New Script...
        }
    }

    void EditorLayer::OpenScript() {
        auto& engine = InsightEngine::Instance();

        ImGui::OpenPopup("Open Script...");
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::BeginPopupModal("Open Script...", &mShowOpenScript, window_flags)) {
            std::string default_text = "";
            char name[std::numeric_limits<char8_t>::max() + 1]{};
            auto source = default_text | std::ranges::views::take(default_text.size());
            std::ranges::copy(source, std::begin(name));

            ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText("##OpenScript", name, sizeof(name), input_flags) || ImGui::Button("Open")) {
                engine.OpenGameScript(name);
                mShowOpenScript = false;
            }

            ImGui::EndPopup(); // end popup Open Script...
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

    void EditorLayer::ExitProgram() { InsightEngine::Instance().Exit(); }

} // end namespace IS
