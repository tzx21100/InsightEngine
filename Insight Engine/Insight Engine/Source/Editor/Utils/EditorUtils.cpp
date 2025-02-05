/*!
 * \file EditorUtils.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for editor utility functions
 * such as custom widgets.
 * 
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#ifdef USING_IMGUI
#include "EditorUtils.h"
#include "Editor/Commands/CommandHistory.h"

// Dependencies
#include <imgui.h>
#include <imgui_internal.h>
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

    WidgetState RenderControlVec2(std::string const& label, Vector2D& values, float x_reset, float y_reset, float column_width)
    {
        WidgetState state;

        ImGui::PushID(label.c_str());

        EditorUtils::RenderTableFixedWidth(label, 2, [&]()
        {
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(label.c_str());
            ImGui::TableNextColumn();

            // Set the width for both columns
            ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

            float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
            ImVec2 button_size = { line_height + 3.f, line_height };

            // Render the X component with red color

            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(196, 41, 10, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(214, 79, 64, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(196, 41, 10, 255));
            if (ImGui::Button("X", button_size))
            {
                values.x = x_reset;
                state.mIsModified = true;
            }
            state.mIsActive |= ImGui::IsItemActive();
            state.mIsDeactivated |= ImGui::IsItemDeactivated();
            state.mIsDeactivatedAfterEdit |= ImGui::IsItemDeactivatedAfterEdit();
            ImGui::PopStyleColor(3);

            ImGui::SameLine();

            if (ImGui::DragFloat("##X", &values.x, .1f, 0.f, 0.f, "%.2f"))
            {
                state.mIsModified = true;
            }
            state.mIsActive |= ImGui::IsItemActive();
            state.mIsDeactivated |= ImGui::IsItemDeactivated();
            state.mIsDeactivatedAfterEdit |= ImGui::IsItemDeactivatedAfterEdit();
            ImGui::PopItemWidth();
            ImGui::SameLine();

            // Render the Y component with green color            
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(112, 170, 2, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(140, 189, 53, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(112, 170, 2, 255));
            if (ImGui::Button("Y", button_size))
            {
                values.y = y_reset;
                state.mIsModified = true;
            }
            state.mIsActive |= ImGui::IsItemActive();
            state.mIsDeactivated |= ImGui::IsItemDeactivated();
            state.mIsDeactivatedAfterEdit |= ImGui::IsItemDeactivatedAfterEdit();
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            if (ImGui::DragFloat("##Y", &values.y, .1f, 0.f, 0.f, "%.2f"))
            {
                state.mIsModified = true;
            }
            state.mIsActive |= ImGui::IsItemActive();
            state.mIsDeactivated |= ImGui::IsItemDeactivated();
            state.mIsDeactivatedAfterEdit |= ImGui::IsItemDeactivatedAfterEdit();
            ImGui::PopItemWidth();

            ImGui::PopStyleVar();
        }, 0, column_width);

        ImGui::PopID();
        return state;
    }

    WidgetState RenderControlVec3(std::string const& label, Vector3D& values, float x_reset, float y_reset, float z_reset, float column_width)
    {
        WidgetState state;
        ImGui::PushID(label.c_str());

        EditorUtils::RenderTableFixedWidth(label, 2, [&]()
        {
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(label.c_str());
            ImGui::TableNextColumn();

            // Set the width for both columns
            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

            float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
            ImVec2 button_size = { line_height + 3.f, line_height };

            // Render the X component with red color
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(196, 41, 10, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(214, 79, 64, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(196, 41, 10, 255));
            if (ImGui::Button("X", button_size))
            {
                values.x = x_reset;
                state.mIsModified = true;
            }
            state.mIsActive |= ImGui::IsItemActive();
            state.mIsDeactivated |= ImGui::IsItemDeactivated();
            state.mIsDeactivatedAfterEdit |= ImGui::IsItemDeactivatedAfterEdit();
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            if (ImGui::DragFloat("##X", &values.x, .1f, 0.f, 0.f, "%.2f"))
            {
                state.mIsModified = true;
            }
            state.mIsActive |= ImGui::IsItemActive();
            state.mIsDeactivated |= ImGui::IsItemDeactivated();
            state.mIsDeactivatedAfterEdit |= ImGui::IsItemDeactivatedAfterEdit();
            ImGui::PopItemWidth();

            // Render the Y component with green color
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(112, 170, 2, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(140, 189, 53, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(112, 170, 2, 255));
            if (ImGui::Button("Y", button_size))
            {
                values.y = y_reset;
                state.mIsModified = true;
            }
            state.mIsActive |= ImGui::IsItemActive();
            state.mIsDeactivated |= ImGui::IsItemDeactivated();
            state.mIsDeactivatedAfterEdit |= ImGui::IsItemDeactivatedAfterEdit();
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            if (ImGui::DragFloat("##Y", &values.y, .1f, 0.f, 0.f, "%.2f"))
            {
                state.mIsModified = true;
            }
            state.mIsActive |= ImGui::IsItemActive();
            state.mIsDeactivated |= ImGui::IsItemDeactivated();
            state.mIsDeactivatedAfterEdit |= ImGui::IsItemDeactivatedAfterEdit();
            ImGui::PopItemWidth();

            // Render the Z component with blue color
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(59, 119, 214, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(78, 145, 223, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(59, 119, 214, 255));
            if (ImGui::Button("Z", button_size))
            {
                values.z = z_reset;
                state.mIsModified = true;
            }
            state.mIsActive |= ImGui::IsItemActive();
            state.mIsDeactivated |= ImGui::IsItemDeactivated();
            state.mIsDeactivatedAfterEdit |= ImGui::IsItemDeactivatedAfterEdit();
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            if (ImGui::DragFloat("##Z", &values.z, .1f, 0.f, 0.f, "%.2f"))
            {
                state.mIsModified = true;
            }
            state.mIsActive |= ImGui::IsItemActive();
            state.mIsDeactivated |= ImGui::IsItemDeactivated();
            state.mIsDeactivatedAfterEdit |= ImGui::IsItemDeactivatedAfterEdit();
            ImGui::PopItemWidth();

            ImGui::PopStyleVar();

        }, 0, column_width);

        ImGui::PopID();
        return state;
    }

    void RenderTableLabel(std::string const& label, std::string const& tooltip)
    {
        ImGui::TableNextColumn();
        ImGui::TextUnformatted(label.c_str());
        if (!tooltip.empty())
        {
            ImGui::SetItemTooltip(tooltip.c_str());
        }
    }

    void RenderTableBoldLabel(std::string const& label, std::string const& tooltip)
    {
        ImFont* const FONT_BOLD = ImGui::GetIO().Fonts->Fonts[FONT_TYPE_BOLD];
        ImGui::TableNextColumn();
        ImGui::PushFont(FONT_BOLD);
        ImGui::TextUnformatted(label.c_str());
        ImGui::PopFont();
        if (!tooltip.empty())
        {
            ImGui::SetItemTooltip(tooltip.c_str());
        }
    }

    void RenderTableFixedWidth(std::string const& label, int columns, std::function<void(void)> contents, ImGuiTableFlags flags, float label_width)
    {
        if (ImGui::BeginTable(label.c_str(), columns, flags))
        {
            ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
            ImGui::TableSetupColumn("Label", column_flags, label_width);
            contents();
            ImGui::EndTable();
        }
    }

    void RenderTable(std::string const& label, int columns, std::function<void(void)> contents, ImGuiTableFlags flags)
    {
        if (ImGui::BeginTable(label.c_str(), columns, flags))
        {
            contents();
            ImGui::EndTable();
        }
    }

    void RenderTableInputText(std::string& text, std::function<void(void)> action)
    {
        ImGui::TableNextColumn();

        ImGui::PushID(text.c_str());
        char buffer[256]{};
        std::memcpy(buffer, text.c_str(), text.size());
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
        if (ImGui::InputText("##Text", buffer, sizeof(buffer), input_text_flags))
        {
            if (strlen(buffer) > 0)
            {
                CommandHistory::AddCommand<ChangeCommand<std::string>>(text, buffer);
                CommandHistory::SetNoMergeMostRecent();
                if (action != nullptr)
                {
                    action();
                }
            }
        }
        ImGui::PopID();
    }

    void RenderInputText(std::string& text, std::function<void(void)> action)
    {
        ImGui::PushID(text.c_str());
        char buffer[256]{};
        std::memcpy(buffer, text.c_str(), text.size());
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
        if (ImGui::InputText("##Text", buffer, sizeof(buffer), input_text_flags))
        {
            if (strlen(buffer) > 0)
            {
                CommandHistory::AddCommand<ChangeCommand<std::string>>(text, buffer);
                CommandHistory::SetNoMergeMostRecent();
                if (action != nullptr)
                {
                    action();
                }
            }
        }
        ImGui::PopID();
    }

    void RenderToggleButton(std::string const& str_id, bool& value)
    {
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        float height = ImGui::GetFrameHeight();
        float width = height * 1.55f;
        float radius = height * 0.50f;

        if (ImGui::InvisibleButton(str_id.c_str(), ImVec2(width, height)))
        {
            CommandHistory::AddCommand<ChangeCommand<bool>>(value, !value);
            CommandHistory::SetNoMergeMostRecent();
        }
        ImU32 color_background;
        ImVec4 color_active = ImGui::GetStyleColorVec4(ImGuiCol_CheckMark);
        ImVec4 color_active_hovered = { color_active.x * 1.05f, color_active.y, color_active.z * 1.05f, color_active.w };
        if (ImGui::IsItemHovered())
        {
            color_background = value ? ImGui::GetColorU32(color_active_hovered) : ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBgHovered));
        }
        else
        {
            color_background = value ? ImGui::GetColorU32(color_active) : ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
        }

        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), color_background, height * 0.5f);
        draw_list->AddCircleFilled(ImVec2(value ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
    }

} // end namespace IS::EditorUtils


#endif // USING_IMGUI
