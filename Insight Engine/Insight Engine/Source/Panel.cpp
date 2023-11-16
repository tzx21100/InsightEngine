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
#include "Physics.h"
#include "Grid.h"

// Dependencies
#include <imgui.h>

namespace IS {

    void GamePanel::UpdatePanel()
    {
        if (mAppearing)
        {
            Camera3D::mActiveCamera = CAMERA_TYPE_GAME;
        }
    }

    void GamePanel::RenderPanel()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

        ImGuiWindowFlags window_flags = 0;
        ImGui::Begin((ICON_LC_GAMEPAD_2 "  " + mName).c_str(), nullptr, window_flags);
        
        // Window contents
        {
            if (mFocused)
            {
                ImGuiIO& io = ImGui::GetIO();
                io.WantCaptureMouse = io.WantCaptureKeyboard = false;
            }

            ImVec2 panel_size = ImGui::GetContentRegionAvail();
            mSize = { panel_size.x, panel_size.y };

            // Display Game scene
            ImGui::Image(EditorUtils::ConvertTextureID(ISGraphics::GetScreenTexture()), panel_size, { 0, 1 }, { 1, 0 });
        }

        // Save window states
        mFocused = ImGui::IsWindowFocused();
        mAppearing = ImGui::IsWindowAppearing();
        mHovered = ImGui::IsItemHovered();
        ImGui::End(); // end window Game
        ImGui::PopStyleVar();
    }

    // Profiler Panel
    void ProfilerPanel::RenderPanel()
    {
        ImGuiIO& io = ImGui::GetIO();
        auto FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];

        // Text Colors
        const ImU32 GREEN_COLOR = IM_COL32(45, 201, 55, 255);
        const ImU32 YELLOW_COLOR = IM_COL32(231, 180, 22, 255);
        const ImU32 RED_COLOR = IM_COL32(204, 50, 50, 255);

        ImGui::Begin((ICON_LC_GAUGE "  " + mName).c_str());

        // Window contents
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);

            if (ImGui::BeginTable("Engine", 2))
            {
                InsightEngine& engine = InsightEngine::Instance();
                const ImU32 text_color = io.Framerate < 15.f ? RED_COLOR : io.Framerate < 30.f ? YELLOW_COLOR : IM_COL32_WHITE;

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
                const char* fps_options[] = { "15", "30", "60", "90", "120", "144", "240", "No Limit"};
                int fps_values[] = { 15, 30, 60, 90, 120, 144, 240, 9999 };

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
                ImGui::TextColored(ImColor(text_color), "%.0f FPS", io.Framerate);

                // Delta Time
                ImGui::TableNextColumn();
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("Time:");
                ImGui::PopFont();
                ImGui::TableNextColumn();
                ImGui::TextColored(ImColor(text_color), "%.3lf ms", io.DeltaTime * 1000.0);

                ImGui::EndTable(); // end table Engine
            }
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::BeginChild("Systems");
            
            // Child window contents
            {
                // Create a table for system usage
                ImGuiTableFlags flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable;
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
                        if (system == "Engine")
                            continue;

                        // Compute usage percent
                        float percent = dt / (engine.GetSystemDeltas().at("Engine"));
                        ImU32 text_color = dt < 1.f / 60.f ? IM_COL32_WHITE : dt < 1.f / 30.f ? YELLOW_COLOR : RED_COLOR;
                        ImU32 bar_color = dt < 1.f / 60.f ? GREEN_COLOR : dt < 1.f / 30.f ? YELLOW_COLOR : RED_COLOR;

                        // Display system usage
                        ImGui::TableNextColumn();
                        ImGui::Spacing();
                        ImGui::TextColored(ImColor(text_color), "%s", system.c_str());

                        ImGui::TableNextColumn();
                        ImGui::Spacing();
                        ImGui::TextColored(ImColor(text_color), "%.3lf ms", dt * 1'000.0);

                        ImGui::TableNextColumn();
                        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, bar_color);
                        ImGui::ProgressBar(percent);
                        ImGui::PopStyleColor();
                    }

                    ImGui::EndTable(); // end table System Usage
                }
            }

            ImGui::EndChild(); // end child window Systems

            ImGui::PopStyleVar();
        }

        // Save window states
        mFocused = ImGui::IsWindowFocused();
        mAppearing = ImGui::IsWindowAppearing();
        mHovered = ImGui::IsItemHovered();
        ImGui::End(); // end window Performance
    }

    // Log Console Panel
    void ConsolePanel::RenderPanel()
    {
        auto& logger_gui = Logger::LoggerGUI::Instance();
        logger_gui.Draw((ICON_LC_TERMINAL_SQUARE "  " + mName).c_str());
        mSize = logger_gui.mPanelSize;
    }

    // Physics Control Panel
    void SettingsPanel::RenderPanel()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFont* FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];

        ImGui::Begin((ICON_LC_SETTINGS "  " + mName).c_str());
        
        // Window contents
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);

            ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;

            const float COLUMN_WIDTH = 100.f;

            if (ImGui::TreeNodeEx(ICON_LC_PERSON_STANDING "  Physics", tree_flags))
            {
                EditorUtils::RenderControlVec2("Gravity", Physics::mGravity);
                if (ImGui::BeginTable("PhysicsTable", 2))
                {
                    ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, COLUMN_WIDTH);

                    ImGui::TableNextColumn();
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::TextUnformatted("Exert Gravity");
                    ImGui::PopFont();
                    ImGui::TableNextColumn();
                    ImGui::Checkbox("##ExertGravity", &Physics::mExertingGravity);

                    ImGui::TableNextColumn();
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::TextUnformatted("Show Velocity");
                    ImGui::PopFont();
                    ImGui::TableNextColumn();
                    ImGui::Checkbox("##ShowVelocity", &Physics::mShowVelocity);

                    ImGui::EndTable(); // end table PhysicsTable
                }

                ImGui::TreePop(); // end tree Table
            }

            if (ImGui::TreeNodeEx(ICON_LC_FLIP_HORIZONTAL_2 "  Collision", tree_flags))
            {
                if (ImGui::BeginTable("CollisionTable", 2))
                {
                    ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, COLUMN_WIDTH);

                    ImGui::TableNextColumn();
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::TextUnformatted("Show Colliders");
                    ImGui::PopFont();
                    ImGui::TableNextColumn();
                    ImGui::Checkbox("##ShowColliders", &Physics::mShowColliders);

                    ImGui::TableNextColumn();
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::TextUnformatted("Implicit Grid");
                    ImGui::PopFont();
                    ImGui::TableNextColumn();
                    ImGui::Checkbox("##ImplicitGrid", &Physics::mEnableImplicitGrid);

                    ImGui::EndTable(); // end table CollisionTable
                }

                if (Physics::mEnableImplicitGrid)
                {
                    if (ImGui::TreeNodeEx(ICON_LC_GRID_2X2 "  Implicit Grid", tree_flags))
                    {
                        if (ImGui::BeginTable("Implicit Grid Table", 2))
                        {
                            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, COLUMN_WIDTH);

                            ImGui::TableNextColumn();
                            ImGui::PushFont(FONT_BOLD);
                            ImGui::TextUnformatted("Show Grid");
                            ImGui::PopFont();
                            ImGui::TableNextColumn();
                            ImGui::Checkbox("##ShowGrid", &Physics::mShowGrid);

                            ImGui::TableNextColumn();
                            ImGui::PushFont(FONT_BOLD);
                            ImGui::TextUnformatted("Columns");
                            ImGui::PopFont();
                            ImGui::TableNextColumn();
                            if (ImGui::InputInt("##GridColumns", &ImplicitGrid::mCols))
                                ImplicitGrid::mCols = std::clamp(ImplicitGrid::mCols, ImplicitGrid::MIN_GRID_COLS, ImplicitGrid::MAX_GRID_COLS);

                            ImGui::TableNextColumn();
                            ImGui::PushFont(FONT_BOLD);
                            ImGui::TextUnformatted("Rows");
                            ImGui::PopFont();
                            ImGui::TableNextColumn();
                            if (ImGui::InputInt("##GridRows", &ImplicitGrid::mRows))
                                ImplicitGrid::mRows = std::clamp(ImplicitGrid::mRows, ImplicitGrid::MIN_GRID_ROWS, ImplicitGrid::MAX_GRID_ROWS);

                            ImGui::EndTable(); // end table Implicit Grid Table
                        }

                        ImGui::TreePop(); // end tree Implicit Grid
                    }
                }

                ImGui::TreePop(); // end tree Collision
            }

            if (ImGui::TreeNodeEx(ICON_LC_IMAGE "  Graphics", tree_flags))
            {
                if (ImGui::BeginTable("GraphicsTable", 2))
                {
                    ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, COLUMN_WIDTH);

                    ImGui::TableNextColumn();
                    ImGui::PushFont(FONT_BOLD);
                    ImGui::TextUnformatted("Text Animation");
                    ImGui::PopFont();
                    ImGui::TableNextColumn();
                    ImGui::Checkbox("##TextAnimation", &ISGraphics::mShowTextAnimation);

                    ImGui::EndTable(); // end table Other Options
                }
                ImGui::TreePop(); // end tree Graphics
            }

            ImGui::PopStyleVar();
        }
        
        // Save window states
        mFocused    = ImGui::IsWindowFocused();
        mAppearing  = ImGui::IsWindowAppearing();
        mHovered    = ImGui::IsItemHovered();
        ImGui::End(); // end window Settings
    }

} // end namespace IS
