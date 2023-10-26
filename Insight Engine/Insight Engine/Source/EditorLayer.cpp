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
#include "InspectorPanel.h"
#include "AssetBrowserPanel.h"
#include "FileUtils.h"

// Dependencies
#include <ranges>
#include <imgui.h>

namespace IS {

    Vec2 EditorLayer::mDockspacePosition{};

    EditorLayer::EditorLayer() : Layer("Editor Layer") {}

    void EditorLayer::OnAttach()
    {
        AttachPanels();
        //OpenScene();

        IS_CORE_DEBUG("{} attached", GetName());
    }

    void EditorLayer::OnDetach()
    {
        mPanels.clear();
        IS_CORE_DEBUG("{} detached", GetName());
    }

    void EditorLayer::OnUpdate(float)
    {
        InsightEngine& engine = InsightEngine::Instance();
        auto const& input = engine.GetSystem<InputManager>("Input");

        // Shortcuts
        const bool CTRL_HELD   = input->IsKeyHeld(GLFW_KEY_LEFT_CONTROL) || input->IsKeyHeld(GLFW_KEY_RIGHT_CONTROL);
        const bool SHIFT_HELD  = input->IsKeyHeld(GLFW_KEY_LEFT_SHIFT)   || input->IsKeyHeld(GLFW_KEY_RIGHT_SHIFT);
        const bool ALT_HELD    = input->IsKeyHeld(GLFW_KEY_LEFT_ALT)     || input->IsKeyHeld(GLFW_KEY_RIGHT_ALT);
        const bool N_PRESSED   = input->IsKeyPressed(GLFW_KEY_N);
        const bool O_PRESSED   = input->IsKeyPressed(GLFW_KEY_O);
        const bool S_PRESSED   = input->IsKeyPressed(GLFW_KEY_S);
        const bool F4_PRESSED  = input->IsKeyPressed(GLFW_KEY_F4);
        const bool F11_PRESSED = input->IsKeyPressed(GLFW_KEY_F11);

        if (CTRL_HELD && N_PRESSED) { mShowNewScene = true; }        // Ctrl+N
        if (CTRL_HELD && O_PRESSED) { OpenScene(); }                 // Ctrl+O
        if (CTRL_HELD && S_PRESSED) { SaveScene(); }                 // Ctrl+S
        if (CTRL_HELD && SHIFT_HELD && S_PRESSED) { SaveSceneAs(); } // Ctrl+Shift+S
        if (ALT_HELD && F4_PRESSED) { ExitProgram(); }               // Alt+F4
        if (F11_PRESSED) { ToggleFullscreen(); }                     // F11
    }

