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
#include "EditorLayer.h"

// Dependencies
#include <imgui.h>

namespace IS {

    // Scene Panel
    void ScenePanel::RenderPanel() {
        auto& engine = InsightEngine::Instance();
        auto input = engine.GetSystem<InputManager>("Input");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
        ImGui::Begin("Scene", nullptr, window_flags);

        // Allow key/mouse event pass through only in this panel
        if (ImGui::IsWindowFocused()) {
            ImGuiIO& io = ImGui::GetIO();
            io.WantCaptureMouse = io.WantCaptureKeyboard = false;
        }

        ImVec2 scene_size = ImGui::GetWindowSize();
        ImVec2 scene_pos = ImGui::GetWindowPos();
        // Scene pos for the input
        ImVec2 actual_scene_pos;
        actual_scene_pos.x = scene_pos.x - EditorLayer::GetDockspacePosition().x;
        actual_scene_pos.y = scene_pos.y - EditorLayer::GetDockspacePosition().y;
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

    // Performance Viewer Panel
    void PerformancePanel::RenderPanel() {
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

    // Log Console Panel
    void LogConsolePanel::RenderPanel() {
        Logger::GetLoggerGUI().Draw("Log Console");
    }

} // end namespace IS
