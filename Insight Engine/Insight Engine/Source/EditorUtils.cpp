/*!
 * \file EditorUtils.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for editor utility functions
 * such as custom widgets.
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
#include "EditorUtils.h"

// Dependencies
#include <imgui.h>
#include <IconsLucide.h>

namespace IS::EditorUtils {

    ImTextureID ConvertTextureID(GLuint tex_id) { return std::bit_cast<ImTextureID>(static_cast<uintptr_t>(tex_id)); }

    ImVec2 operator-(ImVec2 const& lhs, ImVec2 const& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

    ImVec2 operator+(ImVec2 const& lhs, ImVec2 const& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }

    bool operator==(ImVec2 const& lhs, ImVec2 const& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }

    bool TestPointCircle(const ImVec2& point, const ImVec2& center, float radius)
    {
        ImFont* const& FONT_BOLD = ImGui::GetIO().Fonts->Fonts[FONT_TYPE_BOLD];
        float distance = FONT_BOLD->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, " ").x; // Minimum size for text, use any ImGui widget you prefer.
        ImVec2 delta = point - center;
        return delta.x * delta.x + delta.y * delta.y < (radius + distance) * (radius + distance);
    }

    void RenderFilterWithHint(ImGuiTextFilter& filter, const char* hint)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFont* const& font_italic = io.Fonts->Fonts[FONT_TYPE_ITALIC];
        float original_cursor_xpos = ImGui::GetCursorPosX();

        // Render filter input text box
        ImGui::SetNextItemAllowOverlap();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        filter.Draw();

        // Render hint text if search box is empty
        if (!filter.IsActive())
        {
            ImGui::SameLine();
            ImGui::SetCursorPosX(original_cursor_xpos + ImGui::GetStyle().FramePadding.x);
            ImGui::PushFont(font_italic);
            ImGui::TextColored({ .8f, .8f, .8f, .8f }, (ICON_LC_FILTER "  " + std::string(hint)).c_str());
            ImGui::PopFont();
        }
    }

    bool RenderControlVec2(std::string const& label, Vector2D& values, float x_reset, float y_reset, float column_width)
    {
        bool adjusted = false;

        ImGuiIO& io = ImGui::GetIO();
        ImFont* const& FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];

        ImGuiTableFlags table_flags = ImGuiTableFlags_PreciseWidths;

        ImGui::PushID(label.c_str());

        if (ImGui::BeginTable(label.c_str(), 2, table_flags))
        {
            ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
            ImGui::TableSetupColumn(label.c_str(), column_flags, column_width);
            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted(label.c_str());
            ImGui::PopFont();
            ImGui::TableNextColumn();

            // Set the width for both columns
            ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

            float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
            ImVec2 button_size = { line_height + 3.f, line_height };

            // Render the X component with red color
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.77f, .16f, .04f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.84f, .31f, .25f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.77f, .16f, .04f, 1.f));
            ImGui::PushFont(FONT_BOLD);
            if (ImGui::Button("X", button_size))
            {
                values.x = x_reset;
                adjusted = true;
            }
            ImGui::PopFont();
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            if (ImGui::DragFloat("##X", &values.x, .1f, 0.f, 0.f, "%.2f"))
            {
                adjusted = true;
            }
            ImGui::PopItemWidth();
            ImGui::SameLine();

            // Render the Y component with green color
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.44f, .67f, .01f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.55f, .74f, .21f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.44f, .67f, .01f, 1.f));
            ImGui::PushFont(FONT_BOLD);
            if (ImGui::Button("Y", button_size))
            {
                values.y = y_reset;
                adjusted = true;
            }
            ImGui::PopFont();
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            if (ImGui::DragFloat("##Y", &values.y, .1f, 0.f, 0.f, "%.2f"))
            {
                adjusted = true;
            }
            ImGui::PopItemWidth();

            ImGui::PopStyleVar();
            ImGui::EndTable();
        }

        ImGui::PopID();
        return adjusted;
    }

    void DockingToolbar(const char* name, ImGuiAxis* p_toolbar_axis)
    {
        // [Option] Automatically update axis based on parent split (inside of doing it via right-click on the toolbar)
        // Pros:
        // - Less user intervention.
        // - Avoid for need for saving the toolbar direction, since it's automatic.
        // Cons: 
        // - This is currently leading to some glitches.
        // - Some docking setup won't return the axis the user would expect.
        const bool TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED = true;

        // ImGuiAxis_X = horizontal toolbar
        // ImGuiAxis_Y = vertical toolbar
        ImGuiAxis toolbar_axis = *p_toolbar_axis;

        // 1. We request auto-sizing on one axis
        // Note however this will only affect the toolbar when NOT docked.
        ImVec2 requested_size = (toolbar_axis == ImGuiAxis_X) ? ImVec2(-1.0f, 0.0f) : ImVec2(0.0f, -1.0f);
        ImGui::SetNextWindowSize(requested_size);

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
        ImGui::Begin(name, NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

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

        // 5. Dummy populate tab bar
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));
        //UndockWidget(icon_size, toolbar_axis);
        for (int icon_n = 0; icon_n < 10; icon_n++)
        {
            char label[32];
            ImFormatString(label, IM_ARRAYSIZE(label), "%02d", icon_n);
            if (icon_n > 0 && toolbar_axis == ImGuiAxis_X)
                ImGui::SameLine();
            ImGui::Button(label, icon_size);
        }
        ImGui::PopStyleVar(2);

        // 6. Context-menu to change axis
        if (node == NULL || !TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED)
        {
            if (ImGui::BeginPopupContextWindow())
            {
                ImGui::TextUnformatted(name);
                ImGui::Separator();
                if (ImGui::MenuItem("Horizontal", "", (toolbar_axis == ImGuiAxis_X)))
                    toolbar_axis = ImGuiAxis_X;
                if (ImGui::MenuItem("Vertical", "", (toolbar_axis == ImGuiAxis_Y)))
                    toolbar_axis = ImGuiAxis_Y;
                ImGui::EndPopup();
            }
        }

        ImGui::End();

        // Output user stored data
        *p_toolbar_axis = toolbar_axis;
    }

} // end namespace IS::EditorUtils
