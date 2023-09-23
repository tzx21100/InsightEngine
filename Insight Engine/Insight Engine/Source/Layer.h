/*!
 * \file Layer.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for pure virtual class Layer which
 * encapsulates the functionalities of a layer.
 *
 * A Layer provides a modular way to extend application functionality and behavior.
 * Layers can be attached or detached dynamically and handle updates and rendering.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_LAYER_H
#define GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_LAYER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <string>

namespace IS {
    /*!
     * \brief The base class for layers in the application.
     *
     * \brief Layers allow modular functionality to be added to the application in a structured way.
     * \brief Inherit from this class to create a custom layer with specific behavior.
     */
    class Layer {
    public:
        /*!
         * \brief Constructs a Layer with a given name.
         * \param name The name of the layer.
         */
        explicit Layer(std::string const& name) : mDebugName(name) {}
        /*!
         * \brief Virtual destructor for the Layer class.
         */
        virtual ~Layer() = default;

        /*!
         * \brief Called when the layer is attached to the application.
         * Derived classes should implement this method to perform initialization upon attachment.
         */
        virtual void OnAttach() = 0;
        /*!
         * \brief Called when the layer is detached from the application.
         * Derived classes should implement this method to perform cleanup upon detachment.
         */
        virtual void OnDetach() = 0;
        /*!
         * \brief Called when the layer needs to update its state.
         * \param dt The time in seconds since the last update.
         * Derived classes should implement this method to handle update logic.
         */
        virtual void OnUpdate([[maybe_unused]] float dt) = 0;
        /*!
         * \brief Called when the layer needs to render its content.
         * Derived classes should implement this method to handle rendering logic.
         */
        virtual void OnRender() = 0;
        /*!
         * \brief Gets the name of the layer.
         * \return The name of the layer.
         */
        inline std::string const& GetName() const { return mDebugName; }

    private:
        std::string mDebugName; ///< The debug name of the layer.
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_LAYER_H
