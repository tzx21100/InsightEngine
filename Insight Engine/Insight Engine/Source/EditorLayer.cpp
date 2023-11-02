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

        // TODO: use icon font instead
        mIcons["SaveFile"]           = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "save_button.png")->texture_id);

        // Other icons
        mIcons["PlayButton"]         = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "play_button.png")->texture_id);
        mIcons["PauseButton"]        = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "pause_button.png")->texture_id);
        mIcons["StopButton"]         = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "stop_button.png")->texture_id);
        mIcons["StepButton"]         = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "step_button.png")->texture_id);
        mIcons["ZoomIn"]             = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "zoom_in.png")->texture_id);
        mIcons["ZoomOut"]            = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "zoom_out.png")->texture_id);
        mIcons["BackButton"]         = EditorUtils::ConvertTextureID(asset->GetIcon(ICON_DIRECTORY + "back_icon.png")->texture_id);
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

        IS_CORE_DEBUG("{} attached", GetName());
    }

    void EditorLayer::OnDetach()
    {
        mPanels.clear();
        IS_CORE_DEBUG("{} detached", GetName());
    }

    void EditorLayer::OnUpdate(float)
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
        const bool CTRL_HELD    = input->IsKeyHeld(GLFW_KEY_LEFT_CONTROL) || input->IsKeyHeld(GLFW_KEY_RIGHT_CONTROL);
        const bool SHIFT_HELD   = input->IsKeyHeld(GLFW_KEY_LEFT_SHIFT) || input->IsKeyHeld(GLFW_KEY_RIGHT_SHIFT);
        const bool ALT_HELD     = input->IsKeyHeld(GLFW_KEY_LEFT_ALT) || input->IsKeyHeld(GLFW_KEY_RIGHT_ALT);
        const bool N_PRESSED    = input->IsKeyPressed(GLFW_KEY_N);
        const bool O_PRESSED    = input->IsKeyPressed(GLFW_KEY_O);
        const bool S_PRESSED    = input->IsKeyPressed(GLFW_KEY_S);
        const bool F4_PRESSED   = input->IsKeyPressed(GLFW_KEY_F4);
        const bool F11_PRESSED  = input->IsKeyPressed(GLFW_KEY_F11);

        if (CTRL_HELD && N_PRESSED) { mShowNewScene = true; }        // Ctrl+N
        if (CTRL_HELD && O_PRESSED) { OpenScene(); }                 // Ctrl+O
        if (CTRL_HELD && S_PRESSED) { SaveScene(); }                 // Ctrl+S
        if (CTRL_HELD && SHIFT_HELD && S_PRESSED) { SaveSceneAs(); } // Ctrl+Shift+S
        if (ALT_HELD && F4_PRESSED) { ExitProgram(); }               // Alt+F4
        if (F11_PRESSED) { ToggleFullscreen(); }                     // F11

        // Auto pause game if game panel is not in focus
        if (!mGamePanel->IsFocused() && Camera::mActiveCamera == CAMERA_TYPE_EDITOR) { engine.mRuntime = false; }

        // Controls for scene panel
        if (mScenePanel->IsFocused())
        {
            // Set active camera to editor camera
            Camera::mActiveCamera = CAMERA_TYPE_EDITOR;

            auto [mx, my] = ImGui::GetMousePos();
            mx -= mScenePanel->GetViewportBounds()[0].x;
            my -= mScenePanel->GetViewportBounds()[0].y;
            Vec2 viewportSize = mScenePanel->GetViewportBounds()[1] - mScenePanel->GetViewportBounds()[0];
            my = viewportSize.y - my;
            int mouse_x = static_cast<int>(mx);
            int mouse_y = static_cast<int>(my);

            // Check if mouse is within bounds of the scene panel
            if (0 <= mouse_x && mouse_x < static_cast<int>(viewportSize.x) &&
                0 <= mouse_y && mouse_y < static_cast<int>(viewportSize.y))
            {
                ZoomCamera();
                PanCamera();

                // Mouse picking - set hovered entity as the selected entity
                if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                {
                    int pixel_data = ISGraphics::mFramebuffer->ReadPixel(mouse_x, mouse_y);
                    mHoveredEntity = (pixel_data == -1) ? nullptr : std::make_shared<Entity>(pixel_data);
                    mSceneHierarchyPanel->SetSelectedEntity(mHoveredEntity);
                }

                // Mouse dragging - change selected/hovered entity translation
                else if (mSceneHierarchyPanel->GetSelectedEntity() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    Vec2D mouse_position_delta = { // need to GetMousePosition() once to update the previous and current values
                        static_cast<float>(input->GetMousePosition().first - input->previousWorldMousePos.x),
                        static_cast<float>(input->currentWorldMousePos.y - input->previousWorldMousePos.y) };

                    Entity entity = *mSceneHierarchyPanel->GetSelectedEntity();

                    // Translate entity position
                    if (engine.HasComponent<Transform>(entity))
                    {
                        auto& transform = engine.GetComponent<Transform>(entity);
                        transform.world_position.x += mouse_position_delta.x;
                        transform.world_position.y += mouse_position_delta.y;
                    }
                }

            }
        }

        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            mHoveredEntity = {};
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

        if (mSceneHierarchyPanel->GetSelectedEntity() && mHoveredEntity &&
            *mHoveredEntity == *mSceneHierarchyPanel->GetSelectedEntity())
        {
            Entity entity = *mSceneHierarchyPanel->GetSelectedEntity();
            mSceneHierarchyPanel->RenderEntityConfig(entity);
        }

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
        const char* play_pause_tooltip = engine.mRuntime ? "Pause" : "Play";
        const int BUTTON_COUNT = 3;
        const char* buttons[BUTTON_COUNT] = { play_pause_button, "StopButton", "StepButton" };
        const char* tooltips[BUTTON_COUNT] = { play_pause_tooltip, "Stop", "Step" };
        bool button_clicked[BUTTON_COUNT] = {};
        const ImVec2 button_size = { 16.f, 16.f };
        const ImVec4 grey_color = ImVec4(.5f, .5f, .5f, 1.f);
        const ImVec4 white_color = ImVec4(1.f, 1.f, 1.f, 1.f);
        ImVec4 tint_color = scene_manager.GetSceneCount() == 0 ? grey_color : white_color;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding;
        if (ImGui::Begin("##Runtime", nullptr, window_flags))
        {
            ImVec2 window_size = ImGui::GetContentRegionMax();
            ImVec2 start_position = ImVec2((window_size.x - (button_size.x * BUTTON_COUNT)) / 2, (window_size.y - button_size.y) / 2);

            ImGui::SetCursorPosY(start_position.y + style.FramePadding.y);
            ImGui::TextUnformatted("Text Animation");
            ImGui::SameLine();
            ImGui::SetCursorPosY(start_position.y);
            ImGui::Checkbox("##Text Animation", &ISGraphics::mShowTextAnimation);

            // Render buttons
            for (int i{}; i < BUTTON_COUNT; ++i, start_position.x += button_size.x + style.ItemSpacing.x)
            {
                ImGui::SetCursorPos(start_position);

                // Grey out step button when not in game view
                if (i == BUTTON_COUNT - 1)
                {
                    tint_color = (scene_manager.GetSceneCount() > 0 && Camera::mActiveCamera == CAMERA_TYPE_GAME) ? white_color : grey_color;
                }

                button_clicked[i] = ImGui::ImageButton(buttons[i], mIcons[buttons[i]], button_size, { 0, 0 }, { 1, 1 }, {0, 0, 0, 0}, tint_color);
                ImGui::SetItemTooltip(tooltips[i]);
            }

            // Render camera zoom controls
            RenderCameraControls();

            if (scene_loaded)
            {
                // Play/Pause
                if (button_clicked[0])
                {
                    engine.mRuntime = !engine.mRuntime;
                    Camera::mActiveCamera = CAMERA_TYPE_GAME;
                    ImGui::SetWindowFocus("Game");
                }

                // Stop
                if (button_clicked[1])
                {
                    engine.mRuntime = false;
                    Camera::mActiveCamera = CAMERA_TYPE_EDITOR;
                    ImGui::SetWindowFocus("Scene");
                    scene_manager.ReloadActiveScene();
                }

                // Step
                if (button_clicked[2])
                {

                }
            }

            ImGui::End(); // end window Runtime
        }
    }

    void EditorLayer::RenderCameraControls()
    {
        auto& camera = ISGraphics::cameras[Camera::mActiveCamera];
        const float SIZE = 16.f;
        float zoom_level = camera.GetZoomLevel();

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.f * ImGui::GetStyle().ItemSpacing.x);
        if (ImGui::BeginTable("Camera Controls", 6, ImGuiTableFlags_Resizable))
        {
            ImGui::TableNextColumn();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
            ImGui::TextUnformatted("Zoom");
            ImGui::SetItemTooltip("Adjust camera zoom level");

            // Zoom out with - button
            ImGui::TableNextColumn();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::ImageButton("ZoomOut", mIcons.at("ZoomOut"), { SIZE, SIZE }))
            {
                zoom_level *= (1 - Camera::mZoomSpeed);
                camera.SetZoomLevel(zoom_level);
            }
            ImGui::PopStyleColor();
            ImGui::SetItemTooltip("Zooms out camera");

            // Slider to adjust camera zoom
            ImGui::SameLine();
            ImGui::SetNextItemWidth(SIZE * 10.f);
            if (ImGui::SliderFloat("##CameraZoomSlider", &zoom_level,
                                   (Camera::CAMERA_ZOOM_MIN), (Camera::CAMERA_ZOOM_MAX), "%.2fx", ImGuiSliderFlags_Logarithmic))
            {
                // Update the camera's zoom level directly
                camera.SetZoomLevel(zoom_level);
            }

            // Zoom in with + button
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::ImageButton("ZoomIn", mIcons.at("ZoomIn"), { SIZE, SIZE }))
            {
                zoom_level *= (1 + Camera::mZoomSpeed);
                camera.SetZoomLevel(zoom_level);
            }
            ImGui::PopStyleColor();
            ImGui::SetItemTooltip("Zooms in camera");

            // Zoom speed
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Zoom Speed");
            ImGui::SetItemTooltip("Adjust zoom speed of camera");
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(SIZE * 4.f);
            ImGui::SliderFloat("##CameraZoomSpeed", &Camera::mZoomSpeed, Camera::CAMERA_ZOOM_SPEED_MIN, Camera::CAMERA_ZOOM_SPEED_MAX, "%.2f");

            // Pan speed
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Pan Speed");
            ImGui::SetItemTooltip("Adjust pan speed of camera");
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(SIZE * 4.f);
            ImGui::SliderFloat("##CameraPanSpeed", &Camera::mMoveSpeed, Camera::CAMERA_MOVE_SPEED_MIN, Camera::CAMERA_MOVE_SPEED_MAX, "%.2f");

            ImGui::EndTable(); // end table Camera Controls
        }

        //ImGui::TextUnformatted("Camera:");

        //// Zoom out with - button
        //ImGui::SameLine();
        //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        //if (ImGui::ImageButton("ZoomOut", mIcons.at("ZoomOut"), { SIZE, SIZE }))
        //{
        //    zoom_level *= (1 - Camera::mZoomSpeed);
        //    camera.SetZoomLevel(zoom_level);
        //}
        //ImGui::PopStyleColor();

        //// Slider to adjust camera zoom
        //ImGui::SameLine();
        //ImGui::SetNextItemWidth(SIZE * 10.f);
        //if (ImGui::SliderFloat("##CameraZoomSlider", &zoom_level,
        //                        (Camera::mMinZoom), (Camera::mMaxZoom), "%.2fx", ImGuiSliderFlags_Logarithmic))
        //{
        //    // Update the camera's zoom level directly
        //    camera.SetZoomLevel(zoom_level);
        //}

        //// Zoom in with + button
        //ImGui::SameLine();
        //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        //if (ImGui::ImageButton("ZoomIn", mIcons.at("ZoomIn"), { SIZE, SIZE }))
        //{
        //    zoom_level *= (1 + Camera::mZoomSpeed);
        //    camera.SetZoomLevel(zoom_level);
        //}
        //ImGui::PopStyleColor();

        //// Zoom speed
        //ImGui::SameLine();
        //ImGui::TextUnformatted("Zoom Speed:");
        //ImGui::SameLine();
        //ImGui::SetNextItemWidth(SIZE * 4.f);
        //ImGui::SliderFloat("##CameraZoomSpeed", &Camera::mZoomSpeed, 0.01f, 0.2f, "%.2f");
        //ImGui::SameLine();

        //// Pan speed
        //ImGui::SameLine();
        //ImGui::TextUnformatted("Pan Speed:");
        //ImGui::SameLine();
        //ImGui::SetNextItemWidth(SIZE * 4.f);
        //ImGui::SliderFloat("##CameraPanSpeed", &Camera::mMoveSpeed, 1.f, 10.f, "%.2f");
        //ImGui::SameLine();
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

    void EditorLayer::ZoomCamera()
    {
        Camera& camera = ISGraphics::cameras[CAMERA_TYPE_EDITOR];
        ImGuiIO& io = ImGui::GetIO();
        float scroll_delta = io.MouseWheel;
        float zoom_level = camera.GetZoomLevel();
        if (scroll_delta != 0)
        {
            zoom_level *= (scroll_delta > 0) ? (1 + Camera::mZoomSpeed) : (1 - Camera::mZoomSpeed);
        }
        camera.SetZoomLevel(zoom_level);
    }

    void EditorLayer::PanCamera()
    {
        static bool is_panning = false;
        static ImVec2 previous_mouse_position;
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 mouse_position = io.MousePos;
        Camera& camera = ISGraphics::cameras[CAMERA_TYPE_EDITOR];

        if (!io.MouseDown[1])
        {
            is_panning = false;
            return;
        }

        if (!is_panning)
        {
            is_panning = true;
            previous_mouse_position = mouse_position;
        }

        // Calculate the panning offset
        ImVec2 delta = ImVec2(mouse_position.x - previous_mouse_position.x, mouse_position.y - previous_mouse_position.y);
        camera.PanCamera(delta.x, delta.y);
        previous_mouse_position = mouse_position;
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

    void EditorLayer::AcceptAssetBrowserPayload()
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
            {
                std::filesystem::path path = static_cast<wchar_t*>(payload->Data);
                mSceneHierarchyPanel->ResetSelection();
                OpenScene(path.string());
            }
            ImGui::EndDragDropTarget();
        }
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
