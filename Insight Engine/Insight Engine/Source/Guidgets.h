#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_GUIDGETS_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_GUIDGETS_H

#include "ISMath.h"

namespace IS {

    namespace guidgets {

        void RenderControlVec2(std::string const& label, Vector2D& values, float x_reset = 0.f, float y_reset = 0.f, float column_width = 100.f);
        void RenderComboBoxBodyType(std::string const& label, BodyType& bodytype);
        void RenderComboBoxBodyShape(std::string const& label, Shape& bodyshape);
    
    } // end namespace guidgets

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_GUIDGETS_H
