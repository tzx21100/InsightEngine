/*!
 * \file Guidgets.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for ImGui widget RenderControlVec2
 * and template implementation for ImGui widget RenderComboBoxEnum
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_PANELS_GUIDGETS_H
#define GAM200_INSIGHT_ENGINE_EDITOR_PANELS_GUIDGETS_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "ISMath.h"

namespace IS {

    namespace guidgets {
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
        void RenderComboBoxEnum(std::string const& label, T& enum_value, std::initializer_list<std::string> item_list) {
            std::vector<std::string> items = item_list;

            size_t current_item = static_cast<size_t>(enum_value);
            if (ImGui::BeginCombo(label.c_str(), items[current_item].c_str())) {
                for (size_t i{}; i < items.size(); ++i) {
                    const bool is_selected = (current_item == i);
                    if (ImGui::Selectable(items[i].c_str(), is_selected)) {
                        current_item = i;
                        enum_value = static_cast<T>(i);
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }
    
    } // end namespace guidgets

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_GUIDGETS_H