    void EditorLayer::OnRender()
    {
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
        style.WindowMinSize = ImVec2(350.f, 300.f);

        // Enable dockspace
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
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

    Vec2 EditorLayer::GetDockspacePosition() { return mDockspacePosition; }

    void EditorLayer::RenderMenuBar()
    {
        InsightEngine& engine = InsightEngine::Instance();
        std::string fullscreen_label = engine.IsFullScreen() ? "Exit Fullscreen" : "Enter Fullscreen";

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // New File
                if (ImGui::BeginMenu("New"))
                {
                    if (ImGui::MenuItem("Scene", "Ctrl+N")) { mShowNewScene = true; }
                    if (ImGui::MenuItem("Script...")) { mShowNewScript = true; }
                    ImGui::EndMenu();
                } // end menu New

                // Open File
                if (ImGui::BeginMenu("Open"))
                {
                    if (ImGui::MenuItem("Scene...", "Ctrl+O")) { OpenScene(); }
                    if (ImGui::MenuItem("Script...")) { FileUtils::OpenScript(); }
                    ImGui::EndMenu();
                } // end menu Open

                ImGui::Separator();

                // Saving Scene
                if (ImGui::MenuItem("Save Scene", "Ctrl+S")) { SaveScene(); }
                if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) { SaveSceneAs(); }

                ImGui::Separator();

                // Toggle fullscreen mode
                if (ImGui::MenuItem(fullscreen_label.c_str(), "F11")) { ToggleFullscreen(); }

                ImGui::Separator();

                // Exit current program
                if (ImGui::MenuItem("Exit", "Alt+F4")) { ExitProgram(); }

                ImGui::EndMenu();
            } // end menu File

            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::MenuItem("(Empty)");
                ImGui::EndMenu();
            } // end menu Edit

            bool open_testing = ImGui::BeginMenu("Test");

            // Tooltip
            if (!open_testing && ImGui::BeginItemTooltip())
            {
                static const float MAX_WIDTH = 200.f;
                ImGui::PushTextWrapPos(MAX_WIDTH); // Set a maximum width of 200 pixels.
                ImGui::TextUnformatted("These options are for testing purposes, not part of the engine iteself.");
                ImGui::PopTextWrapPos(); // Reset text wrap width to default.
                ImGui::EndTooltip();
            }

            // Render menu Test
            if (open_testing)
            {
                if (ImGui::BeginMenu("Add Entity"))
                {
                    // Create random entities without texture
                    if (ImGui::BeginMenu("Random Color"))
                    {
                        if (ImGui::MenuItem("100")) { for (int i{}; i < 100; ++i) { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("500")) { for (int i{}; i < 500; ++i) { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("1,000")) { for (int i{}; i < 1'000; ++i) { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("5,000")) { for (int i{}; i < 5'000; ++i) { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("10,000")) { for (int i{}; i < 10'000; ++i) { engine.GenerateRandomEntity(); } }
                        ImGui::EndMenu();
                    } // end menu Random Color

                    // Create random entities with texture
                    if (ImGui::BeginMenu("Random Texture"))
                    {
                        if (ImGui::MenuItem("100")) { for (int i{}; i < 100; ++i) { engine.GenerateRandomEntity(true); } }
                        if (ImGui::MenuItem("500")) { for (int i{}; i < 500; ++i) { engine.GenerateRandomEntity(true); } }
                        if (ImGui::MenuItem("1,000")) { for (int i{}; i < 1'000; ++i) { engine.GenerateRandomEntity(true); } }
                        if (ImGui::MenuItem("5,000")) { for (int i{}; i < 5'000; ++i) { engine.GenerateRandomEntity(true); } }
                        if (ImGui::MenuItem("10,000")) { for (int i{}; i < 10'000; ++i) { engine.GenerateRandomEntity(true); } }
                        ImGui::EndMenu();
                    } // end menu Random Texture

                    ImGui::EndMenu();
                } // end menu Create Entity

                ImGui::EndMenu();
            } // end menu Testing

            ImGui::EndMenuBar();
        }

        // Show Create Popup window
        if (mShowNewScene)
            ShowCreatePopup("Create new scene", "NewScene", &mShowNewScene, [this](const char* scene_name)
            {
                SceneManager& scene_manager = SceneManager::Instance();
                mSceneHierarchyPanel->ResetSelection();
                //InsightEngine::Instance().NewScene();
                //mActiveScene = scene_name;
                //IS_CORE_TRACE("Current Scene: {}", mActiveScene);
                scene_manager.NewScene(scene_name);
                IS_CORE_TRACE("Current Scene: {}", scene_name);
                
            });
        if (mShowNewScript)
            ShowCreatePopup("Create new script", "NewScript", &mShowNewScript, [](const char* script_name)
            {
                InsightEngine::Instance().CreateGameScript(script_name);
                InsightEngine::Instance().OpenGameScript(script_name);
            });
    }

    void EditorLayer::AttachPanels()
    {
        mSceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>();

        mPanels.emplace_back(std::make_shared<ScenePanel>(mSceneHierarchyPanel));
        mPanels.emplace_back(std::make_shared<PhysicsControlPanel>());
        mPanels.emplace_back(mSceneHierarchyPanel);
        mPanels.emplace_back(std::make_shared<InspectorPanel>(mSceneHierarchyPanel));
        mPanels.emplace_back(std::make_shared<PerformancePanel>());
        mPanels.emplace_back(std::make_shared<AssetBrowserPanel>());
        mPanels.emplace_back(std::make_shared<LogConsolePanel>());
    }

    void EditorLayer::ShowCreatePopup(const char* popup_name, const char* default_text, bool* show, std::function<void(const char*)> CreateAction)
    {
        ImGui::OpenPopup(popup_name);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::BeginPopupModal(popup_name, show, window_flags))
        {
            char name[256]{};
            memcpy(name, default_text, strlen(default_text));

            ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText(("##" + std::string(popup_name)).c_str(), name, sizeof(name), input_flags) || ImGui::Button("Create"))
            {
                CreateAction(name);
                *show = false;
            }

            ImGui::EndPopup();
        }
    }

    void EditorLayer::OpenScene()
    {
        mSceneHierarchyPanel->ResetSelection();
        //InsightEngine& engine = InsightEngine::Instance();
                
        if (std::filesystem::path filepath(FileUtils::OpenFile("Insight Scene (*.insight)\0*.insight\0", "Assets\\Scene")); !filepath.empty())
        {
            auto const& relative_path = std::filesystem::relative(filepath);
            auto& scene_manager = SceneManager::Instance();
            scene_manager.LoadScene(relative_path.string());
            IS_CORE_DEBUG("Active Scene: {}", relative_path.string());
            //engine.LoadScene(relative_path.string());
            //mActiveScene = filepath.stem().string();

            //IS_CORE_TRACE("Current Scene: {}", mActiveScene);
        }
    }

    void EditorLayer::OpenScene(std::string const& path)
    {
        //InsightEngine::Instance().LoadScene(path);
        std::filesystem::path filepath(path);
        //mActiveScene = filepath.stem().string();
        //IS_CORE_TRACE("Current Scene: {}", mActiveScene);
        auto& scene_manager = SceneManager::Instance();
        scene_manager.LoadScene(filepath.string());
        IS_CORE_DEBUG("Active Scene: {}", filepath.stem().string());
    }

    void EditorLayer::SaveScene() 
    {
        //InsightEngine::Instance().SaveCurrentScene(mActiveScene);
        auto& scene_manager = SceneManager::Instance();
        scene_manager.SaveScene();
    }

    void EditorLayer::SaveSceneAs()
    {
        //InsightEngine& engine = InsightEngine::Instance();

        if (std::filesystem::path filepath(FileUtils::SaveFile("Insight Scene (*.insight)\0*.insight\0", "Assets\\Scene")); !filepath.empty())
        {
            //mActiveScene = filepath.stem().string();
            //engine.SaveCurrentScene(mActiveScene);

            //IS_CORE_TRACE("Current Scene: {}", mActiveScene);
            auto& scene_manager = SceneManager::Instance();
            scene_manager.SaveSceneAs(filepath.stem().string());
            IS_CORE_DEBUG("Active Scene: {}", filepath.stem().string());
        }
    }

    void EditorLayer::ToggleFullscreen()
    {
        InsightEngine& engine = InsightEngine::Instance();
        auto const& window = engine.GetSystem<WindowSystem>("Window");
        static bool fullscreen = window->IsFullScreen();
        fullscreen = !fullscreen;
        window->SetFullScreen(fullscreen);
        IS_CORE_DEBUG("{} mode", fullscreen ? "Fullscreen" : "Windowed");
    }

    void EditorLayer::ExitProgram() { InsightEngine::Instance().Exit(); }

} // end namespace IS
