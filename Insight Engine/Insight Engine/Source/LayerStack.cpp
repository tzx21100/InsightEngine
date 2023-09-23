/*!
 * \file LayerStack.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 *      This source file defines the implementation for class LayerStack which
 *      encapsulates the functionalities of a stack of layers.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "LayerStack.h"

namespace IS {

    void LayerStack::PushLayer(value_type layer) {
        mLayers.emplace(begin() + mInsertIndex, layer);
        ++mInsertIndex;
    }

    void LayerStack::PushOverlay(value_type overlay) {
        mLayers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(value_type layer) {
        if (iterator it = std::ranges::find(begin(), end(), layer); it != end()) {
            mLayers.erase(it);
            --mInsertIndex;
        }
    }

    void LayerStack::PopOverlay(value_type overlay) {
        if (iterator it = std::ranges::find(begin(), end(), overlay); it != end()) {
            mLayers.erase(it);
        }
    }

    void LayerStack::ClearStack() {
        for (value_type layer : mLayers) {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::Update([[maybe_unused]] float delta_time) {
        for (reference layer : mLayers) {
            layer->OnUpdate(delta_time);
        }
    }

    void LayerStack::Render() {
        for (reference layer : mLayers) {
            layer->OnRender();
        }
    }

    LayerStack::reference LayerStack::operator[](size_type index) { return mLayers[index]; }
    LayerStack::const_reference LayerStack::operator[](size_type index) const { return mLayers[index]; }

    LayerStack::iterator LayerStack::begin() { return mLayers.begin(); }
    LayerStack::const_iterator LayerStack::begin() const { return mLayers.begin(); }
    LayerStack::const_iterator LayerStack::cbegin() const { return mLayers.cbegin(); }
    LayerStack::iterator LayerStack::end() { return mLayers.end(); }
    LayerStack::const_iterator LayerStack::end() const { return mLayers.end(); }
    LayerStack::const_iterator LayerStack::cend() const { return mLayers.cend(); }

} // end namespace IS