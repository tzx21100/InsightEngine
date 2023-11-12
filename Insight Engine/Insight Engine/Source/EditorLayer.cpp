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
#include "BrowserPanel.h"
#include "FileUtils.h"

// Dependencies
#include <ranges>
#include <imgui.h>
#include <imgui_internal.h>

namespace IS {

    EditorLayer::EditorLayer() : Layer("Editor Layer"), mDockspacePosition() {}

    void EditorLayer::OnAttach()
    {
        AttachPanels();
        mEditManager = std::make_unique<EditManager>();

        InsightEngine& engine = InsightEngine::Instance();
        auto asset = engine.GetSystem<AssetManager>("Asset");
        std::string ICON_DIRECTORY = AssetManager::ICON_DIRECTORY;

        // Other icons
        mIcons["PlayButton"]         = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "play_button.png")->texture_id);
        mIcons["PauseButton"]        = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "pause_button.png")->texture_id);
        mIcons["StopButton"]         = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "stop_button.png")->texture_id);
        mIcons["StepButton"]         = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "step_button.png")->texture_id);
        mIcons["TexturePlaceholder"] = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "texture_placeholder.png")->texture_id);

        // File icons
        mIcons["Folder"]             = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "folder_icon.png")->texture_id);
        mIcons["File"]               = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "default_file_icon.png")->texture_id);
        mIcons["Insight"]            = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "insight_file_icon.png")->texture_id);
        mIcons["Json"]               = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "json_file_icon.png")->texture_id);
        mIcons["MP3"]                = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "mp3_file_icon.png")->texture_id);
        mIcons["WAV"]                = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "wav_file_icon.png")->texture_id);
        mIcons["PNG"]                = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "png_file_icon.png")->texture_id);
        mIcons["JPEG"]               = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "jpeg_file_icon.png")->texture_id);

        IS_CORE_DEBUG("{} attached.", mDebugName);

    } // end OnAttach()

    void EditorLayer::OnDetach()
    {
        mPanels.Clear();
        mIcons.clear();
        IS_CORE_DEBUG("{} detached.", mDebugName);

    } // end OnDetach()

    void EditorLayer::OnUpdate(float)
    {
        auto& engine = InsightEngine::Instance();
        auto const input = engine.GetSystem<InputManager>("Input");

        // Modifier keys
        const bool CTRL_HELD    = input->IsKeyHeld(GLFW_KEY_LEFT_CONTROL) || input->IsKeyHeld(GLFW_KEY_RIGHT_CONTROL);
        const bool SHIFT_HELD   = input->IsKeyHeld(GLFW_KEY_LEFT_SHIFT) || input->IsKeyHeld(GLFW_KEY_RIGHT_SHIFT);
        const bool ALT_HELD     = input->IsKeyHeld(GLFW_KEY_LEFT_ALT) || input->IsKeyHeld(GLFW_KEY_RIGHT_ALT);

        // File
        const bool N_PRESSED    = input->IsKeyPressed(GLFW_KEY_N);
        const bool O_PRESSED    = input->IsKeyPressed(GLFW_KEY_O);
        const bool S_PRESSED    = input->IsKeyPressed(GLFW_KEY_S);
        const bool F4_PRESSED   = input->IsKeyPressed(GLFW_KEY_F4);
        const bool F11_PRESSED  = input->IsKeyPressed(GLFW_KEY_F11);

        // Edit
        const bool Z_PRESSED    = input->IsKeyPressed(GLFW_KEY_Z);
        const bool Y_PRESSED    = input->IsKeyPressed(GLFW_KEY_Y);

        if (CTRL_HELD && N_PRESSED) { mShowNewScene = true; }        // Ctrl+N
        if (CTRL_HELD && O_PRESSED) { OpenScene(); }                 // Ctrl+O
        if (CTRL_HELD && S_PRESSED) { SaveScene(); }                 // Ctrl+S
        if (CTRL_HELD && SHIFT_HELD && S_PRESSED) { SaveSceneAs(); } // Ctrl+Shift+S
        if (ALT_HELD && F4_PRESSED) { ExitProgram(); }               // Alt+F4
        if (F11_PRESSED) { ToggleFullscreen(); }                     // F11
        if (CTRL_HELD && Z_PRESSED) { mEditManager->Undo(); }        // Ctrl + Z
        if (CTRL_HELD && Y_PRESSED) { mEditManager->Redo(); }        // Ctrl + Y

        // Update panels
        for (auto& panel : mPanels)
        {
            panel->UpdatePanel();
        }

    } // end OnUpdate()

    void EditorLayer::OnRender()
    {
        ImGuiDockNodeFlags dockspace_flags = 0;
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

        // Set minimum window size within dockspace
        //ImGuiStyle& style = ImGui::GetStyle();
        //const ImVec2 default_min_window_size = style.WindowMinSize;
        //const ImVec2 min_window_size = { 350.f, 300.f };
        //ImGui::GetStyle().WindowMinSize = min_window_size;

        // Start Rendering dockspace
        if (ImGui::Begin("EditorDockSpace", nullptr, window_flags))
        {
            mDockspacePosition = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
            ImGui::PopStyleVar(3);

            ImGuiIO& io = ImGui::GetIO();

            // Enable dockspace
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("Editor");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            // Menu bar
            RenderMenuBar();

            // Tool bar
            RenderToolBar();

            // Render Panels
            for (auto& panel : mPanels)
            {
                panel->RenderPanel();
            }

            auto& hovered_entity = mPanels.Get<ScenePanel>("Scene")->mHoveredEntity;

            if (mSelectedEntity && hovered_entity && *mSelectedEntity == *hovered_entity)
            {
                Entity entity = *mSelectedEntity;
                mPanels.Get<HierarchyPanel>("Hierarchy")->RenderEntityConfig(entity);
            }
        }

        ImGui::End(); // end dockspace
        //style.WindowMinSize = default_min_window_size;

        ImGui::ShowDemoWindow();

    } // end OnRender()

    Vec2 EditorLayer::GetDockspacePosition() { return mDockspacePosition; }

    void EditorLayer::RenderMenuBar()
    {
        InsightEngine& engine = InsightEngine::Instance();
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // New File
                if (ImGui::BeginMenu("New"))
                {
                    //if (EditorUtils::RenderIconMenuItem("Scene", "Ctrl+N", mIcons["SaveFile"])) { mShowNewScene = true; }
                    if (ImGui::MenuItem(ICON_LC_FILE "  Scene", "Ctrl+N")) { mShowNewScene = true; }
                    if (ImGui::MenuItem(ICON_LC_FILE_JSON "  Script...")) { mShowNewScript = true; }
                    ImGui::EndMenu();
                } // end menu New

                // Open File
                if (ImGui::BeginMenu("Open"))
                {
                    if (ImGui::MenuItem(ICON_LC_FILE "  Scene...", "Ctrl+O")) { OpenScene(); }
                    if (ImGui::MenuItem(ICON_LC_FILE_JSON "  Script...")) { FileUtils::OpenScript(); }
                    ImGui::EndMenu();
                } // end menu Open

                ImGui::Separator();

                // Saving Scene
                if (ImGui::MenuItem(ICON_LC_SAVE "  Save Scene", "Ctrl+S")) { SaveScene(); }
                if (ImGui::MenuItem(ICON_LC_PEN_SQUARE "  Save Scene As...", "Ctrl+Shift+S")) { SaveSceneAs(); }

                ImGui::Separator();

                // Toggle fullscreen mode
                if (ImGui::MenuItem((engine.IsFullScreen() ? ICON_LC_FULLSCREEN "  Exit Fullscreen" : ICON_LC_FULLSCREEN "  Fullscreen"), "F11")) { ToggleFullscreen(); }

                ImGui::Separator();

                // Exit current program
                if (ImGui::MenuItem(ICON_LC_LOG_OUT "  Exit", "Alt+F4")) { ExitProgram(); }

                ImGui::EndMenu();
            } // end menu File

            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem(ICON_LC_UNDO "  Undo", "Ctrl+Z")) { mEditManager->Undo(); }
                if (ImGui::MenuItem(ICON_LC_REDO "  Redo", "Ctrl+Y")) { mEditManager->Redo(); }

                ImGui::EndMenu();
            } // end menu Edit

            bool open_testing = ImGui::BeginMenu("Test");

            // Tooltip
            if (!open_testing && ImGui::BeginItemTooltip())
            {
                static const float MAX_WIDTH = 200.f;
                ImGui::PushTextWrapPos(MAX_WIDTH); // Set a maximum width of 200 pixels.
                ImGui::TextUnformatted("These options are for testing purposes, not part of the engine itself.");
                ImGui::PopTextWrapPos(); // Reset text wrap width to default.
                ImGui::EndTooltip();
            }

            // Render menu Test
            if (open_testing)
            {
                if (ImGui::BeginMenu(ICON_LC_BOX "  Add Entity"))
                {
                    // Create random entities without texture
                    if (ImGui::BeginMenu(ICON_LC_PALETTE "  Random Color"))
                    {
                        if (ImGui::MenuItem("100")) { for (int i{}; i < 100; ++i) { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("500")) { for (int i{}; i < 500; ++i) { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("1,000")) { for (int i{}; i < 1'000; ++i) { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("5,000")) { for (int i{}; i < 5'000; ++i) { engine.GenerateRandomEntity(); } }
                        if (ImGui::MenuItem("10,000")) { for (int i{}; i < 10'000; ++i) { engine.GenerateRandomEntity(); } }
                        ImGui::EndMenu();
                    } // end menu Random Color

                    // Create random entities with texture
                    if (ImGui::BeginMenu(ICON_LC_IMAGE "  Random Texture"))
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
                ResetEntitySelection();
                scene_manager.NewScene(scene_name);
                IS_CORE_TRACE("Current Scene: {}", scene_name);
                
            });
        if (mShowNewScript)
            ShowCreatePopup("Create new script", "NewScript", &mShowNewScript, [&engine](const char* script_name)
            {
                engine.CreateGameScript(script_name);
                engine.OpenGameScript(script_name);
            });

    } // end RenderMenuBar()

    void EditorLayer::RenderToolBar()
    {
        auto& engine = InsightEngine::Instance();
        auto& scene_manager = SceneManager::Instance();
        const bool scene_loaded = (0 != scene_manager.GetSceneCount());

        auto& style = ImGui::GetStyle();
        const char* play_pause_button = engine.mRuntime ? "PauseButton" : "PlayButton";
        const char* play_pause_tooltip = engine.mRuntime ? "Pause" : "Play";
        const int BUTTON_COUNT = 3;
        const char* buttons[BUTTON_COUNT] = { play_pause_button, "StopButton", "StepButton" };
        const char* tooltips[BUTTON_COUNT] = { play_pause_tooltip, "Stop", "Step" };
        bool button_clicked[BUTTON_COUNT] = {};
        const ImVec2 button_size = { 16.f, 16.f };
        const ImVec4 grey_color = ImVec4(.5f, .5f, .5f, 1.f);
        const ImVec4 white_color = ImVec4(1.f, 1.f, 1.f, 1.f);
        ImVec4 tint_color = scene_manager.GetSceneCount() == 0 ? grey_color : white_color;

        ImGuiDockNodeFlags docknode_flags = ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar |
            ImGuiDockNodeFlags_NoDockingSplitMe | ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingOverOther | ImGuiDockNodeFlags_NoResize;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding;

        ImGuiWindowClass window_class;
        window_class.DockingAllowUnclassed = true;
        window_class.DockNodeFlagsOverrideSet |= docknode_flags;
        ImGui::SetNextWindowClass(&window_class);

        if (ImGui::Begin("##Runtime", nullptr, window_flags))
        {
            ImVec2 window_size = ImGui::GetContentRegionMax();
            ImVec2 start_position = ImVec2((window_size.x - (button_size.x * BUTTON_COUNT)) / 2, (window_size.y - button_size.y) / 2);

            // Render buttons
            for (int i{}; i < BUTTON_COUNT; ++i, start_position.x += button_size.x + style.ItemSpacing.x)
            {
                ImGui::SetCursorPos(start_position);

                // Grey out step button when not in game view
                if (i == BUTTON_COUNT - 1)
                {
                    tint_color = (scene_manager.GetSceneCount() > 0 && Camera3D::mActiveCamera == CAMERA_TYPE_GAME) ? white_color : grey_color;
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !(scene_manager.GetSceneCount() > 0 && Camera3D::mActiveCamera == CAMERA_TYPE_GAME));
                }

                ImGui::PushStyleColor(ImGuiCol_Text, grey_color);
                button_clicked[i] = ImGui::ImageButton(buttons[i], mIcons[buttons[i]], button_size, { 0, 0 }, { 1, 1 }, {0, 0, 0, 0}, tint_color);
                ImGui::PopStyleColor();

                if (i == BUTTON_COUNT - 1)
                {
                    ImGui::PopItemFlag();
                }
                ImGui::SetItemTooltip(tooltips[i]);
            }

            if (scene_loaded)
            {
                // Play/Pause
                if (button_clicked[0])
                {
                    engine.mRuntime = !engine.mRuntime;
                    Camera3D::mActiveCamera = CAMERA_TYPE_GAME;
                    ImGui::SetWindowFocus(ICON_LC_GAMEPAD_2 "  Game");
                    scene_manager.SaveScene();
                }

                // Stop
                if (button_clicked[1])
                {
                    engine.mRuntime = false;
                    Camera3D::mActiveCamera = CAMERA_TYPE_EDITOR;
                    ImGui::SetWindowFocus(ICON_LC_VIEW "  Scene");
                    scene_manager.ReloadActiveScene();
                }

                // Step
                if (button_clicked[2])
                {

                }
            }

            ImGui::End(); // end window Runtime
        }

    } // end RenderToolBar()

    void EditorLayer::AttachPanels()
    {
        mPanels.Emplace<GamePanel>          ("Game",        *this);
        mPanels.Emplace<ScenePanel>         ("Scene",       *this);
        mPanels.Emplace<HierarchyPanel>     ("Hierarchy",   *this);
        mPanels.Emplace<PerformancePanel>   ("Performance", *this);
        mPanels.Emplace<BrowserPanel>       ("Browser",     *this);
        mPanels.Emplace<ConsolePanel>       ("Console",     *this);
        mPanels.Emplace<InspectorPanel>     ("Inspector",   *this);
        mPanels.Emplace<SettingsPanel>      ("Settings",    *this);

    } // end AttachPanels

    void EditorLayer::ShowCreatePopup(const char* popup_name, const char* default_text, bool* show, std::function<void(const char*)> CreateAction)
    {
        ImGui::OpenPopup(popup_name);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::BeginPopupModal(popup_name, show, window_flags))
        {
            char name[256]{};
            memcpy(name, default_text, strlen(default_text));

            ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText(("##" + std::string(popup_name)).c_str(), name, sizeof(name), input_flags))
            {
                CreateAction(name);
                *show = false;
            }

            ImGui::EndPopup();
        }

    } // end ShowCreatePopup()

    void EditorLayer::OpenScene()
    {
        ResetEntitySelection();
                
        if (std::filesystem::path filepath(FileUtils::OpenFile("Insight Scene (*.insight)\0*.insight\0", "Assets\\Scene")); !filepath.empty())
        {
            auto const& relative_path = std::filesystem::relative(filepath);
            SceneManager::Instance().LoadScene(relative_path.string());
            IS_CORE_DEBUG("Active Scene: {}", relative_path.string());
        }

    } // end OpenScene()

    void EditorLayer::OpenScene(std::string const& path)
    {
        std::filesystem::path filepath(path);
        SceneManager::Instance().LoadScene(filepath.string());
        IS_CORE_DEBUG("Active Scene: {}", filepath.stem().string());

    } // end OpenScene()

    void EditorLayer::AcceptAssetBrowserPayload()
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
            {
                std::filesystem::path path = static_cast<wchar_t*>(payload->Data);
                ResetEntitySelection();
                OpenScene(path.string());
            }
            ImGui::EndDragDropTarget();
        }

    } // end AcceptAssetBrowserPayload()

    void EditorLayer::SaveScene()  { SceneManager::Instance().SaveScene(); }

    void EditorLayer::SaveSceneAs()
    {
        if (std::filesystem::path filepath(FileUtils::SaveFile("Insight Scene (*.insight)\0*.insight\0", "Assets\\Scene")); !filepath.empty())
        {
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
