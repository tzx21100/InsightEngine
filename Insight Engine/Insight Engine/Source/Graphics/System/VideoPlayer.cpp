#include "Pch.h"
#include "Graphics/System/VideoPlayer.h"
#include "Graphics/Core/Graphics.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace IS {
    VideoPlayer::~VideoPlayer() {
        //cleanup();  // Ensure all resources are freed
    }

    void VideoPlayer::initVideoPlayer() {
        avformat_network_init(); // Initializes network components
        state.av_format_ctx = nullptr;
        state.av_codec_ctx = nullptr;
        state.av_frame = nullptr;
        state.av_packet = nullptr;
        state.sws_scaler_ctx = nullptr;
        textureID = 0;
        frameBuffer = nullptr;
    }

    bool VideoPlayer::loadVideo(const std::string& filepath) {
        // Ensure all previously used resources are freed before loading a new video
        cleanup();

        // Allocate the AVFormatContext
        state.av_format_ctx = avformat_alloc_context();
        if (!state.av_format_ctx) {
            IS_CORE_ERROR("Failed to allocate format context");
            return false;
        }

        // Open the video file
        if (avformat_open_input(&state.av_format_ctx, filepath.c_str(), nullptr, nullptr) != 0) {
            IS_CORE_ERROR("Could not open input file: {}", filepath);
            return false;
        }

        // Retrieve stream information for the file
        if (avformat_find_stream_info(state.av_format_ctx, nullptr) < 0) {
            IS_CORE_ERROR("Could not find stream information for {}", filepath);
            return false;
        }

        // Locate the first video stream
        state.video_stream_index = -1;
        for (unsigned int i = 0; i < state.av_format_ctx->nb_streams; i++) {
            if (state.av_format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                state.video_stream_index = i;
                break;
            }
        }
        if (state.video_stream_index == -1) {
            IS_CORE_ERROR("Could not find a video stream in the file {}", filepath);
            return false;
        }

        // Find the decoder for the video stream
        AVCodecParameters* codec_parameters = state.av_format_ctx->streams[state.video_stream_index]->codecpar;
        const AVCodec* codec = avcodec_find_decoder(codec_parameters->codec_id);
        if (!codec) {
            IS_CORE_ERROR("Unsupported codec!");
            return false;
        }

        // Set up the codec context for the decoder
        state.av_codec_ctx = avcodec_alloc_context3(codec);
        if (!state.av_codec_ctx) {
            IS_CORE_ERROR("Failed to allocate memory for AVCodecContext");
            return false;
        }
        if (avcodec_parameters_to_context(state.av_codec_ctx, codec_parameters) < 0) {
            IS_CORE_ERROR("Failed to copy codec parameters to decoder context");
            return false;
        }

        // Open the codec
        if (avcodec_open2(state.av_codec_ctx, codec, nullptr) < 0) {
            IS_CORE_ERROR("Failed to open codec");
            return false;
        }

        // Allocate the video frame and packet
        state.av_frame = av_frame_alloc();
        state.av_packet = av_packet_alloc();
        if (!state.av_frame || !state.av_packet) {
            IS_CORE_ERROR("Failed to allocate memory for AVFrame or AVPacket");
            return false;
        }

        // Determine the video size and allocate the frame buffer
        state.width = state.av_codec_ctx->width;
        state.height = state.av_codec_ctx->height;
        if (frameBuffer) av_free(frameBuffer); // Free any previously allocated buffer
        frameBuffer = static_cast<uint8_t*>(av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB24, state.width, state.height, 1)));
        if (!frameBuffer) {
            IS_CORE_ERROR("Failed to allocate frame buffer");
            return false;
        }

        // Initialize the SWS context for software scaling
        state.sws_scaler_ctx = sws_getContext(state.width, state.height, state.av_codec_ctx->pix_fmt, state.width, state.height, AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr, nullptr, nullptr);
        if (!state.sws_scaler_ctx) {
            IS_CORE_ERROR("Failed to initialize swscale context");
            return false;
        }

        // Texture setup
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);  // Delete existing texture if it exists
        }

        glGenTextures(1, &textureID); // Generate new texture
        if (textureID == 0) {
            IS_CORE_ERROR("Failed to generate texture ID");
            return false;  // Failed to create a new texture ID
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        // Set your texture parameters here (e.g., glTexParameteri...)
        glBindTexture(GL_TEXTURE_2D, 0);  // Unbind the texture after setup

        return true;  // Video and texture setup was successful
    }

    void VideoPlayer::update(float deltaTime) {
        deltaTime = deltaTime;

        // If no frame to display or video is over, just return
        if (!state.av_format_ctx || !state.av_codec_ctx || !state.av_frame || !state.av_packet) {
            return; // Added checks to ensure pointers are valid before using them
        }

        // Read the next frame
        int ret = av_read_frame(state.av_format_ctx, state.av_packet);
        if (ret < 0) {
            // End of stream or error. In real application, handle EOF from here.
            IS_CORE_ERROR("Could not read frame (maybe end of stream or error)");
            return;
        }

        // Check if the new packet is from the video stream
        if (state.av_packet->stream_index == state.video_stream_index) {
            // Send the packet to the decoder
            ret = avcodec_send_packet(state.av_codec_ctx, state.av_packet);
            if (ret < 0) {
                IS_CORE_ERROR("Error while sending a packet to the decoder");
                return;
            }

            // Receive frame from the decoder
            while (ret >= 0) {
                ret = avcodec_receive_frame(state.av_codec_ctx, state.av_frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break; // The decoder has been fully flushed, or there is more data required to produce a frame.
                }
                else if (ret < 0) {
                    IS_CORE_ERROR("Error while receiving a frame from the decoder");
                    break;
                }

                // At this point, a frame has been decoded
                // Make sure that the frame data is ready to be processed
                if (state.av_frame->data[0] == NULL) {
                    IS_CORE_ERROR("Frame data is not ready in update");
                    continue; // Skipping this frame but not returning entirely, allows for trying the next one
                }
                else {
                    state.frame_ready = true;
                }

                // Here, process your frame, convert to RGB, and upload it to GPU if necessary.
                // If necessary, calculate the display time based on the stream's time_base and the frame's pts.
                // You can also update your video_clock here based on the pts and time_base.

                break; // Usually we break here to display one frame at a time per update call
            }
        }

        // Free the packet that was allocated by av_read_frame
        av_packet_unref(state.av_packet);
    }

    void VideoPlayer::render(float scaleX, float scaleY, float translateX, float translateY) {
        if (!state.av_frame || !frameBuffer || !textureID || !state.frame_ready) {
            // Log which resources are missing for easier debugging
            if (!state.av_frame) IS_CORE_ERROR("AV Frame is null");
            if (!frameBuffer) IS_CORE_ERROR("Frame buffer is null");
            if (!textureID) IS_CORE_ERROR("Texture ID is zero");
            return; // Exiting if any required resources are missing
        }

        // Make sure that the frame data is ready to be processed
        if (state.av_frame->data[0] == NULL) {
            IS_CORE_ERROR("Frame data is not ready");
            return; // Exiting if frame data is not ready
        }

        // Convert the frame from its native format to RGB
        uint8_t* outPlanes[1] = { frameBuffer }; // The buffer where the RGB image will be stored
        int lineSize[1] = { 3 * state.width }; // Size of a single row in bytes
        int response = sws_scale(state.sws_scaler_ctx,
            (const uint8_t* const*)state.av_frame->data,
            state.av_frame->linesize,
            0, state.height,
            outPlanes, lineSize);
        if (response < 0) {
            IS_CORE_ERROR("Error while converting frame to RGB: {}", response);
            return;
        }
        

        // Update OpenGL texture with new frame data
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, state.width, state.height, 0, GL_RGB, GL_UNSIGNED_BYTE, frameBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);  // Unbind the texture after updating it

        // Setup transformation matrix for rendering
        glm::mat4 transform = glm::translate(glm::mat4(1.0), glm::vec3(translateX, translateY, 0.0)) * glm::scale(glm::mat4(1.0), glm::vec3(scaleX, scaleY, 1.0));

        // Activate shader program and set the transform and texture uniform
        ISGraphics::videoShader.use();  // Assuming 'videoShader' is an instance of your Shader class

        GLint tex_arr_uniform = glGetUniformLocation(ISGraphics::videoShader.getHandle(), "uModel_to_ndc_xform");
        if (tex_arr_uniform >= 0) {
            glUniformMatrix4fv(tex_arr_uniform, 1, GL_FALSE, glm::value_ptr(transform));
        }
        else {
            IS_CORE_ERROR({ "uModel_to_ndc_xform Uniform for video player not found, shader compilation failed?" });
        }

        GLint screenTextureUniform = glGetUniformLocation(ISGraphics::videoShader.getHandle(), "uScreenTexture");
        if (screenTextureUniform >= 0) {
            glActiveTexture(GL_TEXTURE0); // Activate the texture unit first
            glBindTexture(GL_TEXTURE_2D, textureID); // Bind the actual texture
            glUniform1i(screenTextureUniform, 0); // Then set the uniform
        }
        else {
            IS_CORE_ERROR({ "uScreenTexture Uniform for video player not found, shader compilation failed?" });
        }

        // Render the quad with the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindVertexArray(ISGraphics::meshes[4].vao_ID);
        glDrawArrays(GL_TRIANGLES, 0, 6); // Assuming the quad is setup to be drawn with 6 vertices
        glBindVertexArray(0);  // Unbind the VAO

        ISGraphics::videoShader.unUse(); // Unuse the shader program
        state.frame_ready = false;
    }

    void VideoPlayer::cleanup() {
        // Free the YUV frame
        if (state.av_frame) {
            av_frame_free(&state.av_frame);
        }

        // Free the packet
        if (state.av_packet) {
            av_packet_free(&state.av_packet);
        }

        // Close the codec
        if (state.av_codec_ctx) {
            avcodec_close(state.av_codec_ctx);
            avcodec_free_context(&state.av_codec_ctx);
        }

        // Close the video file or stream
        if (state.av_format_ctx) {
            avformat_close_input(&state.av_format_ctx);
        }

        // Free the software scaling context
        if (state.sws_scaler_ctx) {
            sws_freeContext(state.sws_scaler_ctx);
            state.sws_scaler_ctx = nullptr;
        }

        if (textureID) {
            glDeleteTextures(1, &textureID);
            textureID = 0;
        }

        // Free frame buffer if allocated
        if (frameBuffer) {
            delete[] frameBuffer;
            // frameBuffer = nullptr;
        }
    }
}