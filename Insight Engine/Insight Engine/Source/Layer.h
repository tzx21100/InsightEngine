#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_LAYERS_LAYER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_LAYERS_LAYER_H

#include <string>

namespace IS {

    class Layer {
    public:
        explicit Layer(std::string const& name) : debug_name(name) {}
        virtual ~Layer() = default;

        virtual void onAttach() = 0;
        virtual void onDetach() = 0;
        virtual void onUpdate([[maybe_unused]] float dt) = 0;
        virtual void onRender() = 0;
        inline std::string const& getName() const { return debug_name; }

    private:
        std::string debug_name;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_LAYERS_LAYER_H
