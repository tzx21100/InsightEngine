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
#ifdef USING_IMGUI

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_PANELS_PANEL_H
#define GAM200_INSIGHT_ENGINE_EDITOR_PANELS_PANEL_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Entities.h"

#include <string>
#include <IconsLucide.h> // for icon fonts
#include <ImGuizmo.h>

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
         * \param editor_layer Reference to the editor layer that owns the panel.
         */
        Panel(std::string const& name, EditorLayer& editor_layer) 
            : mName(name), mPosition(), mSize(), mFocused(false), mAppearing(false), mHovered(false), mEditorLayer(editor_layer) {}

        /*!
         * \brief Virtual destructor.
         */
        virtual ~Panel() = default;

        /*!
         * \brief Pure virtual function to update the panel.
         */
        virtual void UpdatePanel() = 0;

        /*!
         * \brief Pure virtual function to render the panel.
         */
        virtual void RenderPanel() = 0;

        /*!
         * \brief Gets the name of the panel.
         *
         * \return The name of the panel.
         */
        std::string GetName() const { return mName; }

        /*!
         * \brief Gets the position of the panel.
         * 
         * \return The position of the panel.
         */
        Vector2D GetPosition() const { return mPosition; }

        /*!
         * \brief Gets the size of the panel.
         *
         * \return The size of the panel.
         */
        Vector2D GetSize() const { return mSize; }

        /*!
         * \brief Checks if the panel is in focus.
         *
         * \return Boolean flag indicating if panel is in focus.
         */
        bool IsFocused() const { return mFocused; }

        /*!
         * \brief Checks if the panel is appearing.
         *
         * \return Boolean flag indicating if panel is appearing.
         */
        bool IsAppearing() const { return mAppearing; }

        /*!
         * \brief Checks if the panel is hovered.
         *
         * \return Boolean flag indicating if panel is hovered.
         */
        bool IsHovered() const { return mHovered; }

    protected:
        std::string mName; /// < Name of the panel.
        Vector2D mPosition; ///< Position of panel.
        Vector2D mSize; ///< Size of panel.
        bool mFocused; ///< Boolean flag indicating if panel is in focus.
        bool mAppearing; ///< Boolean flag indicating if panel is appearing.
        bool mHovered; ///< Boolean flag indicating if panel is hovered.
        EditorLayer& mEditorLayer; ///< Reference to owner.
    };

    /*!
     * \brief A derived class representing a game panel in a dockspace.
     */
    class GamePanel : public Panel
    {
    public:

        /*!
         * \brief Default constructor of game panel.
         */
        GamePanel(EditorLayer& editor_layer) : Panel("Game", editor_layer) {}

        /*!
         * \brief Overrides the base class method to update the game panel.
         */
        void UpdatePanel() override;

        /*!
         * \brief Overrides the base class method to render the game panel.
         */
        void RenderPanel() override;
    };

    /*!
     * \brief A derived class representing a performannce panel in a dockspace.
     */
    class ProfilerPanel : public Panel {
    public:

        /*!
         * \brief Default constructor of performance panel.
         */
        ProfilerPanel(EditorLayer& editor_layer) : Panel("Profiler", editor_layer) {}

        /*!
         * \brief Overrides the base class method to update the performance panel.
         */
        void UpdatePanel() override {}

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
        ConsolePanel(EditorLayer& editor_layer) : Panel("Console", editor_layer) {}

        /*!
         * \brief Overrides the base class method to update the console panel.
         */
        void UpdatePanel() override {}

        /*!
         * \brief Overrides the base class method to render the console panel.
         */
        void RenderPanel() override;
    };

    /*!
     * \brief A derived class representing a settings panel in a dockspace.
     */
    class SettingsPanel : public Panel {
    public:

        /*!
         * \brief Default constructor of settings panel.
         */
        SettingsPanel(EditorLayer& editor_layer) : Panel("Settings", editor_layer) {}

        /*!
         * \brief Overrides the base class method to update the settings panel.
         */
        void UpdatePanel() override {}

        /*!
         * \brief Overrides the base class method to render the settings panel.
         */
        void RenderPanel() override;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_PANEL_H

#endif // USING_IMGUI
