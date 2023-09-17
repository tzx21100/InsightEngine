#include "Pch.h"
#include "EditorLayer.h"
#include "Graphics.h"
#include "CoreEngine.h"

namespace IS {

    EditorLayer::EditorLayer() : Layer("Editor Layer"), dockspace_pos(), scene_panel_size(), scene_hierarchy_panel() {}

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
        ImGui::Begin("EditorDockSpace", nullptr, window_flags);
        //i add in pos here to get the position of the dockspace
        dockspace_pos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };

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

            if (ImGui::BeginMenu("Scene")) {
                InsightEngine& engine = InsightEngine::Instance();
                if (ImGui::MenuItem("Create 500 entities")) {
                    for (int i{}; i < 500; i++) {
                        PRNG prng;
                        Entity a = engine.CreateEntityWithComponents<Sprite, Transform>();
                        auto& trans = engine.GetComponent<Transform>(a);
                        trans.setScaling((prng.generate() * 18.f) + 2.f, (prng.generate() * 18.f) + 2.f); // scale [2, 20]
                        trans.setWorldPosition((prng.generate() * WIDTH) - WIDTH / 2.f, (prng.generate() * HEIGHT) - HEIGHT / 2.f); // xpos [-width/2, width/2], ypos [-height/2, height/2]
                        trans.setOrientation((prng.generate() * 360.f), (prng.generate() * 360.f) - 180.f); // angle [0, 360], speed [-180, 180]
                    }
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Other windows
        RenderScenePanel();
        RenderSceneHierarchyPanel();
        RenderPerformancePanel();
        RenderLogConsolePanel();
        RenderSceneOverlay();

        ImGui::End();
    }

    void EditorLayer::RenderScenePanel() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar;
        ImGui::Begin("Scene", nullptr, flags);

        ImVec2 scene_size = ImGui::GetWindowSize();
        ImVec2 scene_pos = ImGui::GetWindowPos();
        //scene pos for the input
        ImVec2 actual_scene_pos;
        actual_scene_pos.x = scene_pos.x-dockspace_pos.x;
        actual_scene_pos.y = scene_pos.y-dockspace_pos.y;
        InputManager::Instance().setCenterPos(actual_scene_pos.x + scene_size.x / 2, actual_scene_pos.y + scene_size.y / 2);
        InputManager::Instance().setRatio(scene_size.x, scene_size.y);

        // Resize framebuffer
        ImVec2 panel_size = ImGui::GetContentRegionAvail();
        if (!(scene_panel_size.x == panel_size.x && scene_panel_size.y == panel_size.y)) {
            ISGraphics::resizeFramebuffer(static_cast<uint32_t>(panel_size.x), static_cast<uint32_t>(panel_size.y));
            scene_panel_size = { panel_size.x, panel_size.y };
        }

        ImGui::Image(std::bit_cast<ImTextureID>(static_cast<uintptr_t>(ISGraphics::getScreenTexture())),
                     ImVec2(scene_panel_size.x, scene_panel_size.y), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorLayer::RenderSceneHierarchyPanel() {
        scene_hierarchy_panel.RenderPanel();
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
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize |
                                        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        InsightEngine& engine = InsightEngine::Instance();
        InputManager& input = InputManager::Instance();

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Overlay", (bool*)1, window_flags)) {
            ImGui::Text("Entities Alive: %d", engine.EntitiesAlive());
            ImGui::Text("Max Entities: %d", MAX_ENTITIES);
            ImGui::Separator();
            ImGui::Text("Cursor Position: (%.2f, %.2f)", input.GetMousePosition().first, input.GetMousePosition().second);
        }
        ImGui::End();
    }

} // end namespace IS
