/*!
 * \file LayerStack.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for class LayerStack which
 * encapsulates the functionalities of a stack of layers.
 * 
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_LAYERSTACK_H
#define GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_LAYERSTACK_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Layer.h"
#include <vector>

namespace IS {

    /*!
     * \brief The LayerStack class manages layers and overlays in the application.
     * A layer stack maintains a list of layers and allows for easy addition, removal, and traversal of layers.
     */
    class LayerStack {
    public:
        using container = std::vector<std::shared_ptr<Layer>>; ///< The container type for storing layers.
        using value_type = container::value_type; ///< The type of elements stored in the container.
        using size_type = container::size_type; ///< The type for size-related operations.
        using reference = container::reference; ///< The type for referencing elements in the container.
        using const_reference = container::const_reference; ///< The type for const referencing elements in the container.
        using iterator = container::iterator; ///< Iterator type for iterating through the layers.
        using const_iterator = container::const_iterator; ///< Const iterator type for iterating through the layers.

        /*!
         * \brief Constructs a LayerStack object.
         */
        LayerStack() = default;

        /*!
         * \brief Destroys the LayerStack object.
         */
        ~LayerStack() = default;

        /*!
         * \brief Adds a layer to the top of the layer stack.
         * 
         * \param layer The layer to be added.
         */
        void PushLayer(value_type layer);

        /*!
         * \brief Adds an overlay to the top of the layer stack.
         * 
         * \param overlay The overlay to be added.
         */
        void PushOverlay(value_type overlay);

        /*!
         * \brief Removes a layer from the layer stack.
         * 
         * \param layer The layer to be removed.
         */
        void PopLayer(value_type layer);

        /*!
         * \brief Removes an overlay from the layer stack.
         * 
         * \param overlay The overlay to be removed.
         */
        void PopOverlay(value_type overlay);

        /*!
         * \brief Clears all layers and overlays from the layer stack.
         */
        void ClearStack();

        /*!
         * \brief Updates all layers and overlays in the layer stack.
         * 
         * \param delta_time The time difference since the last update.
         */
        void Update(float delta_time);

        /*!
         * \brief Renders all layers and overlays in the layer stack.
         */
        void Render();

        /*!
         * \brief Accesses a layer at the specified index.
         * 
         * \param index The index of the layer to access.
         * \return A reference to the layer at the specified index.
         */
        reference operator[](size_type index);

        /*!
         * \brief Accesses a layer at the specified index (const version).
         * 
         * \param index The index of the layer to access.
         * \return A const reference to the layer at the specified index.
         */
        const_reference operator[](size_type index) const;

        /*!
         * \brief Returns an iterator to the beginning of the layer stack.
         * 
         * \return An iterator to the beginning of the layer stack.
         */
        iterator begin();

        /*!
         * \brief Returns a const iterator to the beginning of the layer stack.
         * 
         * \return A const iterator to the beginning of the layer stack.
         */
        const_iterator begin() const;

        /*!
         * \brief Returns a const iterator to the beginning of the layer stack.
         * 
         * \return A const iterator to the beginning of the layer stack.
         */
        const_iterator cbegin() const;

        /*!
         * \brief Returns an iterator to the end of the layer stack.
         * 
         * \return An iterator to the end of the layer stack.
         */
        iterator end();

        /*!
         * \brief Returns a const iterator to the end of the layer stack.
         * 
         * \return A const iterator to the end of the layer stack.
         */
        const_iterator end() const;

        /*!
         * \brief Returns a const iterator to the end of the layer stack.
         * 
         * \return A const iterator to the end of the layer stack.
         */
        const_iterator cend() const;

    private:
        container mLayers; ///< The container storing layers.
        size_type mInsertIndex = 0; ///< The index for inserting layers.
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_LAYERS_LAYERSTACK_H
