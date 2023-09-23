#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ENGINE_WINDOW_WINDOW_SYSTEM_H
#define GAM200_INSIGHT_ENGINE_SOURCE_ENGINE_WINDOW_WINDOW_SYSTEM_H

// Ensure glew is included before gl
#include <GLFW/glfw3.h>

#include "System.h"
#include "Graphics.h"

namespace IS {

    class WindowSystem : public ParentSystem {
    public:
        // struct encapsulating properties of a window
        struct WindowProperties {
            std::string title;
            uint32_t width{}, height{};
            bool vsync{};

            static WindowProperties default_properties;
        };

        // Constructor
        WindowSystem();
        ~WindowSystem();

        // override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        void HandleMessage(const Message& message) override;
        std::string getName() override;

        void BeginUpdate();
        void EndUpdate();

        // Property matters
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
        std::string GetWindowTitle() const;
        void SetWindowSize(uint32_t width, uint32_t height);
        void SetWindowTitle(std::string const& title);
        void EnableVsync(bool enabled);
        bool IsVSync() const;
        void LoadProperties();
        void SaveProperties();

    private:
        GLFWwindow* window = nullptr;
        WindowProperties props;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_ENGINE_WINDOW_WINDOW_SYSTEM_H
