
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LAYERING_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LAYERING_H

#include "Sprite.h"

namespace IS {

    class Layering {
    public:
        // Unordered set insert comparator
        struct GfxLayerComparator {
            bool operator()(Sprite::instanceData const& a, Sprite::instanceData const& b) const {
                return a.layer < b.layer;
            }
        };


    };
}// end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LAYERING_H