/*!
 * \file Guidgets.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for ImGui widgetRenderControlVec2.
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
#include "Guidgets.h"

// Dependencies
#include <imgui.h>
#include <imgui_internal.h> // PushMultiItemsWidths

namespace IS {

    namespace guidgets {

        void RenderControlVec2(std::string const& label, Vector2D& values, float x_reset, float y_reset, float column_width) {
            ImGuiIO& io = ImGui::GetIO();
            auto font_bold = io.Fonts->Fonts[0];

            ImGuiTableFlags table_flags = ImGuiTableFlags_PreciseWidths;

            ImGui::PushID(label.c_str());

            if (ImGui::BeginTable(label.c_str(), 2, table_flags)) {
                ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;
                ImGui::TableSetupColumn(label.c_str(), column_flags, column_width);
                ImGui::TableNextColumn();
                ImGui::PushFont(font_bold);
                ImGui::TextUnformatted(label.c_str());
                ImGui::PopFont();
                ImGui::TableNextColumn();

                ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

                float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
                ImVec2 button_size = { line_height + 3.f, line_height };

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.77f, .16f, .04f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.84f, .31f, .25f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.77f, .16f, .04f, 1.f));
                ImGui::PushFont(font_bold);
                if (ImGui::Button("X", button_size))
                    values.x = x_reset;
                ImGui::PopFont();
                ImGui::PopStyleColor(3);

                ImGui::SameLine();
                ImGui::DragFloat("##X", &values.x, .1f, 0.f, 0.f, "%.2f");
                ImGui::PopItemWidth();
                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.44f, .67f, .01f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.55f, .74f, .21f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.44f, .67f, .01f, 1.f));
                ImGui::PushFont(font_bold);
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

    } // end namespace guidgets

} // end namespace IS
