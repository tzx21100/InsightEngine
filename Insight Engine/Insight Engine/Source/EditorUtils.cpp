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
#include <imgui_internal.h> // PushMultiItemsWidths
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

    void RenderControlVec2(std::string const& label, Vector2D& values, float x_reset, float y_reset, float column_width)
    {
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
                values.x = x_reset;
            ImGui::PopFont();
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##X", &values.x, .1f, 0.f, 0.f, "%.2f");
            ImGui::PopItemWidth();
            ImGui::SameLine();

            // Render the Y component with green color
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.44f, .67f, .01f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.55f, .74f, .21f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.44f, .67f, .01f, 1.f));
            ImGui::PushFont(FONT_BOLD);
            if (ImGui::Button("Y", button_size))
                values.y = y_reset;
            ImGui::PopFont();
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##Y", &values.y, .1f, 0.f, 0.f, "%.2f");
            ImGui::PopItemWidth();

            ImGui::PopStyleVar();
            ImGui::EndTable();
        }

        ImGui::PopID();
    }

} // end namespace IS::EditorUtils
