/*!
 * \file EditorUtils.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for editor utility functions
 * such as custom widgets.
 * 
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
#ifdef USING_IMGUI

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_UTILS_EDITOR_UTILS_H
#define GAM200_INSIGHT_ENGINE_EDITOR_UTILS_EDITOR_UTILS_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Math/ISMath.h"
#include "Editor/Commands/CommandHistory.h"

#include <functional>
#include <glad/glad.h> // for GLuint

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
    namespace EditorUtils
    {
        struct WidgetState
        {
            bool mIsModified = false;
            bool mIsActive = false;
            bool mIsDeactivated = false;
            bool mIsDeactivatedAfterEdit = false;
        };

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
         * \return WidgetState indicating the state of the widget.
         */
        WidgetState RenderControlVec2(std::string const& label, Vector2D& values, float x_reset = 0.f, float y_reset = 0.f, float column_width = 100.f);

        /*!
         * \brief Renders a control for a 3D vector using ImGui.
         *
         * This function renders a control for a 3D vector with provided label and values using ImGui.
         * The control allows resetting the x, y, and z values to specified values.
         *
         * \param label The label for the control.
         * \param values The reference to the Vector3D to be controlled and displayed.
         * \param x_reset The value to reset the x-component to (default is 0.0f).
         * \param y_reset The value to reset the y-component to (default is 0.0f).
         * \param z_reset The value to reset the z-component to (default is 0.0f).
         * \param column_width The width of the control column (default is 100.0f).
         * \return WidgetState indicating the state of the widget.
         */
        WidgetState RenderControlVec3(std::string const& label, Vector3D& values, float x_reset = 0.f, float y_reset = 0.f, float z_reset = 0.f, float column_width = 100.f);

        /*!
         * \brief Renders a table label with optional tooltip.
         *
         * \param label The label to render.
         * \param tooltip Optional tooltip to display when hovering over the label.
         */
        void RenderTableLabel(std::string const& label, std::string const& tooltip = std::string());

        /*!
         * \brief Renders a bold table label with optional tooltip.
         *
         * \param label The label to render.
         * \param tooltip Optional tooltip to display when hovering over the label.
         */
        void RenderTableBoldLabel(std::string const& label, std::string const& tooltip = std::string());

        /*!
         * \brief Renders a table with fixed width columns.
         *
         * \param label The label for the table.
         * \param columns The number of columns in the table.
         * \param contents Function to render the contents of the table.
         * \param flags ImGuiTableFlags for the table.
         * \param label_width The fixed width for the columns.
         */
        void RenderTableFixedWidth(std::string const& label, int columns, std::function<void(void)> contents, ImGuiTableFlags flags = 0, float label_width = 100.f);

        /*!
         * \brief Renders a table.
         *
         * \param label The label for the table.
         * \param columns The number of columns in the table.
         * \param contents Function to render the contents of the table.
         * \param flags ImGuiTableFlags for the table.
         */
        void RenderTable(std::string const& label, int columns, std::function<void(void)> contents, ImGuiTableFlags flags = 0);

        /*!
         * \brief Renders a table input text field.
         *
         * \param text The text to display and edit.
         * \param action Optional action to perform when the text is edited.
         */
        void RenderTableInputText(std::string& text, std::function<void(void)> action = nullptr);

        /*!
         * \brief Renders an input text field.
         *
         * \param text The text to display and edit.
         * \param action Optional action to perform when the text is edited.
         */
        void RenderInputText(std::string& text, std::function<void(void)> action = nullptr);

        /*!
         * \brief Renders a toggle button.
         *
         * \param str_id The string identifier for the button.
         * \param value The current value of the toggle, which will be changed when the button is clicked.
         */
        void RenderToggleButton(std::string const& str_id, bool& value);

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
                        CommandHistory::AddCommand<ChangeCommand<EnumType>>(enum_value, static_cast<EnumType>(i));
                        CommandHistory::SetNoMergeMostRecent();
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

#endif // USING_IMGUI
