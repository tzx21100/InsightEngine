#include "Pch.h"
#include "EditorLayer.h"
#include "Graphics.h"
#include "CoreEngine.h"

namespace IS {

    EditorLayer::EditorLayer() : Layer("Editor Layer") {}

    void EditorLayer::onAttach() {
        // Attach scene viewer, import icons, open project...
        IS_CORE_DEBUG("{} attached", getName());
    }

    void EditorLayer::onDetach() {
        IS_CORE_DEBUG("{} detached", getName());
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

            if (ImGui::BeginMenu("Window")) {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools")) {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Build")) {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Select")) {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Actor")) {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help")) {

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Other windows
        RenderInspector();
        RenderPerformanceViewer();
        RenderLogConsole();
        RenderSceneViewer();

        ImGui::End();
    }

    void EditorLayer::RenderInspector() {
        ImGui::Begin("Inspector");

        for (auto& sprite : ISGraphics::sprites) {

            ImGui::Text("%-6s", sprite.name.c_str());
            ImGui::SameLine();
            ImGui::Checkbox(("##Draw " + sprite.name).c_str(), &sprite.drawing);
            ImGui::Spacing();

            if (sprite.name == "Circle") {
                ImGui::Text("%-6s", "Radius");
                ImGui::SameLine();
                ImGui::SliderFloat(("##Radius " + sprite.name).c_str(), &sprite.model_TRS.scaling.x, 2.f, WIDTH);
                ImGui::Spacing();
            } else {
                ImGui::Text("%-6s", "Width");
                ImGui::SameLine();
                ImGui::SliderFloat(("##Width" + sprite.name).c_str(), &sprite.model_TRS.scaling.x, 2.f, WIDTH);
                ImGui::Spacing();
                ImGui::Text("%-6s", "Height");
                ImGui::SameLine();
                ImGui::SliderFloat(("##Height" + sprite.name).c_str(), &sprite.model_TRS.scaling.y, 2.f, HEIGHT);
                ImGui::Spacing();
            }
            ImGui::Text("%-6s", "Angle");
            ImGui::SameLine();
            ImGui::SliderFloat(("##Angle" + sprite.name).c_str(), &sprite.model_TRS.orientation.x, 0.f, 360.f);
            ImGui::Spacing();
            ImGui::Text("%-6s", "Speed");
            ImGui::SameLine();
            ImGui::SliderFloat(("##Speed" + sprite.name).c_str(), &sprite.model_TRS.orientation.y, -180.f, 180.f);

            // Box uses texture so no point changing color
            if (sprite.name != "Box") {
                ImGui::Spacing();
                ImGui::Text("%-6s", "Color");
                ImGui::SameLine();
                ImGui::ColorEdit3(("##Color" + sprite.name).c_str(), &sprite.color[0]);
            }
            ImGui::Dummy({ 5.f, 5.f });
            ImGui::Separator();
            ImGui::Dummy({ 5.f, 5.f });
        }

        ImGui::End();
    }

    void EditorLayer::RenderPerformanceViewer() {
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
        if (ImGui::BeginTable("Systems", 2, ImGuiTableFlags_Resizable)) {
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
                    ImGui::Separator();

                    ImGui::TableNextColumn();
                    ImGui::Spacing();
                    ImGui::Text("%6d%%", static_cast<int>(percent));
                    ImGui::Separator();
                }
            }
            // End rendering table
            ImGui::EndTable();
        }

        // End rendering window
        ImGui::End();
    }

    // From imgui demo (for reference)
    void EditorLayer::RenderLogConsole() {
        static ExampleAppLog log;

        // For the demo: add a debug button _BEFORE_ the normal log window contents
        // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
        // Most of the contents of the window will be added by the log.Draw() call.
        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        ImGui::Begin("Log");
        if (ImGui::SmallButton("[Debug] Add 5 entries")) {
            static int counter = 0;
            const char* categories[6] = { "trace", "info", "debug", "warn", "error", "critical" };
            const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
            for (int n = 0; n < 5; n++) {
                const char* category = categories[counter % IM_ARRAYSIZE(categories)];
                const char* word = words[counter % IM_ARRAYSIZE(words)];
                log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                           ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
                counter++;
            }
        }
        ImGui::End();

        // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
        log.Draw("Log");
    }

    void EditorLayer::RenderSceneViewer() {
        ImGui::Begin("SceneView");

        const uint32_t window_width  = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
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

} // end namespace IS