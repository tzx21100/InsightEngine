#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H

#include "Entities.h"
#include <memory>

namespace IS {

    class SceneHierarchyPanel {
    public:
        using EntityPtr = std::shared_ptr<Entity>;

        SceneHierarchyPanel() = default;
        ~SceneHierarchyPanel() = default;

        void RenderPanel();
        void RenderEntityNode(Entity it);
        void RenderComponentNodes(EntityPtr entity);

        template <typename Component, typename RenderFunc>
        static void RenderComponent(std::string const& label, Entity entity, RenderFunc render);
    private:
        EntityPtr selected_entity;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H
