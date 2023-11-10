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
#include <IconsLucide.h> // for icon fonts

namespace IS {

    class EditorLayer; ///< Forward declare class EditorLayer.

    /*!
     * \brief A base class representing a panel in a dockspace.
     */
    class Panel {
    public:

        /*!
         * \brief Default constructor.
         * 
         * \param name Name of the panel.
         */
        Panel(std::string const& name, EditorLayer& editor_layer) : mName(name), mEditorLayer(editor_layer) {}

        /*!
         * \brief Virtual destructor.
         */
        virtual ~Panel() = default;

        /*!
         * \brief Pure virtual function to render the panel.
         */
        virtual void RenderPanel() = 0;

    protected:
        std::string mName; ///< Name of the panel.
        EditorLayer& mEditorLayer; ///< Reference to owner.
    };

    class HierarchyPanel; ///< Forward declaration

    /*!
     * \brief A derived class representing a game panel in a dockspace.
     */
    class GamePanel : public Panel
    {
    public:

        /*!
         * \brief Default constructor of game panel.
         */
        GamePanel(EditorLayer& editor_layer) : Panel(ICON_LC_GAMEPAD_2 "  Game", editor_layer) {}

        /*!
         * \brief Overrides the base class method to render the game panel.
         */
        void RenderPanel() override;

        /*!
         * \brief Check if the game panel is in focus.
         * 
         * \return boolean flag indicating if game panel is in focus.
         */
        bool IsFocused() const;

    private:
        bool mFocused{}; ///< Boolean flag indicating if the game panel is in focus.
        Vec2 mViewportSize{}; ///< The size of the game panel.
    };

    /*!
     * \brief A derived class representing a scene panel in a dockspace.
     */
    class ScenePanel : public Panel {
    public:

        /*!
         * \brief Default constructor of scene panel.
         */
        ScenePanel(EditorLayer& editor_layer) : Panel(ICON_LC_VIEW "  Scene", editor_layer) {}

        /*!
         * \brief Overrides the base class method to render the scene panel.
         */
        void RenderPanel() override;

        /*!
         * \brief Check if the scene panel is in focus.
         * 
         * \return boolean flag indicating if scene panel is in focus.
         */
        bool IsFocused() const;

        /*!
         * \brief Get the size of the viewport.
         * 
         * \return vec2 containing the width and height of the viewport.
         */
        Vec2 GetViewportSize() const { return mViewportSize; }

        /*!
         * \brief Get upper and lower bounds of the viewport.
         * 
         * \return an array of 2 vec2 containing the upper and lower bounds of the viewport.
         */
        std::array<Vec2, 2> const& GetViewportBounds() const { return mViewportBounds; }

        bool MouseWithinViewport() const;

    private:
        bool mFocused{}; ///< Boolean flag indicating if scene panel is in focus.
        Vec2 mViewportSize{}; ///< Size of the scene panel.
        Vec2 mViewportPos{};
        std::array<Vec2, 2> mViewportBounds; ///< The upper and lower bounds of the scene panel.

        /*!
         * \brief Renders a help tooltip overlay.
         */
        void RenderHelp();
    };

    /*!
     * \brief A derived class representing a performannce panel in a dockspace.
     */
    class PerformancePanel : public Panel {
    public:

        /*!
         * \brief Default constructor of performance panel.
         */
        PerformancePanel(EditorLayer& editor_layer) : Panel(ICON_LC_GAUGE "  Performance", editor_layer) {}

        /*!
         * \brief Overrides the base class method to render the performance panel.
         */
        void RenderPanel() override;
    };

    /*!
     * \brief A derived class representing a log console panel in a dockspace.
     */
    class ConsolePanel : public Panel {
    public:

        /*!
         * \brief Default constructor of console panel.
         */
        ConsolePanel(EditorLayer& editor_layer) : Panel(ICON_LC_TERMINAL_SQUARE "  Console", editor_layer) {}

        /*!
         * \brief Overrides the base class method to render the log console panel.
         */
        void RenderPanel() override;

        /*!
         * \brief Gets the size of the log console panel.
         * 
         * \return The size of the log console panel.
         */
        Vec2 GetPanelSize() const;

    private:
        Vec2 mPanelSize; ///< Size of the panel.
    };

    /*!
     * \brief A derived class representing a settings panel in a dockspace.
     */
    class SettingsPanel : public Panel {
    public:

        /*!
         * \brief Default constructor of settings panel.
         */
        SettingsPanel(EditorLayer& editor_layer) : Panel(ICON_LC_SETTINGS "  Settings", editor_layer) {}

        /*!
         * \brief Overrides the base class method to render the settings panel.
         */
        void RenderPanel() override;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_PANEL_H
