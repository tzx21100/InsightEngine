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
#include <ImGuizmo.h>

namespace IS {

    EditorLayer::EditorLayer() : Layer("Editor Layer"), mDockspacePosition() {}

    void EditorLayer::OnAttach()
    {
        AttachPanels();
        InsightEngine& engine = InsightEngine::Instance();
        auto asset = engine.GetSystem<AssetManager>("Asset");
        std::string ICON_DIRECTORY = AssetManager::ICON_DIRECTORY;

        mIcons["PlayButton"]         = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "play_button.png")->texture_id);
        mIcons["PauseButton"]        = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "pause_button.png")->texture_id);
        mIcons["StopButton"]         = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "stop_button.png")->texture_id);
        mIcons["StepButton"]         = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "step_button.png")->texture_id);
        mIcons["SaveFile"]           = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "save_button.png")->texture_id); // TODO: use icon font instead
        mIcons["ZoomIn"]             = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "zoom_in.png")->texture_id);
        mIcons["ZoomOut"]            = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "zoom_out.png")->texture_id);
        mIcons["Folder"]             = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "folder_icon.png")->texture_id);
        mIcons["File"]               = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "file_icon.png")->texture_id);
        mIcons["BackButton"]         = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "back_icon.png")->texture_id);
        mIcons["TexturePlaceholder"] = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "texture_placeholder.png")->texture_id);

        IS_CORE_DEBUG("{} attached", GetName());
    }

    void EditorLayer::OnDetach()
    {
        mPanels.clear();
        IS_CORE_DEBUG("{} detached", GetName());
    }

    void EditorLayer::OnUpdate(float dt)
    {
        if (auto const& [fb_width, fb_height] = ISGraphics::mFramebuffer->GetSize();
            mScenePanel->GetViewportSize().x > 0.f && mScenePanel->GetViewportSize().y > 0.f &&
            (fb_width != mScenePanel->GetViewportSize().x || fb_height != mScenePanel->GetViewportSize().y))
        {
            ISGraphics::ResizeFramebuffer(static_cast<GLuint>(mScenePanel->GetViewportSize().x), static_cast<GLuint>(mScenePanel->GetViewportSize().y));
        }

        InsightEngine& engine = InsightEngine::Instance();
        auto const input = engine.GetSystem<InputManager>("Input");

        // Shortcuts
        const bool CTRL_HELD       = input->IsKeyHeld(GLFW_KEY_LEFT_CONTROL) || input->IsKeyHeld(GLFW_KEY_RIGHT_CONTROL);
        const bool SHIFT_HELD      = input->IsKeyHeld(GLFW_KEY_LEFT_SHIFT)   || input->IsKeyHeld(GLFW_KEY_RIGHT_SHIFT);
        const bool ALT_HELD        = input->IsKeyHeld(GLFW_KEY_LEFT_ALT)     || input->IsKeyHeld(GLFW_KEY_RIGHT_ALT);
        const bool N_PRESSED       = input->IsKeyPressed(GLFW_KEY_N);
        const bool O_PRESSED       = input->IsKeyPressed(GLFW_KEY_O);
        const bool S_PRESSED       = input->IsKeyPressed(GLFW_KEY_S);
        const bool F4_PRESSED      = input->IsKeyPressed(GLFW_KEY_F4);
        const bool F11_PRESSED     = input->IsKeyPressed(GLFW_KEY_F11);

        if (CTRL_HELD && N_PRESSED) { mShowNewScene = true; }        // Ctrl+N
        if (CTRL_HELD && O_PRESSED) { OpenScene(); }                 // Ctrl+O
        if (CTRL_HELD && S_PRESSED) { SaveScene(); }                 // Ctrl+S
        if (CTRL_HELD && SHIFT_HELD && S_PRESSED) { SaveSceneAs(); } // Ctrl+Shift+S
        if (ALT_HELD && F4_PRESSED) { ExitProgram(); }               // Alt+F4
        if (F11_PRESSED) { ToggleFullscreen(); }                     // F11

        // Controls for scene panel
        if (mScenePanel->IsFocused())
        {
            Camera::mActiveCamera = CAMERA_TYPE_EDITOR;
            
            // Right mouse drag to pan camera
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
            {
                ImVec2 mouse_position = ImGui::GetMousePos();
                Camera& camera = ISGraphics::cameras[Camera::mActiveCamera];
                auto const& [x_drag, y_drag] = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
                camera.PanCamera(dt, x_drag, y_drag);
            }
        }

        // Auto pause game if game panel is in focus
        if (!mGamePanel->IsFocused() && Camera::mActiveCamera == CAMERA_TYPE_EDITOR)
        {
            engine.mRuntime = false;

            auto [mx, my] = ImGui::GetMousePos();
            mx -= mScenePanel->GetViewportBounds()[0].x;
            my -= mScenePanel->GetViewportBounds()[0].y;
            Vec2 viewportSize = mScenePanel->GetViewportBounds()[1] - mScenePanel->GetViewportBounds()[0];
            my = viewportSize.y - my;
            int mouse_x = static_cast<int>(mx);
            int mouse_y = static_cast<int>(my);

            // Check if mouse is within bounds of the scene panel for mouse picking.
            if (0 <= mouse_x && mouse_x < static_cast<int>(viewportSize.x) &&
                0 <= mouse_y && mouse_y < static_cast<int>(viewportSize.y))
            {
               

                // Set hovered entity as the selected entity
                if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                    int pixel_data = ISGraphics::mFramebuffer->ReadPixel(mouse_x, mouse_y);
                    mHoveredEntity = (pixel_data == -1) ? nullptr : std::make_shared<Entity>(pixel_data);
                    mSceneHierarchyPanel->SetSelectedEntity(mHoveredEntity);
                }
                else if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                    int pixel_data = ISGraphics::mFramebuffer->ReadPixel(mouse_x, mouse_y);
                    mHoveredEntity = (pixel_data == -1) ? nullptr : std::make_shared<Entity>(pixel_data);

                    // need to add if hovering on top of selected but
                    // mHoveredEntity == mSceneHierarchyPanel->GetSelectedEntity() is not working
                    if (mSceneHierarchyPanel->GetSelectedEntity() != nullptr) { // if left click held on selected entity

                        Vec2D mousePosChange = { // need to GetMousePosition() once to update the previous and current values
                            static_cast<float>(input->GetMousePosition().first - input->previousWorldMousePos.x),
                            static_cast<float>(input->currentWorldMousePos.y - input->previousWorldMousePos.y) };

                        Entity entity = *mSceneHierarchyPanel->GetSelectedEntity();

                        if (engine.HasComponent<Transform>(entity)) {
                            auto& transform = engine.GetComponent<Transform>(entity);
                            transform.world_position.x += mousePosChange.x;
                            transform.world_position.y += mousePosChange.y;
                        }
                    }
                }
                    
            }
        }
    }

    void EditorLayer::OnRender()
    {
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoResize; // disable resizing of panels in dockspace
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
        //ImGuiStyle& style = ImGui::GetStyle();
        //ImVec2 min_window_size = style.WindowMinSize;
        //style.WindowMinSize = ImVec2(350.f, 300.f);

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
            panel->RenderPanel();

        //RenderGizmo();

        //style.WindowMinSize = min_window_size;

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
                    //if (EditorUtils::RenderIconMenuItem("Scene", "Ctrl+N", mIcons["SaveFile"])) { mShowNewScene = true; }
                    if (ImGui::MenuItem("Scene", "Ctrl+N")) 
                    {
                        mShowNewScene = true;
                    }
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
                ImGui::TextUnformatted("These options are for testing purposes, not part of the engine itself.");
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
                scene_manager.NewScene(scene_name);
                IS_CORE_TRACE("Current Scene: {}", scene_name);
                
            });
        if (mShowNewScript)
            ShowCreatePopup("Create new script", "NewScript", &mShowNewScript, [&engine](const char* script_name)
            {
                engine.CreateGameScript(script_name);
                engine.OpenGameScript(script_name);
            });
    }

    void EditorLayer::RenderToolBar()
    {
        auto& engine = InsightEngine::Instance();
        auto& scene_manager = SceneManager::Instance();
        auto input = engine.GetSystem<InputManager>("Input");
        const bool scene_loaded = (0 != scene_manager.GetSceneCount());

        auto& style = ImGui::GetStyle();
        const char* play_pause_button = engine.mRuntime ? "PauseButton" : "PlayButton";
        const int BUTTON_COUNT = 3;
        const char* buttons[BUTTON_COUNT] = { play_pause_button, "StopButton", "StepButton" };
        bool button_clicked[BUTTON_COUNT] = {};
        const ImVec2 button_size = { 16.f, 16.f };

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse;
        if (ImGui::Begin("##Runtime", nullptr, window_flags))
        {
            ImVec2 window_size = ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y);
            ImVec2 start_position = ImVec2((window_size.x - (button_size.x * BUTTON_COUNT)) / 2, (window_size.y - button_size.y) / 2);

            // Remove button background
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

            // Render buttons
            for (int i{}; i < BUTTON_COUNT; ++i, start_position.x += button_size.x + style.ItemSpacing.x)
            {
                ImGui::SetCursorPos(start_position);
                button_clicked[i] = ImGui::ImageButton(buttons[i], mIcons[buttons[i]], button_size);
            }

            ImGui::PopStyleColor();

            // Play/Pause
            if (scene_loaded)
            {
                if (button_clicked[0])
                {
                    engine.mRuntime = !engine.mRuntime;
                    Camera::mActiveCamera = CAMERA_TYPE_SCENE;
                    ImGui::SetWindowFocus("Game");
                    IS_CORE_DEBUG("Button 0");
                }

                // Stop
                if (button_clicked[1])
                {
                    engine.mRuntime = false;
                    Camera::mActiveCamera = CAMERA_TYPE_EDITOR;
                    ImGui::SetWindowFocus("Scene");
                    scene_manager.ReloadActiveScene();
                    IS_CORE_DEBUG("Button 1");
                }

                // Step
                if (button_clicked[2])
                {
                    IS_CORE_DEBUG("Button 2");
                }
            }

            ImGui::End(); // end window Runtime
        }
    }

    void EditorLayer::AttachPanels()
    {
        mScenePanel = std::make_shared<ScenePanel>();
        mGamePanel = std::make_shared<GamePanel>();
        mSceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>();
        mLogConsolePanel = std::make_shared<LogConsolePanel>();

        mPanels.emplace_back(mGamePanel);
        mPanels.emplace_back(mScenePanel);
        mPanels.emplace_back(std::make_shared<PhysicsControlPanel>());
        mPanels.emplace_back(mSceneHierarchyPanel);
        mPanels.emplace_back(std::make_shared<InspectorPanel>(mSceneHierarchyPanel));
        mPanels.emplace_back(std::make_shared<PerformancePanel>());
        mPanels.emplace_back(std::make_shared<AssetBrowserPanel>());
        mPanels.emplace_back(mLogConsolePanel);
    }

    //void EditorLayer::RenderGizmo()
    //{
    //    if (!mSceneHierarchyPanel->GetSelectedEntity())
    //        return;

    //    //Entity selected_entity = *mSceneHierarchyPanel->GetSelectedEntity();

    //    ImGuizmo::SetOrthographic(true);
    //    ImGuizmo::SetDrawlist();

    //    ImGuizmo::SetRect(mScenePanel->GetViewportBounds()[0].x, mScenePanel->GetViewportBounds()[0].y,
    //                      mScenePanel->GetViewportBounds()[1].x - mScenePanel->GetViewportBounds()[0].x,
    //                      mScenePanel->GetViewportBounds()[1].y - mScenePanel->GetViewportBounds()[0].y);

    //    //Camera& camera = ISGraphics::cameras[Camera::mActiveCamera];
    //    //glm::mat3 view_matrix = camera.xform;
    //    ImGuizmo::SetOrthographic(true);
    //}

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
    }

    void EditorLayer::OpenScene()
    {
        mSceneHierarchyPanel->ResetSelection();
                
        if (std::filesystem::path filepath(FileUtils::OpenFile("Insight Scene (*.insight)\0*.insight\0", "Assets\\Scene")); !filepath.empty())
        {
            auto const& relative_path = std::filesystem::relative(filepath);
            SceneManager::Instance().LoadScene(relative_path.string());
            IS_CORE_DEBUG("Active Scene: {}", relative_path.string());
        }
    }

    void EditorLayer::OpenScene(std::string const& path)
    {
        std::filesystem::path filepath(path);
        SceneManager::Instance().LoadScene(filepath.string());
        IS_CORE_DEBUG("Active Scene: {}", filepath.stem().string());
    }

    bool EditorLayer::IsGamePanelFocused() const { return mGamePanel->IsFocused(); }

    bool EditorLayer::IsScenePanelFocused() const { return mScenePanel->IsFocused(); }

    ImTextureID EditorLayer::GetIcon(const char* icon) const { return mIcons.at(icon); }

    void EditorLayer::RenderSelectedEntityOutline() const { mSceneHierarchyPanel->RenderSelectedEntityOutline(); }

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

    Vec2 EditorLayer::GetViewportSize() {
        //IS_CORE_DEBUG("VP X: {}, VP Y: {}", mScenePanel->GetViewportSize().x, mScenePanel->GetViewportSize().y);
        return { mScenePanel->GetViewportSize().x , mScenePanel->GetViewportSize().y };
    }

} // end namespace IS
