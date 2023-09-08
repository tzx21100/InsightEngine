#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_LAYERSTACK_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_LAYERSTACK_H

#include "Layer.h"
#include "System.h"
#include <vector>

namespace IS {

    class LayerStack : ParentSystem {
    public:
        using container = std::vector<Layer*>;
        using value_type = container::value_type;
        using size_type = container::size_type;
        using reference = container::reference;
        using const_reference = container::const_reference;
        using iterator = container::iterator;
        using const_iterator = container::const_iterator;

        LayerStack();
        ~LayerStack();

        void pushLayer(value_type layer);
        void pushOverlay(value_type overlay);
        void popLayer(value_type layer);
        void popOverlay(value_type overlay);
        void clearStack();

        void Update(float delta_time) override;
        void HandleMessage(Message const& message) override;
        std::string getName() override;

        reference operator[](size_type index);
        const_reference operator[](size_type index) const;

        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;

    private:
        container layers;
        size_type insert_index = 0;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_LAYERSTACK_H
