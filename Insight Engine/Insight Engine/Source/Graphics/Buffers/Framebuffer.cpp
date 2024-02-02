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
#include "Engine/Core/CoreEngine.h"
#include "Engine/Systems/Window/WindowSystem.h"
#include "Debug/Utils/Assertion.h"
#include "Graphics/Core/Graphics.h"

namespace IS {

    int Framebuffer::ReadPixel(GLint x, GLint y)
    {
        Bind();

        int pixel_data;
        glReadBuffer(GL_COLOR_ATTACHMENT1);
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);
        pixel_data -= 1;

        Unbind();
        return pixel_data;
    }

    Framebuffer::Framebuffer(FramebufferProps const& properties) : mFramebufferID(0), mProps(properties) { Create(); }



    Framebuffer::~Framebuffer()
    {

        //glDeleteRenderbuffers(1, &mProps.mDepthAttachment);
    }

    void Framebuffer::Destroy()
    {
        glDeleteFramebuffers(1, &mFramebufferID);
        glDeleteTextures(1, &mProps.mColorAttachment);
        glDeleteTextures(1, &mProps.mEntIDAttachment);
    }

    void Framebuffer::Create()
    {
        // Create framebuffer object
        GL_CALL(glCreateFramebuffers(1, &mFramebufferID));
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID));

        // Create texture object color attachment
        GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &mProps.mColorAttachment));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, mProps.mColorAttachment));
        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mProps.mWidth, mProps.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mProps.mColorAttachment, 0));

        GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &mProps.mEntIDAttachment));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, mProps.mEntIDAttachment));
        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, mProps.mWidth, mProps.mHeight, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0));
        GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mProps.mEntIDAttachment, 0));

        /*glCreateTextures(GL_TEXTURE_2D, 1, &mProps.mDepthAttachment);
        glBindTexture(GL_TEXTURE_2D, mProps.mDepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mProps.mWidth, mProps.mHeight);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mProps.mDepthAttachment, 0);*/

        // Validate whether framebuffer object is complete
        // 
        IS_CORE_ASSERT_MESG((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Error: Framebuffer is incompelete!");

        GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Framebuffer is incomplete. Error code: " << framebufferStatus << std::endl;
            // Handle the specific error based on the error code.
        }

        unsigned drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, drawBuffers);

        // Unbind framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Bind()
    {
        if (glIsFramebuffer(mFramebufferID))
        {
            GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID));
        }
        else
            std::cout << "urmumgay" << std::endl;
        // Set viewport to size of framebuffer
        glViewport(0, 0, mProps.mWidth, mProps.mHeight);
    }

    void Framebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Set viewport to window size
        InsightEngine& engine = InsightEngine::Instance();
        auto const& window = engine.GetSystem<WindowSystem>("Window");
        auto [width, height] = window->GetWindowSize();

        glViewport(0, 0, width, height);
    }

    std::pair<GLuint, GLuint> Framebuffer::GetSize() const { return std::make_pair(mProps.mWidth, mProps.mHeight); }

    void Framebuffer::Resize(GLuint w, GLuint h)
    {
        mProps.mWidth = w;
        mProps.mHeight = h;

        // Delete current framebuffer
        if (mFramebufferID) {
            glDeleteFramebuffers(1, &mFramebufferID);
            glDeleteTextures(1, &mProps.mColorAttachment);
            glDeleteTextures(1, &mProps.mEntIDAttachment);
            //glDeleteRenderbuffers(1, &mProps.mDepthAttachment);
        }

        // Recreate framebuffer with new size
        Create();
    }

    GLuint Framebuffer::GetColorAttachment() const { return mProps.mColorAttachment; }
    GLuint Framebuffer::GetEntityIDAttachment() const { return mProps.mEntIDAttachment; }

    void Framebuffer::SetColorAttachment(GLuint color_attachment) { mProps.mColorAttachment = color_attachment; }

} // end namespace IS
