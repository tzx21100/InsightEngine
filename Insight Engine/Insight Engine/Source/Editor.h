/*!
 * \file Editor.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for class Editor, which
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
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_SYSTEM_EDITOR_H
#define GAM200_INSIGHT_ENGINE_EDITOR_SYSTEM_EDITOR_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "System.h"
#include "Layer.h"
#include "LayerStack.h"
#include "EditorLayer.h"

namespace IS {

    /*!
     * \brief The Editor class manages GUI-related functionality.
     *
     * Editor is a subclass of ParentSystem and handles the GUI for the application.
     */
    class Editor : public ParentSystem {
    public:
        using layer_type = LayerStack::value_type; ///< Type alias for a layer.

        /*!
         * \brief Constructs a Editor object.
         */
        Editor();

        /*!
         * \brief Destroys the Editor object.
         */
        ~Editor() override;

        /*!
         * \brief Initializes the Editor.
         */
        void Initialize() override;

        /*!
         * \brief Updates the Editor.
         * 
         * \param delta_time The time difference since the last update.
         */
        void Update(float delta_time) override;

        /*!
         * \brief Terminates the Editor.
         */
        void Terminate();

        /*!
         * \brief Handles a message.
         * 
         * \param message The message to be handled.
         */
        void HandleMessage(Message const& message) override;

        /*!
         * \brief Gets the name of the Editor.
         * 
         * \return The name of the Editor.
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
         * 
         * \param layer The layer to be pushed.
         */
        void PushLayer(layer_type layer);

        /*!
         * \brief Pushes an overlay to the GUI layer stack.
         * 
         * \param overlay The overlay to be pushed.
         */
        void PushOverlay(layer_type overlay);

        /*!
         * \brief Pops a layer from the GUI layer stack.
         * 
         * \param layer The layer to be popped.
         */
        void PopLayer(layer_type layer);

        /*!
         * \brief Pops an overlay from the GUI layer stack.
         * 
         * \param overlay The overlay to be popped.
         */
        void PopOverlay(layer_type overlay);

        /*!
         * \brief Sets the colors for a dark theme.
         */
        void SetDarkThemeColors() const;

        /*!
         * \brief Checks if current context wants to capture mouse.
         * 
         * \return true if current context wants to capture mouse, else false.
         */
        bool WantCaptureMouse() const;

        /*!
         * \brief Checks if current context wants to capture keyboard.
         * 
         * \return true if current context wants to capture keyboard, else false.
         */
        bool WantCaptureKeyboard() const;

        /*!
         * \brief Get the instance of editor layer.
         * 
         * \return the instance of editor layer.
         */
        std::shared_ptr<EditorLayer> GetEditorLayer();

    private:
        std::shared_ptr<EditorLayer> mEditorLayer; ///< Instance of editor layer.
        LayerStack mLayers; ///< The stack of GUI layers
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_SYSTEM_EDITOR_H
