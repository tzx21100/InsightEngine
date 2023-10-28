/*!
 * \file Panel.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 17-10-2023
 * \brief
 * This header file declares the interface for parent class Panel and its derived classes,
 * which encapsulate the functionalities of a panel in a dockspace.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_PANELS_PANEL_H
#define GAM200_INSIGHT_ENGINE_EDITOR_PANELS_PANEL_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Entities.h"

#include <string>

namespace IS {

    /*!
     * \brief A base class representing a panel in a dockspace.
     */
    class Panel {
    public:
        /*!
         * \brief Default constructor.
         */
        Panel() = default;

        /*!
         * \brief Virtual destructor.
         */
        virtual ~Panel() = default;

        /*!
         * \brief Pure virtual function to render the panel.
         */
        virtual void RenderPanel() = 0;
    };

    class SceneHierarchyPanel; ///< Forward declaration

    class GamePanel : public Panel
    {
    public:
        GamePanel() = default;

        void RenderPanel() override;

        bool IsFocused() const;

        Vec2 GetViewportSize() const { return mViewportSize; }

    private:
        bool mFocused{};
        Vec2 mViewportSize{};
        Vec2 mViewportBounds[2];
    };

    /*!
     * \brief A derived class representing a scene panel in a dockspace.
     */
    class ScenePanel : public Panel {
    public:
        ScenePanel() = default;

        /*!
         * \brief Overrides the base class method to render the scene panel.
         */
        void RenderPanel() override;

        bool IsFocused() const;

        Vec2 GetViewportSize() const { return mViewportSize; }
        Vec2 mViewportBounds[2];

    private:
        bool mFocused{};
        Vec2 mViewportSize{}; ///< Size of the scene panel.

        /*!
         * \brief Renders a help tooltip overlay.
         */
        void RenderHelp();

        /*!
         * \brief Renders the camera zoom level.
         */
        void RenderCameraZoom();
    };

    /*!
     * \brief A derived class representing a performannce panel in a dockspace.
     */
    class PerformancePanel : public Panel {
    public:
        /*!
         * \brief Overrides the base class method to render the performance panel.
         */
        void RenderPanel() override;
    };

    /*!
     * \brief A derived class representing a log console panel in a dockspace.
     */
    class LogConsolePanel : public Panel {
    public:
        /*!
         * \brief Overrides the base class method to render the log console panel.
         */
        void RenderPanel() override;
    };

    /*!
     * \brief A derived class representing a physics control panel in a dockspace.
     */
    class PhysicsControlPanel : public Panel {
    public:
        /*!
         * \brief Overrides the base class method to render the physics control panel.
         */
        void RenderPanel() override;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_PANEL_H
