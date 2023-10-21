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

namespace IS {

    /*!
     * \brief Type of font stored in imgui fonts array.
     */
    enum class aFontType : int { FONT_TYPE_DEFAULT = 0, FONT_TYPE_BOLD, FONT_TYPE_ITALIC };

    /*!
     * \brief Namespace containing all utility functions for the editor.
     */
    namespace EditorUtils {

        /*!
         * \brief Converts font type to int.
         * \param font_type Font type to be converted.
         * \return The integer version of the given font type.
         */
        int FontTypeToInt(aFontType font_type);

        /*!
         * \brief -operator overload for ImVec2.
         * \param lhs Left hand side of the binary operation.
         * \param rhs Right hand side of the binary operation.
         * \return The resulting subtracted value.
         */
        ImVec2 operator-(ImVec2 const& lhs, ImVec2 const& rhs);

        /*!
         * \brief Testing if point is within a circle.
         * \param point Position of the point.
         * \param center Center of the circle.
         * \param radius Radius of the circle.
         * \return bool indicating whether point is within the cicrle.
         */
        bool TestPointCircle(ImVec2 const& point, ImVec2 const& center, float radius);

        /*!
         * \brief Renders the max entity count and entities alive count.
         */
        void RenderEntityCount();

        /*!
         * \brief Renders the filter input text box with hint text.
         * \param filter The imgui text filter to render.
         * \param hitn The hint to render in the filter input text box.
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
         */
        void RenderControlVec2(std::string const& label, Vector2D& values, float x_reset = 0.f, float y_reset = 0.f, float column_width = 100.f);

        /*!
         * \brief Renders a combo box for an enumeration type using ImGui.
         *
         * This function renders a combo box for an enumeration type with provided label and options using ImGui.
         *
         * \tparam T The enumeration type.
         * \param label The label for the combo box.
         * \param enum_value The reference to the enumeration value to be controlled and displayed.
         * \param item_list The list of item names for the combo box.
         */
        template <typename T>
        void RenderComboBoxEnum(const char* label, T& enum_value, std::initializer_list<const char*> item_list) {
            std::vector<const char*> items = item_list;

            size_t current_item = static_cast<size_t>(enum_value);
            if (ImGui::BeginCombo(label, items[current_item])) {
                for (size_t i{}; i < items.size(); ++i) {
                    const bool is_selected = (current_item == i);
                    if (ImGui::Selectable(items[i], is_selected)) {
                        current_item = i;
                        enum_value = static_cast<T>(i);
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }

    } // end namespace EditorUtils

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_UTILS_EDITOR_UTILS_H
