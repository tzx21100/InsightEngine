#ifndef GAM200_INSIGHT_ENGINE_SOURCE_GRAPHICS_BUFFERS_FRAMEBUFFER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_GRAPHICS_BUFFERS_FRAMEBUFFER_H

#include <GL/glew.h> 

namespace IS {

    class Framebuffer {
    public:
        Framebuffer();
        ~Framebuffer();

        void Bind();
        void Unbind();

        GLuint getFramebuffer() const { return fbo_id; }
        GLuint getTexture() const { return tex_id; }
    private:
        GLuint fbo_id; // framebuffer handle
        GLuint tex_id; // color attachment
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_GRAPHICS_BUFFERS_FRAMEBUFFER_H
