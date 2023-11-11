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

 // Dependencies
#include <imgui.h>

#pragma warning(push)
#pragma warning(disable: 4201) // nonstandard nameless struct
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#pragma warning(pop)

namespace IS {

    void GamePanel::RenderPanel()
    {
        auto& engine = InsightEngine::Instance();
        auto input = engine.GetSystem<InputManager>("Input");
        auto editor = engine.GetSystem<ImGuiLayer>("Editor");
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

        ImGuiWindowFlags window_flags = 0;
        ImGui::Begin(mName.c_str(), nullptr, window_flags);

        // Allow key/mouse event pass through only in this panel
        mFocused = ImGui::IsWindowFocused();
        if (mFocused)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.WantCaptureMouse = io.WantCaptureKeyboard = false;
        }

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
        auto editor = engine.GetSystem<ImGuiLayer>("Editor");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

        ImGuiWindowFlags window_flags = 0;
        ImGui::Begin(mName.c_str(), nullptr, window_flags);

        auto viewport_lower_bound = ImGui::GetWindowContentRegionMin();
        auto viewport_upper_bound = ImGui::GetWindowContentRegionMax();
        auto viewport_offset = ImGui::GetWindowPos();
        mViewportBounds[0] = { viewport_lower_bound.x + viewport_offset.x, viewport_lower_bound.y + viewport_offset.y };
        mViewportBounds[1] = { viewport_upper_bound.x + viewport_offset.x, viewport_upper_bound.y + viewport_offset.y };

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
        mViewportPos.x = scene_pos.x - mEditorLayer.GetDockspacePosition().x;
        mViewportPos.y = scene_pos.y - mEditorLayer.GetDockspacePosition().y;

