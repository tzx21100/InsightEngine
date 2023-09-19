#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_GUIDGETS_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_GUIDGETS_H

#include "ISMath.h"

namespace IS {

    namespace guidgets {

        void RenderControlVec2(std::string const& label, Vector2D& values, float x_reset = 0.f, float y_reset = 0.f, float column_width = 100.f);

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

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_GUIDGETS_H
