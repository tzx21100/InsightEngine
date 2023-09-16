#include "Pch.h"
#include "EditorLayer.h"
#include "Graphics.h"
#include "CoreEngine.h"

namespace IS {

    //i need the dockspace pos for input
    ImVec2 dockspace_pos;

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
        //i add in pos here to get the position of the dockspace
        dockspace_pos = ImGui::GetWindowPos();

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

            if (ImGui::BeginMenu("Entity")) {
                InsightEngine& engine = InsightEngine::Instance();
                if (ImGui::MenuItem("Create 500 entities")) {
                    InputManager& input = InputManager::Instance();
                    for (int i = 0; i < 500; i++) {
                        Entity a = engine.CreateEntityWithComponents<Sprite, Transform>();
                        auto& trans = engine.GetComponent<Transform>(a);
                        trans.setScaling(120 - i, 120 - i);
                        trans.setWorldPosition(input.GetMousePosition().first / 2, input.GetMousePosition().second / 2);
                    }
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Other windows
        RenderScenePanel();
        RenderInspectorPanel();
        RenderPerformancePanel();
        RenderLogConsolePanel();
        RenderSceneOverlay();

        ImGui::End();
    }

    void EditorLayer::RenderScenePanel() {


        ImGui::Begin("Scene");
        ImVec2 scene_size = ImGui::GetWindowSize();
        ImVec2 scene_pos = ImGui::GetWindowPos();
        //scene pos for the input
        ImVec2 actual_scene_pos;
        actual_scene_pos.x = scene_pos.x-dockspace_pos.x;
        actual_scene_pos.y = scene_pos.y-dockspace_pos.y;
        InputManager::Instance().setCenterPos(actual_scene_pos.x + scene_size.x / 2, actual_scene_pos.y + scene_size.y / 2);
        InputManager::Instance().setRatio(scene_size.x, scene_size.y);


        const uint32_t window_width = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
        const uint32_t window_height = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);
        ImVec2 pos = ImGui::GetCursorScreenPos();


        ImGui::GetWindowDrawList()->AddImage(
            std::bit_cast<void*>(static_cast<uintptr_t>(ISGraphics::getScreenTexture())),
            ImVec2(pos.x, pos.y),
            ImVec2(pos.x + window_width, pos.y + window_height),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImGui::End();
    }

    void EditorLayer::RenderInspectorPanel() {
        ImGui::Begin("Inspector");

        /*for (auto& sprite : ISGraphics::sprites) {

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


            ImGui::Spacing();
            ImGui::Text("%-6s", "Color");
            ImGui::SameLine();
            ImGui::ColorEdit3(("##Color" + sprite.name).c_str(), &sprite.color[0]);

            ImGui::Dummy({ 5.f, 5.f });
            ImGui::Separator();
            ImGui::Dummy({ 5.f, 5.f });
        }*/

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

    void EditorLayer::RenderSceneOverlay() {
        static bool show = true;
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize |
                                        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
        InsightEngine& engine = InsightEngine::Instance();

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Overlay", &show, window_flags)) {
            ImGui::Text("Entities Alive: %d", engine.EntitiesAlive());
            ImGui::Text("Max Entities: %d", MAX_ENTITIES);
        }
        ImGui::End();
    }

} // end namespace IS