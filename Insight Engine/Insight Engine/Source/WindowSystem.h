#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ENGINE_WINDOW_WINDOW_SYSTEM_H
#define GAM200_INSIGHT_ENGINE_SOURCE_ENGINE_WINDOW_WINDOW_SYSTEM_H

// Ensure glew is included before gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "System.h"
#include "Graphics.h"

namespace IS {

    class WindowSystem : public ParentSystem {
    public:
        // struct encapsulating properties of a window
        struct WindowProperties {
            uint32_t width{}, height{};
            std::string title;
            bool VSync{};
        };

        // Constructor
        WindowSystem(uint32_t width, uint32_t height, std::string const& title, bool vsync = true);
        ~WindowSystem();

        // override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        void HandleMessage(const Message& message) override;
        std::string getName() override;

        // Property matters
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
        std::string GetWindowTitle() const;
        void SetWindowSize(uint32_t width, uint32_t height);
        void SetWindowTitle(std::string const& title);
        void SetVSync(bool enabled);
        bool IsVSync() const;

    private:
        GLFWwindow* window = nullptr;
        WindowProperties props;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_ENGINE_WINDOW_WINDOW_SYSTEM_H
