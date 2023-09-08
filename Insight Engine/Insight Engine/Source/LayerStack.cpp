#include "Pch.h"
#include "LayerStack.h"

namespace IS {

    LayerStack::LayerStack() {
        //Subscribe(MessageType::Quit);
    }

    LayerStack::~LayerStack() {}

    void LayerStack::pushLayer(value_type layer) {
        layers.emplace(begin() + insert_index, layer);
        ++insert_index;
    }

    void LayerStack::pushOverlay(value_type overlay) {
        layers.emplace_back(overlay);
    }

    void LayerStack::popLayer(value_type layer) {
        if (iterator it = std::ranges::find(begin(), end(), layer); it != end()) {
            layers.erase(it);
            --insert_index;
        }
    }

    void LayerStack::popOverlay(value_type overlay) {
        if (iterator it = std::ranges::find(begin(), end(), overlay); it != end()) {
            layers.erase(it);
        }
    }

    void LayerStack::clearStack() {
        for (value_type layer : layers) {
            layer->onDetach();
            delete layer;
        }
    }

    void LayerStack::Update([[maybe_unused]] float delta_time) {
        for (reference layer : layers) {
            layer->onUpdate();
        }
    }

    void LayerStack::HandleMessage(Message const& message) {
        if (message.GetType() == MessageType::Quit) {
            clearStack();
        }
    }

    std::string LayerStack::getName() { return "LayerStack"; }

    LayerStack::reference LayerStack::operator[](size_type index) { return layers[index]; }
    LayerStack::const_reference LayerStack::operator[](size_type index) const { return layers[index]; }

    LayerStack::iterator LayerStack::begin() { return layers.begin(); }
    LayerStack::const_iterator LayerStack::begin() const { return layers.begin(); }
    LayerStack::const_iterator LayerStack::cbegin() const { return layers.cbegin(); }
    LayerStack::iterator LayerStack::end() { return layers.end(); }
    LayerStack::const_iterator LayerStack::end() const { return layers.end(); }
    LayerStack::const_iterator LayerStack::cend() const { return layers.cend(); }

} // end namespace IS