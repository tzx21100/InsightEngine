/*!
 * \file Framebuffer.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for class Framebuffer which
 * encapsulates the functionalities of a framebuffer in a graphics
 * pipeline.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_BUFFERS_FRAMEBUFFER_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_BUFFERS_FRAMEBUFFER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <glad/glad.h> // for access to OpenGL API

namespace IS {

    /*!
     * \brief The Framebuffer class represents an OpenGL framebuffer object.
     */
    class Framebuffer {
    public:
        struct FramebufferProps {
            GLuint mColorAttachment{}; /**< Color attachment ID. */
            GLuint mEntIDAttachment{};
            //GLuint mDepthAttachment; /**< Depth attachment ID. */
            GLuint mWidth{}; /**< Width of the framebuffer. */
            GLuint mHeight{}; /**< Height of the framebuffer. */
        };

        Framebuffer() = default;

        /*!
         * \brief Constructs a Framebuffer object with the specified properties.
         * 
         * \param properties The properties for configuring the framebuffer.
         */
        Framebuffer(FramebufferProps const& properties);

        /*!
         * \brief Destructor for cleaning up the framebuffer resources.
         */
        ~Framebuffer();

        /*!
         * \brief Creates the framebuffer object.
         */
        void Create();

        /*!
         * \brief Binds the framebuffer for rendering.
         */
        void Bind();

        /*!
         * \brief Unbinds the currently bound framebuffer.
         */
        void Unbind();

        /*!
         * \brief Reads the pixel at given location.
         * 
         * \param x The x coordinate of the pixel.
         * \param y The y coordinate of the pixel.
         * \return The integer value at the given location.
         */
        int ReadPixel(GLint x, GLint y);

        /*!
         * \brief Gets the width and height of the framebuffer.
         * 
         * \return A pair containing the width and height of the framebuffer.
         */
        std::pair<GLuint, GLuint> GetSize() const;

        /*!
         * \brief Resizes the framebuffer to the specified width and height.
         * \param w The new width of the framebuffer.
         * \param h The new height of the framebuffer.
         */
        void Resize(GLuint w, GLuint h);

        /*!
         * \brief Gets the color attachment ID.
         * 
         * \return The color attachment ID.
         */
        GLuint GetColorAttachment() const;
        GLuint GetEntityIDAttachment() const;

        /*!
         * \brief Sets the color attachment ID.
         * 
         * \param color_attachment The color attachment ID to set.
         */
        void SetColorAttachment(GLuint color_attachment);        

    private:
        GLuint mFramebufferID;  ///< ID of the framebuffer
        FramebufferProps mProps; ///< Properties of the framebuffer.
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_BUFFERS_FRAMEBUFFER_H
