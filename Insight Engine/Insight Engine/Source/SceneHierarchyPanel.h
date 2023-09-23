/*!
 * \file SceneHierarchyPanel.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 *      This header file declares the interface for class SceneHierarchyPanel,
 *      which encapsulates the functionalities of a scene hierarchy panel
 *      akin to other game engines (i.e., Unity/Unreal Engine, etc.), which
 *      manages the entities in a scene.
 * 
 * \copyright Copyright (C) 2023, Digipen Institute of Technology
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <memory> // for std::shared_ptr
#include "Entities.h"

namespace IS {

    /*!
     * \brief The SceneHierarchyPanel class provides a panel for managing scene entities and components.
     */
    class SceneHierarchyPanel {
    public:
        using EntityPtr = std::shared_ptr<Entity>; ///< Type alias for a shared pointer to an Entity.

        /*!
         * \brief Constructs a SceneHierarchyPanel object.
         */
        SceneHierarchyPanel() = default;

        /*!
         * \brief Destroys the SceneHierarchyPanel object.
         */
        ~SceneHierarchyPanel() = default;

        /*!
         * \brief Renders the panel for the scene hierarchy.
         */
        void RenderPanel();

        /*!
         * \brief Renders a node for the specified Entity in the scene hierarchy.
         * \param it The Entity to be rendered.
         */
        void RenderEntityNode(Entity it);

        /*!
         * \brief Renders component nodes for the specified Entity.
         * \param entity The Entity for which components will be rendered.
         */
        void RenderComponentNodes(EntityPtr entity);

        /*!
         * \brief Renders a specific component for the specified Entity.
         * \tparam Component The type of the component to render.
         * \tparam RenderFunc The type of the rendering function for the component.
         * \param label The label for the component.
         * \param entity The Entity for which the component will be rendered.
         * \param render The rendering function for the component.
         */
        template <typename Component, typename RenderFunc>
        static void RenderComponent(std::string const& label, Entity entity, RenderFunc render);

    private:
        EntityPtr mSelectedEntity; ///< The selected Entity in the hierarchy.
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H
