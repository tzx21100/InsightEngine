/*!
 * \file EditorUtils.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for editor utility functions
 * such as custom widgets.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_UTILS_EDITOR_UTILS_H
#define GAM200_INSIGHT_ENGINE_EDITOR_UTILS_EDITOR_UTILS_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "ISMath.h"

#include <imgui_internal.h>

namespace IS {

    /*!
     * \brief Type of font stored in imgui fonts array.
     */
    enum aFontType : int {
        FONT_TYPE_DEFAULT = 0,
        FONT_TYPE_BOLD,
        FONT_TYPE_ITALIC,
        FONT_TYPE_LOG_DEFAULT,
        FONT_TYPE_LOG_BOLD
    };

    /*!
     * \brief Namespace containing all utility functions for the editor.
     */
    namespace EditorUtils {

        /*!
         * \brief Converts tex id from GLuint to ImTextureID.
         * 
         * \param tex_id Texture id to be converted.
         * \return The ImTextureID version of the given texture id.
         */
        ImTextureID ConvertTextureID(GLuint tex_id);

        /*!
         * \brief -operator overload for ImVec2.
         * 
         * \param lhs Left hand side of the binary operation.
         * \param rhs Right hand side of the binary operation.
         * \return The resulting subtracted value.
         */
        ImVec2 operator-(ImVec2 const& lhs, ImVec2 const& rhs);

        /*!
         * \brief +operator overload for ImVec2.
         * 
         * \param lhs Left hand side of the binary operation.
         * \param rhs Right hand side of the binary operation.
         * \return The resulting added value.
         */
        ImVec2 operator+(ImVec2 const& lhs, ImVec2 const& rhs);

        bool operator==(ImVec2 const& lhs, ImVec2 const& rhs);

        /*!
         * \brief Testing if point is within a circle.
         * 
         * \param point Position of the point.
         * \param center Center of the circle.
         * \param radius Radius of the circle.
         * \return bool indicating whether point is within the cicrle.
         */
        bool TestPointCircle(ImVec2 const& point, ImVec2 const& center, float radius);

        /*!
         * \brief Renders the filter input text box with hint text.
         * 
         * \param filter The imgui text filter to render.
         * \param hint The hint to render in the filter input text box.
         */
        void RenderFilterWithHint(ImGuiTextFilter& filter, const char* hint);

        /*!
         * \brief Renders a control for a 2D vector using ImGui.
         *
         * This function renders a control for a 2D vector with provided label and values using ImGui.
         * The control allows resetting the x and y values to specified values.
         *
         * \param label The label for the control.
         * \param values The reference to the Vector2D to be controlled and displayed.
         * \param x_reset The value to reset the x-component to (default is 0.0f).
         * \param y_reset The value to reset the y-component to (default is 0.0f).
         * \param column_width The width of the control column (default is 100.0f).
         * \return Boolean value indicating whether control has been adjusted.
         */
        bool RenderControlVec2(std::string const& label, Vector2D& values, float x_reset = 0.f, float y_reset = 0.f, float column_width = 100.f);

        bool RenderControlVec3(std::string const& label, Vector3D& values, float x_reset = 0.f, float y_reset = 0.f, float z_reset = 0.f, float column_width = 100.f);

        /*!
         * \brief Renders a combo box for an enumeration type using ImGui.
         *
         * This function renders a combo box for an enumeration type with provided label and options using ImGui.
         *
         * \tparam EnumType The enumeration type.
         * 
         * \param label The label for the combo box.
         * \param enum_value The reference to the enumeration value to be controlled and displayed.
         * \param item_list The list of item names for the combo box.
         */
        template <typename EnumType>
        void RenderComboBoxEnum(const char* label, EnumType& enum_value, std::initializer_list<const char*> item_list) {
            auto longest_item = std::max_element(item_list.begin(), item_list.end(), [](const char* lhs, const char* rhs) {
                return std::strlen(lhs) < std::strlen(rhs);
            });
            std::vector<const char*> items = item_list;
            size_t current_item = static_cast<size_t>(enum_value);

            // Compute the width of the combo box based on the longest item
            ImGuiStyle& style = ImGui::GetStyle();
            const float PADDING = style.FramePadding.x;
            const float LONGEST_ITEM_WIDTH = ImGui::CalcTextSize(*longest_item).x + 2 * PADDING;
            const float ARROW_WIDTH = style.ItemSpacing.x + 2 * PADDING;
            const float COMBO_WIDTH = LONGEST_ITEM_WIDTH + ARROW_WIDTH + PADDING;

            ImGui::SetNextItemWidth(COMBO_WIDTH);
            if (ImGui::BeginCombo(label, items[current_item])) {
                for (size_t i{}; i < items.size(); ++i) {
                    const bool is_selected = (current_item == i);
                    if (ImGui::Selectable(items[i], is_selected)) {
                        current_item = i;
                        enum_value = static_cast<EnumType>(i);
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }

        void DockingToolbar(const char* name, ImGuiAxis* p_toolbar_axis);

    } // end namespace EditorUtils

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_UTILS_EDITOR_UTILS_H
