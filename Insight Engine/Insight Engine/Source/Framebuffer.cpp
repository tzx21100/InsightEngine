#include "Pch.h"
#include "Framebuffer.h"

namespace IS {

    Framebuffer::Framebuffer(FramebufferProps const& properties) : framebuffer_id(0), props(properties) {
        Create();
    }

    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1, &framebuffer_id);
        glDeleteTextures(1, &props.color_attachment);
        glDeleteRenderbuffers(1, &props.depth_attachment);
    }

    void Framebuffer::Create() {
        // Create framebuffer object
        glGenFramebuffers(1, &framebuffer_id);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

        // Create texture object color attachment
        glGenTextures(1, &props.color_attachment);
        glBindTexture(GL_TEXTURE_2D, props.color_attachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, props.width, props.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, props.color_attachment, 0);

        // Create texture object for depth attachment
        glCreateTextures(GL_TEXTURE_2D, 1, &props.depth_attachment);
        glBindTexture(GL_TEXTURE_2D, props.depth_attachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, props.width, props.height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, props.depth_attachment, 0);

        // Validate whether framebuffer object is complete
        IS_CORE_ASSERT_MESG((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Error: Framebuffer is incompelete!");

        // Unbind framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
        glViewport(0, 0, props.width, props.height);
    }

    void Framebuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WIDTH, HEIGHT);
    }

    std::pair<GLuint, GLuint> Framebuffer::GetSize() const {
        return std::make_pair(props.width, props.height);
    }

    void Framebuffer::Resize(GLuint w, GLuint h) {
        props.width = w;
        props.height = h;

        if (framebuffer_id) {
            glDeleteFramebuffers(1, &framebuffer_id);
            glDeleteTextures(1, &props.color_attachment);
            glDeleteRenderbuffers(1, &props.depth_attachment);
        }

        Create();
    }

    GLuint Framebuffer::GetColorAttachment() const {
        return props.color_attachment;
    }

    void Framebuffer::SetColorAttachment(GLuint color_attachment) {
        props.color_attachment = color_attachment;
    }

} // end namespace IS
