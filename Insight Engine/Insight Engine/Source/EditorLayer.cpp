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
        static bool opt_fullscreen = false;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        } else {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

        ImGui::SetNextWindowBgAlpha(0.f);
        ImGui::Begin("EditorDockSpace", &show_dockspace, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
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
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
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

        ImGui::End();
    }

    void EditorLayer::RenderInspector() {
        ImGui::Begin("Inspector");

        for (auto& model : ISGraphics::models) {

            ImGui::Text("%-6s", model.name.c_str());
            ImGui::SameLine();
            ImGui::Checkbox(("##Draw " + model.name).c_str(), &model.drawing);
            ImGui::Spacing();
            ImGui::Text("Width ");
            ImGui::SameLine();
            ImGui::SliderFloat(("##Width" + model.name).c_str(), &model.scaling.x, 2.f, 2.f * WIDTH);
            ImGui::Spacing();
            ImGui::Text("Height");
            ImGui::SameLine();
            ImGui::SliderFloat(("##Height" + model.name).c_str(), &model.scaling.y, 1.f, 2.f * HEIGHT);
            ImGui::Spacing();
            ImGui::Text("Angle ");
            ImGui::SameLine();
            ImGui::SliderFloat(("##Angle" + model.name).c_str(), &model.orientation.x, 0.f, 360.f);
            ImGui::Spacing();
            ImGui::Text("Color ");
            ImGui::SameLine();
            ImGui::ColorEdit3(("##Color" + model.name).c_str(), &model.color[0]);
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

} // end namespace IS