        auto& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];

        input->setCenterPos(mViewportPos.x + (float)scene_size.x / 2.f, mViewportPos.y + (float)scene_size.y / 2.f);
        input->setRatio(scene_size.x * camera.GetZoomLevel(), scene_size.y * camera.GetZoomLevel());

        // Resize framebuffer
        ImVec2 panel_size = ImGui::GetContentRegionAvail();
        mViewportSize = { panel_size.x, panel_size.y };

        // Display actual scene
        ImGui::Image(EditorUtils::ConvertTextureID(ISGraphics::GetScreenTexture()), panel_size, { 0, 1 }, { 1, 0 });

        // Accept asset browser payload
        mEditorLayer.AcceptAssetBrowserPayload();

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMPORTED_TEXTURE"))
            {
                std::filesystem::path path = static_cast<wchar_t*>(payload->Data);
                Entity entity = SceneManager::Instance().AddEntity("Imported Entity").value();
                engine.AddComponent<Sprite>(entity, Sprite());
                engine.AddComponent<Transform>(entity, Transform());
                auto& sprite = engine.GetComponent<Sprite>(entity);
                auto& transform = engine.GetComponent<Transform>(entity);
                auto const asset = engine.GetSystem<AssetManager>("Asset");
                IS_CORE_DEBUG("Image : {} ", path.string());
                sprite.img = *asset->GetImage(path.string());

                transform.scaling = { static_cast<float>(sprite.img.width), static_cast<float>(sprite.img.height) };
                transform.world_position = { static_cast<float>(input->GetMousePosition().first),
                static_cast<float>(input->GetMousePosition().second) };
            }

            ImGui::EndDragDropTarget();
        }

        // Help tooltip
        if (Camera::mActiveCamera == CAMERA_TYPE_EDITOR)
        {
            RenderHelp();
        }

        // Render gizmo
        RenderGizmo();

        ImGui::End(); // end window Scene
        ImGui::PopStyleVar();
    }

    bool ScenePanel::IsFocused() const { return mFocused; }

    bool ScenePanel::MouseWithinViewport() const
    {
        auto [mx, my] = ImGui::GetMousePos();
        mx -= mViewportBounds[0].x;
        my -= mViewportBounds[0].y;
        my = mViewportSize.y - my;
        int mouse_x = static_cast<int>(mx);
        int mouse_y = static_cast<int>(my);

        return (0 <= mouse_x && mouse_x < static_cast<int>(mViewportSize.x) &&
                0 <= mouse_y && mouse_y < static_cast<int>(mViewportSize.y));
    }

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
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
            if (ImGui::BeginTooltip())
            {
                // Set padding and indetation
                const float PADDING = 10.f;
                ImGui::Dummy({ PADDING, PADDING });
                ImGui::Indent(PADDING);

                // Tooltip content
                ImGui::PushFont(FONT_BOLD);
                ImGui::TextUnformatted("The following controls ONLY work if scene panel focused!");

                ImGui::Dummy({ PADDING, PADDING });

                ImGui::SeparatorText("Editor Controls");
                ImGui::PopFont();
                ImGui::BulletText("Press 'Tab' to toggle editor");
                ImGui::Dummy({ PADDING, PADDING });

                ImGui::PushFont(FONT_BOLD);
                ImGui::SeparatorText("Gizmo Controls");
                ImGui::PopFont();
                ImGui::BulletText("Press 'Q' to disable gizmo");
                ImGui::BulletText("Press 'W' to switch gizmo to translation mode");
                ImGui::BulletText("Press 'E' to switch gizmo to rotation mode");
                ImGui::BulletText("Press 'R' to switch gizmo to scaling mode");
                ImGui::Dummy({ PADDING, PADDING });

                ImGui::PushFont(FONT_BOLD);
                ImGui::SeparatorText("Camera Controls");
                ImGui::PopFont();
                ImGui::BulletText("Scroll to zoom in/out");
                ImGui::BulletText("Drag right mouse button to pan around");
                ImGui::BulletText("Press 'Up' to move camera closer");
                ImGui::BulletText("Press 'Down' key to move further");
                ImGui::BulletText("Press 'Left' key to move left");
                ImGui::BulletText("Press 'Right' key to move right");
                ImGui::Dummy({ PADDING, PADDING });

                ImGui::PushFont(FONT_BOLD);
                ImGui::SeparatorText("Physics Controls");
                ImGui::PopFont();
                ImGui::BulletText("Press 'Shift' + 'Enter' to freeze frame");
                ImGui::BulletText("Press 'Enter' to step frame");
                ImGui::BulletText("Press '9' to spawn rigidbody+collider entity");
                ImGui::Dummy({ PADDING, PADDING });

                ImGui::PushFont(FONT_BOLD);
                ImGui::SeparatorText("Audio Controls");
                ImGui::PopFont();
                ImGui::BulletText("Press 'Z' to play sfx");
                ImGui::BulletText("Press 'X' to play music");

                // Reset padding and indentation
                ImGui::Unindent(PADDING);
                ImGui::Dummy({ PADDING, PADDING });

                ImGui::EndTooltip();
            }
            ImGui::PopStyleVar();
        }

        // Icon to be displayed as overlay
        window_drawlist->AddCircleFilled(circle_center, CIRCLE_RADIUS, IM_COL32(255, 255, 255, 50));
        window_drawlist->AddCircle(circle_center, CIRCLE_RADIUS, IM_COL32_WHITE);
        window_drawlist->AddText(text_position, IM_COL32_WHITE, display_text);
    }

    void ScenePanel::RenderGizmo()
    {
        if (!mEditorLayer.IsAnyEntitySelected() || mGizmoType == aGizmoType::GIZMO_TYPE_INVALID)
        {
            mGizmoInUse = false;
            return;
        }

        auto& engine = InsightEngine::Instance();
        Entity selected_entity = mEditorLayer.GetSelectedEntity();

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(mViewportBounds[0].x, mViewportBounds[0].y,
                          mViewportBounds[1].x - mViewportBounds[0].x,
                          mViewportBounds[1].y - mViewportBounds[0].y);

        auto& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
        const glm::mat4 view = camera.GetViewMatrix();
        const glm::mat4 projection = camera.GetProjectionMatrix();
        auto& transform = engine.GetComponent<Transform>(selected_entity);
        glm::mat4 transform_matrix = transform.FUCKYK();

        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
                             static_cast<ImGuizmo::OPERATION>(mGizmoType),
                             ImGuizmo::LOCAL, glm::value_ptr(transform_matrix));

        if (!ImGuizmo::IsUsing())
        {
            mGizmoInUse = false;
            return;
        }

        glm::vec3 translation, rotation, scale;
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform_matrix), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));
        float delta_rot = rotation.z - transform.rotation;

        transform.world_position = { translation.x, translation.y };
        transform.rotation = fmod((transform.rotation + delta_rot + 360.0f), 360.0f);
        transform.scaling = { std::max(scale.x, 1.f), std::max(scale.y, 1.f) };

        mGizmoInUse = true;

    } // end RenderGizmo()

    // Performance Viewer Panel
    void PerformancePanel::RenderPanel()
    {
        ImGuiIO& io = ImGui::GetIO();
        auto FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];

        // Text Colors
        const ImVec4 RED_COLOR = { 1.f, 0.3f, 0.2f, 1.f };
        const ImVec4 YELLOW_COLOR = { 1.f, .98f, 0.5f, 1.f };
        const ImVec4 WHITE_COLOR = { 1.f, 1.f, 1.f, 1.f };

        ImGui::Begin(mName.c_str());
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
            const char* fps_options[] = { "30", "60", "120", "240", "9999" };
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
            ImGui::TextColored(text_color, "%.3lf ms", io.DeltaTime * 1000.0);

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
                    ImGui::TextColored(text_color, "%.3lf ms", dt * 1000.0);

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
    void ConsolePanel::RenderPanel()
    {
        auto& logger_gui = Logger::LoggerGUI::Instance();
        logger_gui.Draw(mName.c_str());
        mPanelSize = logger_gui.mPanelSize;
    }

    Vec2 ConsolePanel::GetPanelSize() const { return mPanelSize; }

    // Physics Control Panel
    void SettingsPanel::RenderPanel()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFont* FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];

        ImGui::Begin(mName.c_str());

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
        ImGui::End(); // end window Settings
    }

} // end namespace IS
