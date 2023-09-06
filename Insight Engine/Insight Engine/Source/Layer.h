#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_LAYER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_LAYER_H

#include <string>

namespace IS {

    class Layer {
    public:
        explicit Layer(std::string const& name) : debug_name(name) {}
        virtual ~Layer() = default;

        virtual void onAttach() const {}
        virtual void onDetach() const {}
        virtual void onUpdate() const {}
        virtual void onRender() const {}
        inline std::string const& getName() const { return debug_name; }

    private:
        std::string debug_name;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_EDITOR_LAYER_H
