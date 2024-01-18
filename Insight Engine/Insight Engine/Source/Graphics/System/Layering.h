
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

        void AddEntity(Entity entity) {
            mLayerEntities.emplace_back(entity);
        }

        void RemoveEntity(Entity entity) {
            auto val = std::find(mLayerEntities.begin(), mLayerEntities.end(), entity);
            if (val == mLayerEntities.end()) {
                return;
            }
            mLayerEntities.erase(val);
        }

        bool mLayerActive=true;
        std::vector<Entity> mLayerEntities;
    };
}// end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LAYERING_H