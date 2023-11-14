/*!
 * \file ScenePanel.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 12-11-2023
 * \brief
 * This header file declares the interface for class ScenePanel, which encapsulates
 * the functionalities of a scene viewport panel.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_PANELS_SCENE_PANEL_H
#define GAM200_INSIGHT_ENGINE_EDITOR_PANELS_SCENE_PANEL_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Panel.h"

#include <ImGuizmo.h>

namespace IS {

    /*!
     * \brief A derived class representing a scene panel in a dockspace.
     */
    class ScenePanel : public Panel
    {
    public:

        /*!
         * \brief Represents type of gizmo to render.
         */
        enum class aGizmoType
        {
            GIZMO_TYPE_INVALID = -1,
            GIZMO_TYPE_TRANSLATE = ImGuizmo::TRANSLATE,
            GIZMO_TYPE_ROTATE    = ImGuizmo::ROTATE_SCREEN,
            GIZMO_TYPE_SCALE     = ImGuizmo::SCALE
        };

        /*!
         * \brief Default constructor of scene panel.
         */
        ScenePanel(EditorLayer& editor_layer) 
            : Panel("Scene", editor_layer), mGizmoType(aGizmoType::GIZMO_TYPE_INVALID), mGizmoInUse(false), mSnap(false), mToolbarInUse(false) {}

        /*!
         * \brief Overrides the base class method to update the scene panel.
         */
        void UpdatePanel() override;

        /*!
         * \brief Overrides the base class method to render the scene panel.
         */
        void RenderPanel() override;

        /*!
         * \brief Get upper and lower bounds of the viewport.
         *
         * \return An array of two vec2 containing the upper and lower bounds of the viewport.
         */
        std::array<Vec2, 2> const& GetViewportBounds() const { return mViewportBounds; }

        /*!
         * \brief Checks if mouse is within viewport.
         * 
         * \return Boolean flag indicating if mouse is within viewport.
         */
        bool MouseWithinViewport() const;

    private:
        std::shared_ptr<Entity> mHoveredEntity; ///< Entity hovered.
        aGizmoType mGizmoType; ///< Type gizmo used.
        bool mGizmoInUse; ///< Boolean flag indicating if Gizmo is in use.
        bool mSnap; ///< Boolean flag indicating if gizmo snapping is enabled.
        bool mToolbarInUse; ///< Boolean flag indicating if toolbar is in use.
        std::array<Vec2, 2> mViewportBounds; ///< The upper and lower bounds of the scene panel.

        /*!
         * \brief Renders a help tooltip overlay.
         */
        void RenderHelp();

        /*!
         * \brief Renders a toolbar overlay.
         */
        void RenderToolbar();

        /*!
         * \brief Render the gizmo.
         */
        void RenderGizmo();

        /*!
         * \brief Zooms the camera.
         */
        void ZoomCamera();

        /*!
         * \brief Pans the camera.
         */
        void PanCamera();

        /*!
         * \brief Moves the camera with a specified move speed via key controls.
         *
         * \param move_speed The speed at which the camera should move.
         */
        void MoveCamera(float move_speed);

        /*!
         * \brief Rotates the camera.
         */
        void RotateCamera();

        friend class EditorLayer;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_SCENE_PANEL_H
