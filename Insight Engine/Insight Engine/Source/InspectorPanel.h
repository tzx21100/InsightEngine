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
#include "Entities.h"

// Dependencies
#include <memory> // for std::shared_ptr
#include <imgui.h>

namespace IS {

    class Sprite;

    /*!
     * \brief The InspectorPanel class provides a panel for managing selected entity and its components.
     */
    class InspectorPanel : public Panel
    {
    public:

        /*!
         * \brief Represents the inspect mode.
         */
        enum class aInspectMode { INSPECT_NONE = -1, INSPECT_CAMERA, INSPECT_SCENE, INSPECT_ENTITY };

        /*!
         * \brief Constructs an InspectorPanel object.
         * 
         * \param hierarchy_panel The instance of hierarchy panel.
         */
        InspectorPanel(EditorLayer& editor_layer)
            : Panel("Inspector", editor_layer), mInspectMode(aInspectMode::INSPECT_NONE) {}

        void UpdatePanel() override {}

        /*!
         * \brief Renders the panel for the inspector.
         */
        void RenderPanel() override;

    private:
        aInspectMode mInspectMode; ///< Mode of inspection.
        bool mShowPrefabs = false; ///< Flag indicating show prefabs
        bool mShowAddAnimation = false; ///< Flag indicating show add animation

        /*!
         * \brief Renders component nodes for the specified Entity.
         * 
         * \param entity The Entity for which components will be rendered.
         */
        void RenderComponentNodes(Entity entity);

        /*!
         * \brief Renders configuration for the specified Entity.
         * 
         * \param entity The Entity for which configuration will be rendered.
         */
        void RenderEntityConfig(Entity entity);

        /*!
         * \brief Renders a specific component for the specified Entity.
         * 
         * \tparam Component The type of the component to render.
         * \tparam RenderFunc The type of the rendering function for the component.
         * 
         * \param label The label for the component.
         * \param entity The Entity for which the component will be rendered.
         * \param render The rendering function for the component.
         */
        template <typename Component, typename RenderFunc>
        void RenderComponent(std::string const& label, Entity entity, RenderFunc render);

        /*!
         * \brief Add animation to a sprite component.
         * 
         * \param sprite Sprite to add animation to.
         */
        void AddAnimation(Sprite& sprite);

        /*!
         * \brief Renders controls for camera.
         */
        void RenderCameraControls();

        friend class EditorLayer;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_INSPECTOR_PANEL_H
