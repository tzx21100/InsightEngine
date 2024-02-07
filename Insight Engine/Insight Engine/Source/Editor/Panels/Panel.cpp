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
#ifdef USING_IMGUI
#include "Panel.h"
#include "Editor/Utils/EditorUtils.h"
#include "Editor/Layers/EditorLayer.h"
#include "Editor/Utils/FileUtils.h"
#include "Physics/System/Physics.h"
#include "Physics/Collision/Grid.h"
#include "Graphics/Core/Graphics.h"
#include "Graphics/System/Camera3D.h"

// Dependencies
#include <imgui.h>
#include <imgui_internal.h>

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

        if (mHovered)
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);
            ImVec2 cusor_pos = ImGui::GetMousePos();
            ImGui::GetWindowDrawList()->AddImage(mEditorLayer.GetIcon("Cursor"), cusor_pos, ImVec2(cusor_pos.x + 32, cusor_pos.y + 32));
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

        // Text Colors
        const ImU32 GREEN_COLOR = IM_COL32(45, 201, 55, 255);
        const ImU32 YELLOW_COLOR = IM_COL32(231, 180, 22, 255);
        const ImU32 RED_COLOR = IM_COL32(204, 50, 50, 255);

        ImGui::Begin((ICON_LC_GAUGE "  " + mName).c_str());

        // Window contents
        {
            auto& engine = InsightEngine::Instance();

            ImGui::SeparatorText("General");

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);

            ImGui::TextUnformatted("V-Sync");
            ImGui::SameLine();

            static bool vsync_enabled = engine.IsVSync();
            EditorUtils::RenderToggleButton("V-Sync", vsync_enabled);
            engine.EnableVSync(vsync_enabled);

            ImGui::SameLine();
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

            ImGui::SameLine();
            ImGui::TextUnformatted("Target FPS:");

            ImGui::SameLine();
            static int selected_fps_index = -1;
            const char* fps_options[] = { "15", "30", "60", "90", "120", "144", "240", "No Limit" };
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

            ImGui::SameLine();
            const float LONGEST_ITEM_WIDTH = ImGui::CalcTextSize("No Limit").x * 2;
            ImGui::SetNextItemWidth(LONGEST_ITEM_WIDTH);
            if (ImGui::Combo("##FPS", &selected_fps_index, fps_options, IM_ARRAYSIZE(fps_options)))
            {
                int selected_fps = fps_values[selected_fps_index];
                engine.SetFPS(selected_fps);
            }

            ImGui::SameLine();
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
            ImGui::SameLine();
            ImGui::TextUnformatted("Delta Time: ");
            ImGui::SameLine();
            ImU32 text_color = io.Framerate < 15.f ? RED_COLOR : io.Framerate < 30.f ? YELLOW_COLOR : IM_COL32_WHITE;
            ImGui::TextColored(ImColor(text_color), "%.3lf ms", io.DeltaTime * 1000.0);

            ImGui::SameLine();
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
            ImGui::SameLine();
            ImGui::TextUnformatted("Current FPS: ");
            ImGui::SameLine();
            ImGui::TextColored(ImColor(text_color), "%.3f", io.Framerate);

            ImGui::SeparatorText("FPS Tracker");
            ImGui::SetItemTooltip("Tracks FPS for the last 100 frames");

            static std::vector<float> fps_data;
            static constexpr int MAX_DATA_SIZE = 100;
            static float UPDATE_FREQUENCY = .05f;
            static float elapsed_time = 0.f;
            elapsed_time += static_cast<float>(engine.mDeltaTime);
            if (elapsed_time == 0.f || elapsed_time >= UPDATE_FREQUENCY)
            {
                fps_data.push_back(static_cast<float>(io.Framerate));
                elapsed_time = 0.1f;
                UPDATE_FREQUENCY = .15f;
            }
            if (fps_data.size() > MAX_DATA_SIZE)
            {
                fps_data.erase(fps_data.begin());
            }

            ImVec2 histogram_size = { ImGui::GetContentRegionAvail().x, 100.f };
            ImGui::PlotLines("##Current FPS", fps_data.data(), static_cast<int>(fps_data.size()), 0, nullptr, 0.f, FLT_MAX, histogram_size);

            ImGui::Spacing();
                        
            ImGui::SeparatorText("System Usage");

            // Create a table for system usage
            ImGuiTableFlags flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable;
            EditorUtils::RenderTableFixedWidth("System Usage", 3, [&]()
            {
                // Table values
                for (auto const& [system, dt] : engine.GetSystemDeltas())
                {
                    if (system == "Engine")
                        continue;

                    // Compute usage percent
                    float percent = dt / (engine.GetSystemDeltas().at("Engine"));
                    text_color = dt < 1.f / 60.f ? IM_COL32_WHITE : dt < 1.f / 30.f ? YELLOW_COLOR : RED_COLOR;
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
            }, flags);

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
        auto& logger_gui = LoggerGUI::Instance();
        logger_gui.Draw((ICON_LC_TERMINAL_SQUARE "  " + mName).c_str());
        mSize = logger_gui.mPanelSize;
    }

    // Physics Control Panel
    void SettingsPanel::RenderPanel()
    {
        ImGui::Begin((ICON_LC_SETTINGS "  " + mName).c_str());
        
        // Window contents
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);

            ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;

            if (ImGui::TreeNodeEx(ICON_LC_PERSON_STANDING "  Physics", tree_flags))
            {
                EditorUtils::RenderControlVec2("Gravity", Physics::mGravity);
                EditorUtils::RenderTableFixedWidth("PhysicsTable", 2, [&]()
                {
                    EditorUtils::RenderTableLabel("Exert Gravity");
                    ImGui::TableNextColumn();
                    EditorUtils::RenderToggleButton("ExertGravity", Physics::mExertingGravity);

                    EditorUtils::RenderTableLabel("Show Velocity");
                    ImGui::TableNextColumn();
                    EditorUtils::RenderToggleButton("ShowVelocity", Physics::mShowVelocity);
                });

                ImGui::TreePop(); // end tree Table
            }

            if (ImGui::TreeNodeEx(ICON_LC_FLIP_HORIZONTAL_2 "  Collision", tree_flags))
            {
                EditorUtils::RenderTableFixedWidth("CollisionTable", 2, [&]()
                {
                    EditorUtils::RenderTableLabel("Show Colliders");
                    ImGui::TableNextColumn();
                    EditorUtils::RenderToggleButton("ShowColliders", Physics::mShowColliders);

                    EditorUtils::RenderTableLabel("Implicit Grid");
                    ImGui::TableNextColumn();
                    EditorUtils::RenderToggleButton("ImplicitGrid", Physics::mEnableImplicitGrid);
                });

                if (Physics::mEnableImplicitGrid)
                {
                    ImGui::SeparatorText(ICON_LC_GRID_2X2 "  Implicit Grid");

                    EditorUtils::RenderTableFixedWidth("Implicit Grid Table", 2, [&]()
                    {
                        EditorUtils::RenderTableLabel("Show Grid");
                        ImGui::TableNextColumn();
                        EditorUtils::RenderToggleButton("ShowGrid", Physics::mShowGrid);

                        EditorUtils::RenderTableLabel("Columns");
                        ImGui::TableNextColumn();
                        if (ImGui::InputInt("##GridColumns", &ImplicitGrid::mCols))
                        {
                            ImplicitGrid::mCols = std::clamp(ImplicitGrid::mCols, ImplicitGrid::MIN_GRID_COLS, ImplicitGrid::MAX_GRID_COLS);
                        }

                        EditorUtils::RenderTableLabel("Rows");
                        ImGui::TableNextColumn();
                        if (ImGui::InputInt("##GridRows", &ImplicitGrid::mRows))
                            ImplicitGrid::mRows = std::clamp(ImplicitGrid::mRows, ImplicitGrid::MIN_GRID_ROWS, ImplicitGrid::MAX_GRID_ROWS);
                    });
                }

                ImGui::TreePop(); // end tree Collision
            }

            ImGui::PopStyleVar();
        }
        
        // Save window states
        mFocused    = ImGui::IsWindowFocused();
        mAppearing  = ImGui::IsWindowAppearing();
        mHovered    = ImGui::IsWindowHovered();
        ImGui::End(); // end window Settings
    }

} // end namespace IS

#endif // USING_IMGUI

