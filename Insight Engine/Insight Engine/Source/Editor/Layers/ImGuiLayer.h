/*!
 * \file ImGuiLayer.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for class ImGuiLayer, which
 * encapsulates the functionalities of a graphical user interface (GUI)
 * using the Dear ImGui libary.
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
#ifndef GAM200_INSIGHT_ENGINE_IMGUI_LAYER_EDITOR_H
#define GAM200_INSIGHT_ENGINE_IMGUI_LAYER_EDITOR_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Engine/Core/Core.h"
#include "Engine/ECS/System.h"
#include "Layer.h"
#include "LayerStack.h"

namespace IS {

    /*!
     * \brief The Editor class manages GUI-related functionality.
     *
     * Editor is a subclass of ParentSystem and handles the GUI for the application.
     */
    class IS_API ImGuiLayer : public Layer {
    public:

        /*!
         * \brief Constructs a Editor object.
         */
        ImGuiLayer() : Layer("ImGui Layer") {}

        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate(float) override {}
        void OnRender() override {}

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
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_SYSTEM_EDITOR_H


#endif // USING_IMGUI
