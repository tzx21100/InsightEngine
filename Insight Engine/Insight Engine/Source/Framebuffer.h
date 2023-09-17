#ifndef GAM200_INSIGHT_ENGINE_SOURCE_GRAPHICS_BUFFERS_FRAMEBUFFER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_GRAPHICS_BUFFERS_FRAMEBUFFER_H

#include <GL/glew.h> // for access to OpenGL API

namespace IS {

    /**
     * @brief The Framebuffer class represents an OpenGL framebuffer object.
     */
    class Framebuffer {
    public:
        struct FramebufferProps {
            GLuint color_attachment; /**< Color attachment ID. */
            GLuint depth_attachment; /**< Depth attachment ID. */
            GLuint width; /**< Width of the framebuffer. */
            GLuint height; /**< Height of the framebuffer. */
        };

        /**
         * @brief Constructs a Framebuffer object with the specified properties.
         * @param properties The properties for configuring the framebuffer.
         */
        Framebuffer(FramebufferProps const& properties);

        /**
         * @brief Destructor for cleaning up the framebuffer resources.
         */
        ~Framebuffer();

        /**
         * @brief Creates the framebuffer object.
         */
        void Create();

        /**
         * @brief Binds the framebuffer for rendering.
         */
        void Bind();

        /**
         * @brief Unbinds the currently bound framebuffer.
         */
        void Unbind();

        /**
         * @brief Gets the width and height of the framebuffer.
         * @return A pair containing the width and height of the framebuffer.
         */
        std::pair<GLuint, GLuint> GetSize() const;

        /**
         * @brief Resizes the framebuffer to the specified width and height.
         * @param w The new width of the framebuffer.
         * @param h The new height of the framebuffer.
         */
        void Resize(GLuint w, GLuint h);

        /**
         * @brief Gets the color attachment ID.
         * @return The color attachment ID.
         */
        GLuint GetColorAttachment() const;

        /**
         * @brief Sets the color attachment ID.
         * @param color_attachment The color attachment ID to set.
         */
        void SetColorAttachment(GLuint color_attachment);

    private:
        GLuint framebuffer_id;  /**< ID of the framebuffer. */
        FramebufferProps props; /**< Properties of the framebuffer. */
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_GRAPHICS_BUFFERS_FRAMEBUFFER_H
