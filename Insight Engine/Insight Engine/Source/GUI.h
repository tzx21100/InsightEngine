/*!
 * \file GUI.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for class GUISystem, which
 * encapsulates the functionalities of a graphical user interface (GUI)
 * using the Dear ImGui libary.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_SYSTEM_GUI_H
#define GAM200_INSIGHT_ENGINE_EDITOR_SYSTEM_GUI_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "System.h"
#include "Layer.h"
#include "LayerStack.h"

namespace IS {

    /*!
     * \brief The GUISystem class manages GUI-related functionality.
     *
     * GUISystem is a subclass of ParentSystem and handles the GUI for the application.
     */
    class GUISystem : public ParentSystem {
    public:
        using layer_type = LayerStack::value_type; ///< Type alias for a layer.

        /*!
         * \brief Constructs a GUISystem object.
         */
        GUISystem();

        /*!
         * \brief Destroys the GUISystem object.
         */
        ~GUISystem() override;

        /*!
         * \brief Initializes the GUISystem.
         */
        void Initialize() override;

        /*!
         * \brief Updates the GUISystem.
         * \param delta_time The time difference since the last update.
         */
        void Update(float delta_time) override;

        /*!
         * \brief Terminates the GUISystem.
         */
        void Terminate();

        /*!
         * \brief Handles a message.
         * \param message The message to be handled.
         */
        void HandleMessage(Message const& message) override;

        /*!
         * \brief Gets the name of the GUISystem.
         * \return The name of the GUISystem.
         */
        std::string GetName() override;

        /*!
         * \brief Begins GUI functionality.
         *
         * This function is intended to be called once at the beginning of GUI functionality.
         */
        void Begin();

        /*!
         * \brief Ends GUI functionality.
         *
         * This function is intended to be called once at the end of GUI functionality.
         */
        void End();

        /*!
         * \brief Pushes a layer to the GUI layer stack.
         * \param layer The layer to be pushed.
         */
        void PushLayer(layer_type layer);

        /*!
         * \brief Pushes an overlay to the GUI layer stack.
         * \param overlay The overlay to be pushed.
         */
        void PushOverlay(layer_type overlay);

        /*!
         * \brief Pops a layer from the GUI layer stack.
         * \param layer The layer to be popped.
         */
        void PopLayer(layer_type layer);

        /*!
         * \brief Pops an overlay from the GUI layer stack.
         * \param overlay The overlay to be popped.
         */
        void PopOverlay(layer_type overlay);

        /*!
         * \brief Sets the colors for a dark theme.
         */
        void SetDarkThemeColors() const;

        /*!
         * \brief Checks if current context wants to capture mouse.
         * \return true if current context wants to capture mouse, else false.
         */
        bool WantCaptureMouse() const;

        /*!
         * \brief Checks if current context wants to capture keyboard.
         * \return true if current context wants to capture keyboard, else false.
         */
        bool WantCaptureKeyboard() const;

    private:
        LayerStack mLayers; ///< The stack of GUI layers.
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_SYSTEM_GUI_H
