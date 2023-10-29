/*!
 * \file Panel.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 17-10-2023
 * \brief
 * This source file defines the implementation for parent class Panel and its
 * derived classes, which encapsulate the functionalities of a panel in a dockspace.
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
#include "Panel.h"
#include "EditorUtils.h"
#include "EditorLayer.h"
#include "FileUtils.h"
#include "Editor.h"

// Dependencies
#include <imgui.h>

namespace IS {

    void GamePanel::RenderPanel()
    {
        auto& engine = InsightEngine::Instance();
        auto input = engine.GetSystem<InputManager>("Input");
        auto editor = engine.GetSystem<Editor>("Editor");
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
        ImGui::Begin("Game", nullptr, window_flags);

        // Allow key/mouse event pass through only in this panel
        mFocused = ImGui::IsWindowFocused();
        if (mFocused)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.WantCaptureMouse = io.WantCaptureKeyboard = false;
        }

        // Size of scene panel
        ImVec2 scene_size = ImGui::GetWindowSize();
        ImVec2 scene_pos = ImGui::GetWindowPos();

        // Scene pos for the input
        //ImVec2 actual_scene_pos;
        //actual_scene_pos.x = scene_pos.x - editor->GetEditorLayer()->GetDockspacePosition().x;
        //actual_scene_pos.y = scene_pos.y - editor->GetEditorLayer()->GetDockspacePosition().y;
        //input->setCenterPos(actual_scene_pos.x + (float)scene_size.x / 2.f,
        //                    actual_scene_pos.y + (float)scene_size.y / 2.f);
        //input->setRatio(scene_size.x, scene_size.y);

        // Resize framebuffer
        ImVec2 panel_size = ImGui::GetContentRegionAvail();
        mViewportSize = { panel_size.x, panel_size.y };

        // Display actual scene
        ImGui::Image(EditorUtils::ConvertTextureID(ISGraphics::GetScreenTexture()), panel_size, { 0, 1 }, { 1, 0 });

        ImGui::End(); // end window Scene
        ImGui::PopStyleVar();
    }

    bool GamePanel::IsFocused() const { return mFocused; }

    void ScenePanel::RenderPanel()
    {
        auto& engine = InsightEngine::Instance();
        auto input = engine.GetSystem<InputManager>("Input");
        auto editor = engine.GetSystem<Editor>("Editor");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
        ImGui::Begin("Scene", nullptr, window_flags);

        auto viewport_lower_bound = ImGui::GetWindowContentRegionMin();
        auto viewport_upper_bound = ImGui::GetWindowContentRegionMax();
        auto viewport_offset      = ImGui::GetWindowPos();
        mViewportBounds[0]        = { viewport_lower_bound.x + viewport_offset.x, viewport_lower_bound.y + viewport_offset.y };
        mViewportBounds[1]        = { viewport_upper_bound.x + viewport_offset.x, viewport_upper_bound.y + viewport_offset.y };

        // Allow key/mouse event pass through only in this panel
        mFocused = ImGui::IsWindowFocused();
        if (mFocused)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.WantCaptureMouse = io.WantCaptureKeyboard = false;
        }

        // Size of scene panel
        ImVec2 scene_size = ImGui::GetWindowSize();
        ImVec2 scene_pos = ImGui::GetWindowPos();
       

        // Scene pos for the input
        ImVec2 actual_scene_pos;
        actual_scene_pos.x = scene_pos.x - editor->GetEditorLayer()->GetDockspacePosition().x;
        actual_scene_pos.y = scene_pos.y - editor->GetEditorLayer()->GetDockspacePosition().y;
        //auto& camera = ISGraphics::cameras[Camera::mActiveCamera];
        //input->setCenterPos(scene_size.x - camera.GetCamPos().x, scene_size.y - camera.GetCamPos().y);
        //input->setRatio(camera.GetCamDim().x, camera.GetCamDim().y);
        //IS_CORE_DEBUG("center x : {}, center y : {}", camera.GetCamPos().x, camera.GetCamPos().y);
        //input->setCenterPos(scene_size.x - ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().x, scene_size.y - ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().y);
        input->setCenterPos(actual_scene_pos.x + (float)scene_size.x / 2.f, actual_scene_pos.y + (float)scene_size.y / 2.f );
        input->setRatio(scene_size.x, scene_size.y);
        //IS_CORE_DEBUG("{}, {}", ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().x, ISGraphics::cameras[Camera::mActiveCamera].GetCamPos().y);

        //input->setRatio(ISGraphics::cameras[Camera::mActiveCamera].GetCamDim().x * , ISGraphics::cameras[Camera::mActiveCamera].GetCamDim().y);
        IS_CORE_DEBUG("x:{}, y:{}", input->GetMousePosition().first, input->GetMousePosition().second);

        // Resize framebuffer
        ImVec2 panel_size = ImGui::GetContentRegionAvail();
        mViewportSize = { panel_size.x, panel_size.y };
        
        // Display actual scene
        ImGui::Image(EditorUtils::ConvertTextureID(ISGraphics::GetScreenTexture()), panel_size, { 0, 1 }, { 1, 0 });

        // Help tooltip
        if (Camera::mActiveCamera == CAMERA_TYPE_EDITOR)
        {
            RenderHelp();
            RenderCameraZoom();
        }

        ImGui::End(); // end window Scene
        ImGui::PopStyleVar();
    }

    bool ScenePanel::IsFocused() const { return mFocused; }

    void ScenePanel::RenderHelp()
    {
        ImDrawList* window_drawlist = ImGui::GetWindowDrawList();
        ImFont* const FONT_BOLD = ImGui::GetIO().Fonts->Fonts[FONT_TYPE_BOLD];
        ImVec2 mouse_pos = ImGui::GetMousePos();

        // Attributes of circle
        const float CIRCLE_RADIUS = 20.f;
        ImVec2 window_pos = ImGui::GetWindowPos();
        float tab_height = ImGui::GetTextLineHeightWithSpacing() + ImGui::GetFontSize();
        ImVec2 circle_center = { window_pos.x + CIRCLE_RADIUS + tab_height / 2.f, window_pos.y + CIRCLE_RADIUS + tab_height };

        // Calculate the position for the text (center of the circle)
        const char* display_text = "Help";
        ImVec2 text_position = ImVec2(circle_center.x - ImGui::CalcTextSize(display_text).x * 0.5f, circle_center.y - ImGui::GetTextLineHeight() * 0.5f);

        // Check if the circle is being hovered
        if (!ImGui::IsAnyItemHovered() && EditorUtils::TestPointCircle(mouse_pos, circle_center, CIRCLE_RADIUS))
        {
            const ImVec2 TOOLTIP_SIZE = { 400.f, 0.f };

            // Create a custom tooltip window
            ImGui::SetNextWindowSize(TOOLTIP_SIZE);
            if (ImGui::BeginTooltip())
            {
                // Set padding and indetation
                const float PADDING = 10.f;
                ImGui::Spacing();
                ImGui::Indent(PADDING);

                // Tooltip content
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("The following controls ONLY work if scene panel focused!");
                ImGui::PopFont();

                ImGui::Separator();

                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("General Controls");
                ImGui::PopFont();
                ImGui::BulletText("Press 'Tab' to toggle GUI");
                ImGui::BulletText("Click mouse scrollwheel to spawn entity");
                ImGui::BulletText("Click right mouse button to spawn rigidbody entity");

                ImGui::Separator();

                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Camera Controls");
                ImGui::PopFont();
                ImGui::BulletText("Hold Ctrl+Scroll to zoom in/out");
                ImGui::BulletText("Drag mouse to move around");

                ImGui::Separator();

                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Player Controls");
                ImGui::PopFont();
                ImGui::BulletText("Press 'WASD' to move in the four directions");
                ImGui::BulletText("Press 'Q' to rotate clockwise, 'E' to rotate counter-clockwise");

                ImGui::Separator();

                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Physics Controls");
                ImGui::PopFont();
                ImGui::BulletText("Press '2' to enable draw collision boxes, '1' to disable");
                ImGui::BulletText("Press 'G' to enable gravity, 'F' to disable");
                ImGui::BulletText("Press 'Shift' + 'Enter' to freeze frame, 'Enter' to step frame");

                ImGui::Separator();

                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Audio Controls");
                ImGui::PopFont();
                ImGui::BulletText("Press 'Z' to play sfx");
                ImGui::BulletText("Press 'X' to play music");

                // Reset padding and indentation
                ImGui::Unindent(PADDING);
                ImGui::Spacing();

                ImGui::EndTooltip();
            }
        }
        
        // Icon to be displayed as overlay
        window_drawlist->AddCircleFilled(circle_center, CIRCLE_RADIUS, IM_COL32(255, 255, 255, 50));
        window_drawlist->AddCircle(circle_center, CIRCLE_RADIUS, IM_COL32_WHITE);
        window_drawlist->AddText(text_position, IM_COL32_WHITE, display_text);
    }

    void ScenePanel::RenderCameraZoom()
    {
        auto& engine = InsightEngine::Instance();
        auto editor = engine.GetSystem<Editor>("Editor");
        auto& camera = ISGraphics::cameras[Camera::mActiveCamera];
        auto& style = ImGui::GetStyle();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
        ImGui::SetNextWindowBgAlpha(0.f);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
        ImGui::Begin("##Camera Zoom", nullptr, window_flags);

        const float size = 16.f;
        const float xpos = (ImGui::GetContentRegionAvail().x - size - style.ItemSpacing.x) / 2.f;

        ImGui::SetCursorPosX(xpos);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        if (ImGui::ImageButton("ZoomIn", editor->GetEditorLayer()->GetIcon("ZoomIn"), {size, size}))
        {
            camera.ZoomCamera(1.f);
        }
        ImGui::PopStyleColor();

        ImGui::SetCursorPosX(xpos + style.ItemSpacing.x / 2.f);
        ImGui::VSliderFloat("##Camera Zoom", { size, size * 10.f }, &camera.ZoomLevel(), Camera::mMinZoom, Camera::mMaxZoom, "##%.2f");
        
        ImGui::SetCursorPosX(xpos);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        if (ImGui::ImageButton("ZoomOut", editor->GetEditorLayer()->GetIcon("ZoomOut"), {size, size}))
        {
            camera.ZoomCamera(-1.f);
        }
        ImGui::PopStyleColor();

        ImGui::End(); // end window camera zoom
        ImGui::PopStyleVar();
    }

    // Performance Viewer Panel
    void PerformancePanel::RenderPanel()
    {
        ImGuiIO& io = ImGui::GetIO();
        auto FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];

        // Text Colors
        const ImVec4 RED_COLOR = { 1.f, 0.3f, 0.2f, 1.f };
        const ImVec4 YELLOW_COLOR = { 1.f, .98f, 0.5f, 1.f };
        const ImVec4 WHITE_COLOR = { 1.f, 1.f, 1.f, 1.f };

        ImGui::Begin("Performance");
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
        
        if (ImGui::BeginTable("Engine", 2))
        {
            InsightEngine& engine = InsightEngine::Instance();
            const ImVec4 text_color = io.Framerate < 15.f ? RED_COLOR : io.Framerate < 30.f ? YELLOW_COLOR : WHITE_COLOR;

            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("V-Sync");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            static bool vsync_enabled = engine.IsVSync();
            if (ImGui::Checkbox("##V-Sync", &vsync_enabled))
                engine.EnableVSync(vsync_enabled);

            // Target FPS
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::SetNextItemWidth(20);
            ImGui::TextUnformatted("Target FPS:");
            ImGui::PopFont();
            ImGui::TableNextColumn();

            static int selected_fps_index = -1;
            const char* fps_options[] = { "30", "60", "120", "240", "9999"};
            int fps_values[] = { 30, 60, 120, 240, 9999 };

            for (int i{}; i < IM_ARRAYSIZE(fps_values); ++i)
            {
                if (fps_values[i] == engine.GetTargetFPS())
                {
                    selected_fps_index = i;
                    break;
                }
            }
            if (selected_fps_index == -1)
                selected_fps_index = 1; // Default to 60 FPS

            if (ImGui::Combo("FPS", &selected_fps_index, fps_options, IM_ARRAYSIZE(fps_options)))
            {
                int selected_fps = fps_values[selected_fps_index];
                engine.SetFPS(selected_fps);
            }

            // Current FPS
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Current FPS:");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::TextColored(text_color, "%.0f FPS", io.Framerate);

            // Delta Time
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Time:");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::TextColored(text_color, "%.3f ms", 1000.f / io.Framerate);

            ImGui::EndTable(); // end table Engine
        }
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::BeginChild("Systems"))
        {
            // Create a table for system usage
            ImGuiTableFlags flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_BordersInnerV;
            if (ImGui::BeginTable("System Usage", 3, flags))
            {
                // Table headers
                ImGui::PushFont(FONT_BOLD);
                ImGui::TableSetupColumn("System");
                ImGui::TableSetupColumn("Time");
                ImGui::TableSetupColumn("Usage");
                ImGui::TableHeadersRow();
                ImGui::PopFont();

                // Table values
                for (InsightEngine& engine = InsightEngine::Instance();
                     auto const& [system, dt] : engine.GetSystemDeltas())
                {
                    // Skip engine delta time
                    if (system == "Engine")
                        continue;

                    // Compute usage percent
                    double percent = (dt / engine.GetSystemDeltas().at("Engine")) * 100.0;
                    ImVec4 text_color = dt < 1.f / 60.f ? WHITE_COLOR : dt < 1.f / 30.f ? YELLOW_COLOR : RED_COLOR;

                    // Display system usage
                    ImGui::TableNextColumn();
                    ImGui::Spacing();
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::TextColored(text_color, "%s", system.c_str());
                    ImGui::PopFont();

                    ImGui::TableNextColumn();
                    ImGui::Spacing();
                    ImGui::TextColored(text_color, "%.3f ms", dt * 1000.f);

                    ImGui::TableNextColumn();
                    ImGui::Spacing();
                    ImGui::TextColored(text_color, "%.2lf%%", percent);
                    ImGui::Spacing();
                }

                ImGui::EndTable(); // end table System Usage
            }

            ImGui::EndChild(); // end child window Systems
        }

        ImGui::PopStyleVar();
        ImGui::End(); // end window Performance
    }

    // Log Console Panel
    void LogConsolePanel::RenderPanel() { Logger::LoggerGUI::Instance().Draw("Log Console"); }

    // Physics Control Panel
    void PhysicsControlPanel::RenderPanel()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFont* FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];

        ImGui::Begin("Physics");

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
        ImGuiTableFlags table_flags = 0;
        if (ImGui::BeginTable("Grid", 2, table_flags))
        {
            ImGui::PushFont(FONT_BOLD);
            ImGui::TableSetupColumn("Grid");
            ImGui::TableHeadersRow();

            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Show Grid");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::Checkbox("##ShowGrid", &Physics::mShowGrid);

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Columns");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::SliderInt("##GridColumns", &ImplicitGrid::mCols, ImplicitGrid::MIN_GRID_COLS, ImplicitGrid::MAX_GRID_COLS);

            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Rows");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::SliderInt("##GridRows", &ImplicitGrid::mRows, ImplicitGrid::MIN_GRID_ROWS, ImplicitGrid::MAX_GRID_ROWS);

            ImGui::EndTable(); // end table Grid
        }

        if (ImGui::BeginTable("Other Options", 2, table_flags))
        {
            ImGui::PushFont(FONT_BOLD);
            ImGui::TableSetupColumn("Other Options");
            ImGui::TableHeadersRow();

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Exert Gravity");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::Checkbox("##ExertGravity", &Physics::mExertingGravity);

            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Show Colliders");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::Checkbox("##ShowColliders", &Physics::mShowColliders);

            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Show Velocity");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::Checkbox("##ShowVelocity", &Physics::mShowVelocity);

            ImGui::EndTable(); // end table Other Options
        }

        ImGui::PopStyleVar();
        ImGui::End(); // end window Physics
    }

} // end namespace IS
