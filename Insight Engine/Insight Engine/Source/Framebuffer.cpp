#include "Pch.h"
#include "Framebuffer.h"

namespace IS {

    Framebuffer::Framebuffer() : fbo_id(), tex_id() {
        GLuint fbo_hdl, tex_hdl, rbo_hdl;

        // Create framebuffer object
        glGenFramebuffers(1, &fbo_hdl);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_hdl);

        // Create texture object (color attachment)
        glGenTextures(1, &tex_hdl);
        glBindTexture(GL_TEXTURE_2D, tex_hdl);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        // Allow rescaling
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_hdl, 0);

        // Create renderbuffer object (depth attachment)
        glGenRenderbuffers(1, &rbo_hdl);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_hdl);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
        glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_hdl);

        // Validate whether framebuffer object is complete
        GLenum fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fbo_status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer is incomplete!\n";
            std::exit(EXIT_FAILURE);
        }

        fbo_id = fbo_hdl;
        tex_id = tex_hdl;
    }

    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1, &fbo_id);
        glDeleteTextures(1, &tex_id);
    }

    void Framebuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
    }

    void Framebuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

} // end namespace IS
