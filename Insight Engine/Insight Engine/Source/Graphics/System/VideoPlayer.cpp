#include "Pch.h"
#include "Graphics/System/VideoPlayer.h"
#include "Graphics/Core/Graphics.h"

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace IS {
    VideoPlayer::VideoPlayer() {
        avformat_network_init();
    }

    VideoPlayer::~VideoPlayer() {
        //closeVideo();  // Ensure all resources are freed
    }

    bool VideoPlayer::loadVideo(const std::string& filepath) {
        // Open the file and allocate the format context
        state.av_format_ctx = nullptr;
        if (avformat_open_input(&state.av_format_ctx, filepath.c_str(), nullptr, nullptr) < 0) {
            std::cerr << "Could not open input file: " << filepath << std::endl;
            return false;
        }

        // Retrieve stream information
        if (avformat_find_stream_info(state.av_format_ctx, nullptr) < 0) {
            std::cerr << "Could not find stream information for " << filepath << std::endl;
            return false;
        }

        // Find the first video stream
        state.video_stream_index = -1;
        for (unsigned int i = 0; i < state.av_format_ctx->nb_streams; i++) {
            if (state.av_format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                state.video_stream_index = i;
                break;
            }
        }

        if (state.video_stream_index == -1) {
            std::cerr << "Could not find a video stream in the file " << filepath << std::endl;
            return false;
        }

        // Get codec parameters from the video stream
        AVCodecParameters * codec_parameters = state.av_format_ctx->streams[state.video_stream_index]->codecpar;

        // Find the decoder for the video stream
        const AVCodec* codec = avcodec_find_decoder(codec_parameters->codec_id);
        if (!codec) {
            std::cerr << "Unsupported codec!" << std::endl;
            return false;
        }

        // Copy context for decoding
        state.av_codec_ctx = avcodec_alloc_context3(codec);
        if (!state.av_codec_ctx) {
            std::cerr << "Failed to allocate memory for AVCodecContext" << std::endl;
            return false;
        }

        if (avcodec_parameters_to_context(state.av_codec_ctx, codec_parameters) < 0) {
            std::cerr << "Failed to copy codec parameters to decoder context" << std::endl;
            return false;
        }

        // Open codec
        if (avcodec_open2(state.av_codec_ctx, codec, nullptr) < 0) {
            std::cerr << "Failed to open codec" << std::endl;
            return false;
        }

        // Allocate memory for frames and packets
        state.av_frame = av_frame_alloc();
        state.av_packet = av_packet_alloc();

        if (!state.av_frame || !state.av_packet) {
            std::cerr << "Failed to allocate memory for AVFrame or AVPacket" << std::endl;
            return false;
        }

        // Setup frame buffer if necessary (e.g., for texture upload)
        state.width = state.av_codec_ctx->width;
        state.height = state.av_codec_ctx->height;
        frameBuffer = new unsigned char[state.width * state.height * 4];  // Assuming RGBA

        return true;
    }

    void VideoPlayer::update(float deltaTime) {
        deltaTime = deltaTime;
        // Read frames from the video stream
        if (av_read_frame(state.av_format_ctx, state.av_packet) >= 0) {
            // Check if the packet is from the video stream
            if (state.av_packet->stream_index == state.video_stream_index) {
                // Send the packet to the decoder
                int response = avcodec_send_packet(state.av_codec_ctx, state.av_packet);
                if (response < 0) {
                    std::cerr << "Error while sending a packet to the decoder: " << response << std::endl;
                    return;
                }

                while (response >= 0) {
                    response = avcodec_receive_frame(state.av_codec_ctx, state.av_frame);
                    if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
                        // The decoder has been fully flushed, or there is more data required to produce a frame.
                        break;
                    }
                    else if (response < 0) {
                        std::cerr << "Error while receiving a frame from the decoder: " << response << std::endl;
                        return;
                    }

                    // At this point, a frame has been decoded
                    // Convert the frame to a suitable format and upload it to the GPU if necessary
                    // Note: Actual frame processing or display logic should be added here.
                    // This could include updating texture buffers for OpenGL.

                    // Example placeholder: Print out frame number
                    // std::cout << "Decoded Frame PTS: " << state.av_frame->pts << std::endl;

                    // Once you're done with the frame, you should free the AVPacket
                    av_packet_unref(state.av_packet);

                    // Based on the video's FPS and the deltaTime, decide if you should display this frame now
                    // This part is omitted for simplicity. You need to match the frame presentation with real time.
                    break;  // In a real application, you might want to wait or skip frames.
                }
            }
            // Unreference the packet every time, whether it was from the video stream or not
            av_packet_unref(state.av_packet);
        }
        else {
            // If av_read_frame returns a negative value, it might be the end of the file or an error.
            // Handle EOF or error here. For simplicity, this is just logging.
            std::cerr << "Could not read frame (maybe end of stream)" << std::endl;
        }
    }

    void VideoPlayer::render(float scaleX, float scaleY, float translateX, float translateY) {
        // Check if there is a frame to render
        if (!state.av_frame || !frameBuffer) return;

        // Convert the frame from its native format to RGB (or another target format)
        int response = sws_scale(state.sws_scaler_ctx,
            (const uint8_t* const*)state.av_frame->data,
            state.av_frame->linesize,
            0,
            state.av_codec_ctx->height,
            &frameBuffer,
            state.av_frame->linesize); // Adjust target linesize for your format
        if (response < 0) {
            std::cerr << "Error while converting frame to RGB" << std::endl;
            return;
        }

        // Update OpenGL texture with the new frame data
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, state.width, state.height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, frameBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);  // Unbind the texture

        // Now draw the texture on the screen
        // Setup the transformation matrix for scaling and translation
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(translateX, translateY, 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, 1.0f));

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
            glUniform1i(screenTextureUniform, 0); // Set this uniform to use texture unit 0
        }
        else {
            IS_CORE_ERROR({ "uScreenTexture Uniform for video player not found, shader compilation failed?" });
        }

        // Render the quad
        glBindVertexArray(ISGraphics::meshes[4].vao_ID); // Ensure you have the correct VAO bound
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);  // Unbind the VAO

        ISGraphics::videoShader.unUse(); // Unuse the shader program
    }

    void VideoPlayer::cleanup() {
        // Free the YUV frame
        if (state.av_frame) {
            av_frame_free(&state.av_frame);
            state.av_frame = nullptr;
        }

        // Free the packet that was allocated by av_read_frame
        if (state.av_packet) {
            av_packet_free(&state.av_packet);
            state.av_packet = nullptr;
        }

        // Close the codecs
        if (state.av_codec_ctx) {
            avcodec_close(state.av_codec_ctx);
            avcodec_free_context(&state.av_codec_ctx);
            state.av_codec_ctx = nullptr;
        }

        // Close the video file or stream
        if (state.av_format_ctx) {
            avformat_close_input(&state.av_format_ctx);
            state.av_format_ctx = nullptr;
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
            frameBuffer = nullptr;
        }
    }
}