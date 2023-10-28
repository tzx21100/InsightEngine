/*!
 * \file InspectorPanel.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for class InspectorPanel,
 * which encapsulates the functionalities of an inspector panel which
 * manages an entity and its components
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_PANELS_INSPECTOR_PANEL_H
#define GAM200_INSIGHT_ENGINE_EDITOR_PANELS_INSPECTOR_PANEL_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Panel.h"
#include "SceneHierarchyPanel.h"
#include "Entities.h"

// Dependencies
#include <memory> // for std::shared_ptr
#include <imgui.h>

namespace IS {

    /*!
     * \brief The InspectorPanel class provides a panel for managing selected entity and its components.
     */
    class InspectorPanel : public Panel
    {
    public:

        /*!
         * \brief Constructs an InspectorPanel object.
         * \param scene_hierarchy_panel The instance of scene hierarchy panel.
         */
        InspectorPanel(std::shared_ptr<SceneHierarchyPanel> scene_hierarchy_panel);

        /*!
         * \brief Renders the panel for the inspector.
         */
        void RenderPanel() override;

    private:
        std::shared_ptr<SceneHierarchyPanel> mSceneHierarchyPanel; ///< Reference to instance of scene hierarchy panel.
        bool mShowPrefabs = false; ///< Flag indicating show prefabs

        /*!
         * \brief Renders component nodes for the specified Entity.
         * \param entity The Entity for which components will be rendered.
         */
        void RenderComponentNodes(Entity entity);

        /*!
         * \brief Renders configuration for the specified Entity.
         * \param entity The Entity for which configuration will be rendered.
         */
        void RenderEntityConfig(Entity entity);

        /*!
         * \brief Renders a specific component for the specified Entity.
         * \tparam Component The type of the component to render.
         * \tparam RenderFunc The type of the rendering function for the component.
         * \param label The label for the component.
         * \param entity The Entity for which the component will be rendered.
         * \param render The rendering function for the component.
         */
        template <typename Component, typename RenderFunc>
        void RenderComponent(std::string const& label, Entity entity, RenderFunc render);

        void RenderOutline(Entity entity);
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_INSPECTOR_PANEL_H
