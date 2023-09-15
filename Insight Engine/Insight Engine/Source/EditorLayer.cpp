#include "Pch.h"
#include "EditorLayer.h"
#include "Graphics.h"
#include "CoreEngine.h"

namespace IS {

    EditorLayer::EditorLayer() : Layer("Editor Layer") {}

    void EditorLayer::onAttach() {
        // Attach scene viewer, import icons, open project...
        //IS_CORE_DEBUG("{} attached", getName());
    }

    void EditorLayer::onDetach() {
        //IS_CORE_DEBUG("{} detached", getName());
    }

    void EditorLayer::onUpdate([[maybe_unused]] float dt) {
        // some updating
    }

    void EditorLayer::onRender() {

        static bool show_dockspace = true;
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
        ImGui::Begin("EditorDockSpace", &show_dockspace, window_flags);
        ImGui::PopStyleVar();

        ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("Editor");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        // Create Menu Bar
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::Separator();
                ImGui::MenuItem("New Level");
                ImGui::MenuItem("Open Level");
                ImGui::Separator();
                ImGui::MenuItem("Save Current Level");
                ImGui::MenuItem("Save Current Level As...");
                ImGui::MenuItem("Save All");
                ImGui::Separator();
                ImGui::MenuItem("Import into Level...");
                ImGui::MenuItem("Export All...");
                ImGui::MenuItem("Export Selected...");
                ImGui::Separator();
                ImGui::MenuItem("New Project...");
                ImGui::MenuItem("Open Project...");
                ImGui::MenuItem("Recent Projects");
                ImGui::Separator();
                if (ImGui::MenuItem("Exit"))
                    InsightEngine::Instance().Exit();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Other windows
        RenderScenePanel();
        RenderInspectorPanel();
        RenderPerformancePanel();
        RenderLogConsolePanel();

        ImGui::End();
    }

    void EditorLayer::RenderScenePanel() {
        ImGui::Begin("Scene");

        const uint32_t window_width = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
        const uint32_t window_height = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);
        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::GetWindowDrawList()->AddImage(
            (void*)(static_cast<uintptr_t>(ISGraphics::tex_id)),
            ImVec2(pos.x, pos.y),
            ImVec2(pos.x + window_width, pos.y + window_height),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImGui::End();
    }

    void EditorLayer::RenderInspectorPanel() {
        ImGui::Begin("Inspector");

        for (auto& model : ISGraphics::models) {

            ImGui::Text("%-6s", model.name.c_str());
            ImGui::SameLine();
            ImGui::Checkbox(("##Draw " + model.name).c_str(), &model.drawing);
            ImGui::Spacing();

            if (model.name == "Circle") {
                ImGui::Text("%-6s", "Radius");
                ImGui::SameLine();
                ImGui::SliderFloat(("##Radius " + model.name).c_str(), &model.model_TRS.scaling.x, 2.f, WIDTH);
                ImGui::Spacing();
            } else {
                ImGui::Text("%-6s", "Width");
                ImGui::SameLine();
                ImGui::SliderFloat(("##Width" + model.name).c_str(), &model.model_TRS.scaling.x, 2.f, WIDTH);
                ImGui::Spacing();
                ImGui::Text("%-6s", "Height");
                ImGui::SameLine();
                ImGui::SliderFloat(("##Height" + model.name).c_str(), &model.model_TRS.scaling.y, 2.f, HEIGHT);
                ImGui::Spacing();
            }
            ImGui::Text("%-6s", "Angle");
            ImGui::SameLine();
            ImGui::SliderFloat(("##Angle" + model.name).c_str(), &model.model_TRS.orientation.x, 0.f, 360.f);
            ImGui::Spacing();
            ImGui::Text("%-6s", "Speed");
            ImGui::SameLine();
            ImGui::SliderFloat(("##Speed" + model.name).c_str(), &model.model_TRS.orientation.y, -180.f, 180.f);

            // Box uses texture so no point changing color
            if (model.name != "Box") {
                ImGui::Spacing();
                ImGui::Text("%-6s", "Color");
                ImGui::SameLine();
                ImGui::ColorEdit3(("##Color" + model.name).c_str(), &model.color[0]);
            }
            ImGui::Dummy({ 5.f, 5.f });
            ImGui::Separator();
            ImGui::Dummy({ 5.f, 5.f });
        }

        ImGui::End();
    }

    void EditorLayer::RenderPerformancePanel() {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Begin("Performance");
        if (ImGui::BeginTable("Engine", 2)) {
            ImGui::TableNextColumn();
            ImGui::Text("Framerate:");
            ImGui::TableNextColumn();
            ImGui::Text("%5.0f FPS", io.Framerate);

            ImGui::TableNextColumn();
            ImGui::Text(" Timestep:");
            ImGui::TableNextColumn();
            ImGui::Text("%.2f ms", 1000.f / io.Framerate);

            ImGui::EndTable();
        }
        ImGui::Dummy({ 5.f, 5.f });
        ImGui::Separator();
        ImGui::Dummy({ 5.f, 5.f });
        
        // Create a table for system usage
        ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_BordersH;
        if (ImGui::BeginTable("Systems", 2, flags)) {
            // Table headers
            ImGui::TableSetupColumn("System");
            ImGui::TableSetupColumn("Usage %");
            ImGui::TableHeadersRow();

            // Table values
            for (InsightEngine& engine = InsightEngine::Instance();
                auto const& [system, dt] : engine.GetSystemDeltas()) {
                double percent = std::round((dt / engine.GetSystemDeltas().at("Engine")) * 100.0);
                if (system != "Engine") {
                    ImGui::TableNextColumn();
                    ImGui::Spacing();
                    ImGui::Text("%s", system.c_str());

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

    void EditorLayer::RenderLogConsolePanel() {
        Logger::getLoggerGUI().draw("Log Console");
    }

} // end namespace IS