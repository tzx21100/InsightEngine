#include "Pch.h"
#include "Guidgets.h"

// Dependencies
#include <imgui.h>
#include <imgui_internal.h>

namespace IS {

    namespace guidgets {

        void RenderControlVec2(std::string const& label, Vector2D& values, float x_reset, float y_reset, float column_width) {
            ImGuiTableFlags flags = ImGuiTableFlags_PreciseWidths;

            ImGui::PushID(label.c_str());

            if (ImGui::BeginTable(label.c_str(), 2, flags, ImVec2(0, 0), column_width)) {
                ImGui::TableNextColumn();
                ImGui::Text(label.c_str());
                ImGui::TableNextColumn();

                ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);

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

                ImGui::PopStyleVar(2);
                ImGui::EndTable();
                ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 0.5f);
            }

            ImGui::PopID();
        }

        void RenderComboBoxBodyType(std::string const& label, BodyType& bodytype) {
            std::array<std::string, 3> items = { "Static", "Dynamic", "Kinematic" };

            size_t current_item = static_cast<size_t>(bodytype);

            if (ImGui::BeginCombo(label.c_str(), items[current_item].c_str())) {
                for (size_t i{}; i < items.size(); ++i) {
                    const bool is_selected = (current_item == i);
                    if (ImGui::Selectable(items[i].c_str(), is_selected)) {
                        current_item = i;
                        bodytype = static_cast<BodyType>(i);
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }

        void RenderComboBoxBodyShape(std::string const& label, Shape& bodyshape) {
            std::array<std::string, 3> items = { "Box", "Circle", "Line" };

            size_t current_item = static_cast<size_t>(bodyshape);

            if (ImGui::BeginCombo(label.c_str(), items[current_item].c_str())) {
                for (size_t i{}; i < items.size(); ++i) {
                    const bool is_selected = (current_item == i);
                    if (ImGui::Selectable(items[i].c_str(), is_selected)) {
                        current_item = i;
                        bodyshape = static_cast<Shape>(i);
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }

    } // end namespace guidgets

} // end namespace IS
