/*!
 * \file ScenePanel.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 12-11-2023
 * \brief
 * This source file defines the implementation for class ScenePanel, which encapsulates
 * the functionalities of a scene viewport panel.
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
#include "ScenePanel.h"
#include "EditorUtils.h"
#include "CommandHistory.h"

#include <imgui.h>
#include <imgui_internal.h>

#pragma warning(push)
#pragma warning(disable: 4201) // nonstandard nameless struct
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#pragma warning(pop)

namespace IS {

    void ScenePanel::UpdatePanel()
    {
        auto& engine = InsightEngine::Instance();
        auto const input = engine.GetSystem<InputManager>("Input");

        const bool CTRL_HELD = input->IsKeyHeld(GLFW_KEY_LEFT_CONTROL) || input->IsKeyHeld(GLFW_KEY_RIGHT_CONTROL);
        const bool Q_PRESSED = input->IsKeyPressed(GLFW_KEY_Q);
        const bool W_PRESSED = input->IsKeyPressed(GLFW_KEY_W);
        const bool E_PRESSED = input->IsKeyPressed(GLFW_KEY_E);
        const bool R_PRESSED = input->IsKeyPressed(GLFW_KEY_R);

        mSnap = CTRL_HELD;

        // Keyboard inputs
        if (mHovered && !mGizmoInUse)
        {
            if (Q_PRESSED) { mGizmoType = aGizmoType::GIZMO_TYPE_INVALID; }
            if (W_PRESSED) { mGizmoType = aGizmoType::GIZMO_TYPE_TRANSLATE; }
            if (E_PRESSED) { mGizmoType = aGizmoType::GIZMO_TYPE_ROTATE; }
            if (R_PRESSED) { mGizmoType = aGizmoType::GIZMO_TYPE_SCALE; }
        }

        // Auto pause game if game panel is not in focus
        if (mAppearing)
        {
            engine.mRuntime = false;
            Camera3D::mActiveCamera = CAMERA_TYPE_EDITOR;
        }

        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            mHoveredEntity.reset();
        }

        if (!mEditorLayer.IsGamePanelFocused())
        {
            auto [mx, my] = ImGui::GetMousePos();
            mx -= mViewportBounds[0].x;
            my -= mViewportBounds[0].y;
            my = mSize.y - my;
            int mouse_x = static_cast<int>(mx);
            int mouse_y = static_cast<int>(my);

            // Check if mouse is within bounds of the scene panel
            if (mHovered && !mToolbarInUse)
            {
                ZoomCamera();
                PanCamera();
                MoveCamera(10.f);
                RotateCamera();

                // Mouse picking - set hovered entity as the selected entity
                if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGuizmo::IsOver())
                {
                    int pixel_data = ISGraphics::mFramebuffer->ReadPixel(mouse_x, mouse_y);
                    mHoveredEntity = (pixel_data < 0 || pixel_data > MAX_ENTITIES) ? nullptr : std::make_shared<Entity>(pixel_data);
                    mEditorLayer.SetSelectedEntity(mHoveredEntity);

                    // Set inspect mode to inspect entity
                    if (mEditorLayer.IsAnyEntitySelected())
                        mEditorLayer.SetInspectMode(InspectorPanel::aInspectMode::INSPECT_ENTITY);
                }
            }
        }
    } // end UpdatePanel()

    void ScenePanel::RenderPanel()
    {
        auto& engine = InsightEngine::Instance();
        auto input = engine.GetSystem<InputManager>("Input");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

        ImGuiWindowFlags window_flags = 0;
        ImGui::Begin((ICON_LC_VIEW "  " + mName).c_str(), nullptr, window_flags);
        
        // Window contents
        {
            auto viewport_lower_bound = ImGui::GetWindowContentRegionMin();
            auto viewport_upper_bound = ImGui::GetWindowContentRegionMax();
            auto viewport_offset = ImGui::GetWindowPos();
            mViewportBounds[0] = { viewport_lower_bound.x + viewport_offset.x, viewport_lower_bound.y + viewport_offset.y };
            mViewportBounds[1] = { viewport_upper_bound.x + viewport_offset.x, viewport_upper_bound.y + viewport_offset.y };

            // Allow key/mouse event pass through only in this panel
            //if (mFocused)
            //{
            //    ImGuiIO& io = ImGui::GetIO();
            //    io.WantCaptureMouse = io.WantCaptureKeyboard = false;
            //}

            // Size of scene panel
            ImVec2 scene_size = ImGui::GetWindowSize();
            ImVec2 scene_pos = ImGui::GetWindowPos();

            // Scene pos for the input
            mSize.x = scene_pos.x - mEditorLayer.GetDockspacePosition().x;
            mSize.y = scene_pos.y - mEditorLayer.GetDockspacePosition().y;

            auto& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];

            input->setCenterPos(mSize.x + (float)scene_size.x / 2.f, mSize.y + (float)scene_size.y / 2.f);
            input->setRatio(scene_size.x * camera.GetZoomLevel(), scene_size.y * camera.GetZoomLevel());

            // Resize framebuffer
            ImVec2 panel_size = ImGui::GetContentRegionAvail();
            mSize = { panel_size.x, panel_size.y };

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
            if (Camera3D::mActiveCamera == CAMERA_TYPE_EDITOR)
            {
                RenderHelp();
                RenderToolbar();
            }

            // Render gizmo
            RenderGizmo();
        }

        // Save window states
        mFocused = ImGui::IsWindowFocused();
        mAppearing = ImGui::IsWindowAppearing();
        mHovered = ImGui::IsItemHovered();
        ImGui::End(); // end window Scene
        ImGui::PopStyleVar();

    } // end RenderPanel()

    bool ScenePanel::MouseWithinViewport() const
    {
        auto [mx, my] = ImGui::GetMousePos();
        mx -= mViewportBounds[0].x;
        my -= mViewportBounds[0].y;
        my = mSize.y - my;
        int mouse_x = static_cast<int>(mx);
        int mouse_y = static_cast<int>(my);

        return (0 <= mouse_x && mouse_x < static_cast<int>(mSize.x) &&
                0 <= mouse_y && mouse_y < static_cast<int>(mSize.y));
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
                ImGui::BulletText("Hold 'Ctrl' to snap gizmo");
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

    void ScenePanel::RenderToolbar()
    {
        const bool TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED = true;

        // ImGuiAxis_X = horizontal toolbar
        // ImGuiAxis_Y = vertical toolbar
        static ImGuiAxis toolbar_axis = ImGuiAxis_Y;
        ImVec2 requested_size = (toolbar_axis == ImGuiAxis_X) ? ImVec2(-1.0f, 0.0f) : ImVec2(0.0f, -1.0f);
        ImGui::SetNextWindowSize(requested_size);
        ImGui::SetNextWindowBgAlpha(0.5f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 5.f, 5.f});

        // 2. Specific docking options for toolbars.
        // Currently they add some constraint we ideally wouldn't want, but this is simplifying our first implementation
        ImGuiWindowClass window_class;
        window_class.DockingAllowUnclassed = true;
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoCloseButton;
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_HiddenTabBar; // ImGuiDockNodeFlags_NoTabBar // FIXME: Will need a working Undock widget for _NoTabBar to work
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingSplitMe;
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;

        if (toolbar_axis == ImGuiAxis_X)
            window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeY;
        else
            window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeX;
        ImGui::SetNextWindowClass(&window_class);

        // 3. Begin into the window
        const float font_size = ImGui::GetFontSize();
        const ImVec2 icon_size(ImFloor(font_size * 1.7f), ImFloor(font_size * 1.7f));
        ImGui::Begin("ScenePanelToolbar", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
        mToolbarInUse = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
        // 4. Overwrite node size
        ImGuiDockNode* node = ImGui::GetWindowDockNode();
        if (node != NULL)
        {
            // Overwrite size of the node
            ImGuiStyle& style = ImGui::GetStyle();
            const ImGuiAxis toolbar_axis_perp = (ImGuiAxis)(toolbar_axis ^ 1);
            const float TOOLBAR_SIZE_WHEN_DOCKED = style.WindowPadding[toolbar_axis_perp] * 2.0f + icon_size[toolbar_axis_perp];
            node->WantLockSizeOnce = true;
            node->Size[toolbar_axis_perp] = node->SizeRef[toolbar_axis_perp] = TOOLBAR_SIZE_WHEN_DOCKED;

            if (TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED)
                if (node->ParentNode && node->ParentNode->SplitAxis != ImGuiAxis_None)
                    toolbar_axis = (ImGuiAxis)(node->ParentNode->SplitAxis ^ 1);
        }

        // Populate tab bar
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, .0f));
        ImGuiStyle& style = ImGui::GetStyle();

        const ImColor GREY_COLOR = ImColor(150, 150, 150);
        static ImColor color = GREY_COLOR;
        if (toolbar_axis == ImGuiAxis_X)
        {
            ImGui::SetCursorPosY((ImGui::GetCursorPos().y + ImGui::CalcTextSize(ICON_LC_GRIP_VERTICAL).y) / 2.f);
            ImGui::TextColored(color, ICON_LC_GRIP_VERTICAL);
            ImGui::SameLine();
            ImGui::SetCursorPosY((ImGui::GetCursorPos().y - style.FramePadding.y * 2));
        }
        else
        {
            ImGui::SetCursorPosX((ImGui::GetCursorPos().x + ImGui::CalcTextSize(ICON_LC_GRIP_HORIZONTAL).x) / 2.f);
            ImGui::TextColored(color, ICON_LC_GRIP_HORIZONTAL);
        }
        color = mToolbarInUse ? ImColor(IM_COL32_WHITE) : GREY_COLOR;

        const int BUTTON_COUNT = 4;
        aGizmoType gizmo_types[BUTTON_COUNT] = { aGizmoType::GIZMO_TYPE_INVALID, aGizmoType::GIZMO_TYPE_TRANSLATE, aGizmoType::GIZMO_TYPE_ROTATE, aGizmoType::GIZMO_TYPE_SCALE };
        const char* icons[BUTTON_COUNT] = { ICON_LC_HAND, ICON_LC_MOVE, ICON_LC_REFRESH_CCW, ICON_LC_SCALING };
        const char* tooltips[BUTTON_COUNT] = { "View Tool", "Move Tool", "Rotate Tool", "Scale Tool" };
        bool buttons[BUTTON_COUNT] = {};

        for (int i{}; i < BUTTON_COUNT; ++i)
        {
            // Set item default focus
            if (gizmo_types[i] == mGizmoType)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_ButtonActive]);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_ButtonActive]);
            }

            if (i > 0 && toolbar_axis == ImGuiAxis_X)
            {
                ImGui::SameLine();
                ImGui::SetCursorPosY((ImGui::GetCursorPos().y - style.FramePadding.y * 2));
            }
            buttons[i] = ImGui::Button(icons[i], icon_size);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
            ImGui::SetItemTooltip(tooltips[i]);
            ImGui::PopStyleVar();

            if (gizmo_types[i] == mGizmoType)
                ImGui::PopStyleColor(2);
        }
        ImGui::PopStyleVar();

        // Context-menu to change axis
        if (node == nullptr || !TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED)
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Horizontal", "", (toolbar_axis == ImGuiAxis_X)))
                    toolbar_axis = ImGuiAxis_X;
                if (ImGui::MenuItem("Vertical", "", (toolbar_axis == ImGuiAxis_Y)))
                    toolbar_axis = ImGuiAxis_Y;
                ImGui::EndPopup();
            }
        }

        // Change gizmo type
        mGizmoType = buttons[0] ? gizmo_types[0] :
                     buttons[1] ? gizmo_types[1] :
                     buttons[2] ? gizmo_types[2] :
                     buttons[3] ? gizmo_types[3] : mGizmoType;

        ImGui::End();

        ImGui::PopStyleVar(3);
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

        float snap_value{};
        switch (mGizmoType)
        {
        case aGizmoType::GIZMO_TYPE_TRANSLATE:  snap_value = 50.f; break;
        case aGizmoType::GIZMO_TYPE_ROTATE:     snap_value = 15.f; break;
        case aGizmoType::GIZMO_TYPE_SCALE:      snap_value = 1.f; break;
        default: break;
        }
        float snap_values[3] = { snap_value, snap_value, snap_value };

        static bool manipulating = false;
        bool manipulated = ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
                                                static_cast<ImGuizmo::OPERATION>(mGizmoType),
                                                ImGuizmo::LOCAL, glm::value_ptr(transform_matrix),
                                                nullptr, mSnap ? snap_values : nullptr);

        if (!ImGuizmo::IsUsing())
        {
            mGizmoInUse = false;
            return;
        }

        if (manipulating)

        if (manipulated)
        {
            glm::vec3 translation3D, rotation3D, scale3D;
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform_matrix), glm::value_ptr(translation3D), glm::value_ptr(rotation3D), glm::value_ptr(scale3D));

            float delta_rot = rotation3D.z - transform.rotation;
            Vec2 translation{ translation3D.x, translation3D.y }, scale{ std::max(scale3D.x, 1.f), std::max(scale3D.y, 1.f) };
            float rotation = fmod((transform.rotation + delta_rot + 360.0f), 360.0f);

            transform.world_position = translation;
            transform.rotation = rotation;
            transform.scaling = scale;
        }


        mGizmoInUse = true;

    } // end RenderGizmo()

    void ScenePanel::ZoomCamera()
    {
        Camera3D& camera = ISGraphics::cameras3D[CAMERA_TYPE_EDITOR];
        ImGuiIO& io = ImGui::GetIO();
        float scroll_delta = io.MouseWheel;
        float zoom_level = camera.GetZoomLevel();
        if (scroll_delta != 0)
        {
            zoom_level *= (scroll_delta > 0) ? (1 + Camera::mZoomSpeed) : (1 - Camera::mZoomSpeed);
        }
        camera.SetZoomLevel(zoom_level);

    } // end ZoomCamera()

    void ScenePanel::PanCamera()
    {
        static bool is_panning = false;
        static ImVec2 previous_mouse_position;
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 mouse_position = io.MousePos;
        Camera3D& camera = ISGraphics::cameras3D[CAMERA_TYPE_EDITOR];

        if (!io.MouseDown[1])
        {
            is_panning = false;
            return;
        }

        if (!is_panning)
        {
            is_panning = true;
            previous_mouse_position = mouse_position;
        }

        // Calculate the panning offset
        ImVec2 delta = ImVec2(mouse_position.x - previous_mouse_position.x, mouse_position.y - previous_mouse_position.y);
        camera.PanCamera(delta.x, delta.y);
        previous_mouse_position = mouse_position;

    } // end PanCamera()

    void ScenePanel::MoveCamera(float move_speed)
    {
        Camera3D& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
        camera.MoveCamera(move_speed);
    }

    void ScenePanel::RotateCamera()
    {
        auto& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
        ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle);
        camera.Rotate(drag_delta.x, drag_delta.y);
        ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
    }

} // end namespace IS
