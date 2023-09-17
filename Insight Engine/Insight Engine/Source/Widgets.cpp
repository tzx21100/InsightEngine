#include "Pch.h"
#include "Widgets.h"

// Dependencies
#include <imgui.h>
#include <imgui_internal.h>

namespace IS {

    void RenderControlVec2(std::string const& label, Vector2D& values, float x_reset, float y_reset, float column_width) {
        ImGuiTableFlags flags = ImGuiTableFlags_Resizable;

        ImGui::PushID(label.c_str());

        if (ImGui::BeginTable(label.c_str(), 2, flags, ImVec2(0, 0), column_width)) {
            ImGui::TableNextColumn();
            ImGui::Text(label.c_str());
            ImGui::TableNextColumn();

            ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

            float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
            ImVec2 button_size = { line_height + 3.f, line_height };

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.8f, .1f, .15f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.9f, .2f, .2f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.8f, .1f, .15f, 1.f));
            if (ImGui::Button("X", button_size))
                values.x = x_reset;
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##X", &values.x, .1f, 0.f, 0.f, "%.2f");
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.2f, .7f, .2f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.3f, .8f, .3f, 1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.2f, .7f, .2f, 1.f));
            if (ImGui::Button("Y", button_size))
                values.y = y_reset;
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##Y", &values.y, .1f, 0.f, 0.f, "%.2f");
            ImGui::PopItemWidth();

            ImGui::PopStyleVar();
            ImGui::EndTable();
            ImGui::Separator();
        }

        ImGui::PopID();
    }

} // end namespace IS
