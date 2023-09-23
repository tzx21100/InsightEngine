/*!
 * \file Framebuffer.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for class Framebuffer
 * which encapsulates the functionalities of a framebuffer in a graphics
 * pipeline.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Framebuffer.h"

namespace IS {

    Framebuffer::Framebuffer(FramebufferProps const& properties) : mFramebufferID(0), mProps(properties) {
        Create();
    }

    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1, &mFramebufferID);
        glDeleteTextures(1, &mProps.mColorAttachment);
        glDeleteRenderbuffers(1, &mProps.mDepthAttachment);
    }

    void Framebuffer::Create() {
        // Create framebuffer object
        glGenFramebuffers(1, &mFramebufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);

        // Create texture object color attachment
        glGenTextures(1, &mProps.mColorAttachment);
        glBindTexture(GL_TEXTURE_2D, mProps.mColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mProps.mWidth, mProps.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mProps.mColorAttachment, 0);

        // Create texture object for depth attachment
        glCreateTextures(GL_TEXTURE_2D, 1, &mProps.mDepthAttachment);
        glBindTexture(GL_TEXTURE_2D, mProps.mDepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mProps.mWidth, mProps.mHeight);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mProps.mDepthAttachment, 0);

        // Validate whether framebuffer object is complete
        IS_CORE_ASSERT_MESG((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Error: Framebuffer is incompelete!");

        // Unbind framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);

        // Set viewport to size of framebuffer
        glViewport(0, 0, mProps.mWidth, mProps.mHeight);
    }

    void Framebuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Set viewport to default size
        glViewport(0, 0, WIDTH, HEIGHT);
    }

    std::pair<GLuint, GLuint> Framebuffer::GetSize() const { return std::make_pair(mProps.mWidth, mProps.mHeight); }

    void Framebuffer::Resize(GLuint w, GLuint h) {
        mProps.mWidth = w;
        mProps.mHeight = h;

        // Delete current framebuffer
        if (mFramebufferID) {
            glDeleteFramebuffers(1, &mFramebufferID);
            glDeleteTextures(1, &mProps.mColorAttachment);
            glDeleteRenderbuffers(1, &mProps.mDepthAttachment);
        }

        // Recreate framebuffer with new size
        Create();
    }

    GLuint Framebuffer::GetColorAttachment() const { return mProps.mColorAttachment; }

    void Framebuffer::SetColorAttachment(GLuint color_attachment) { mProps.mColorAttachment = color_attachment; }

} // end namespace IS
