#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H

#include "Entities.h"
#include <memory>

namespace IS {

    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;
        ~SceneHierarchyPanel() = default;

        void RenderPanel();
        void RenderEntityNode(Entity entity);
        void RenderComponentNodes(std::shared_ptr<Entity> entity);
    private:
        std::shared_ptr<Entity> selected_entity;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H
