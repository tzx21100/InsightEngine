#include "Pch.h"
#include "Framebuffer.h"

namespace IS {

    Framebuffer::Framebuffer(FramebufferProps const& properties) : framebuffer_id(0), props(properties) {
        GLuint fbo_hdl, tex_hdl, rbo_hdl;

        // Create framebuffer object
        glGenFramebuffers(1, &fbo_hdl);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_hdl);

        // Create texture object (color attachment)
        glGenTextures(1, &tex_hdl);
        glBindTexture(GL_TEXTURE_2D, tex_hdl);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, props.width, props.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        // Allow rescaling
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_hdl, 0);

        // Create renderbuffer object (depth attachment)
        glGenRenderbuffers(1, &rbo_hdl);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_hdl);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, props.width, props.height);
        glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_hdl);

        // Validate whether framebuffer object is complete
        GLenum fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fbo_status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer is incomplete!\n";
            std::exit(EXIT_FAILURE);
        }

        // Unbind framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        framebuffer_id         = fbo_hdl;
        props.color_attachment = tex_hdl;
        props.depth_attachment = rbo_hdl;
    }

    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1, &framebuffer_id);
        glDeleteTextures(1, &props.color_attachment);
        glDeleteRenderbuffers(1, &props.depth_attachment);
    }

    void Framebuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
        glViewport(0, 0, props.width, props.height);
    }

    void Framebuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    std::pair<GLuint, GLuint> Framebuffer::GetSize() const {
        return std::make_pair(props.width, props.height);
    }

    void Framebuffer::Resize(GLuint w, GLuint h) {
        props.width = w;
        props.height = h;
    }

    GLuint Framebuffer::GetColorAttachment() const {
        return props.color_attachment;
    }

    void Framebuffer::SetColorAttachment(GLuint color_attachment) {
        props.color_attachment = color_attachment;
    }

} // end namespace IS
