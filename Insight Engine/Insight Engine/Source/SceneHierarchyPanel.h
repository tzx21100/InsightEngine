#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H

#include "Entities.h"

namespace IS {

    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;
        ~SceneHierarchyPanel() = default;

        void RenderPanel();
        void RenderEntityNode(Entity entity);
        void RenderComponentNodes(Entity entity);
    private:
        Entity entity_selected;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H
