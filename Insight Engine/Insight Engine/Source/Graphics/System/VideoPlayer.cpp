#include "Pch.h"
#include "Graphics/System/VideoPlayer.h"
#include "Graphics/Core/Graphics.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace IS {
    void VideoPlayer::initVideoPlayer(float widthScalar, float heightScalar, float xPosScalar, float yPosScalar, bool loop) {
        avformat_network_init();
        textureID = 0;
        frameBuffer = nullptr;
        state.av_format_ctx = nullptr;
        state.av_codec_ctx = nullptr;
        state.av_frame = nullptr;
        state.av_packet = nullptr;
        state.sws_scaler_ctx = nullptr;

        state.toLoop = loop;
        state.scaleX = widthScalar;
        state.scaleY = heightScalar;
        state.transX = xPosScalar;
        state.transY = yPosScalar;
    }


    void VideoPlayer::loadVideo(const std::string& filepath) {
        cleanup(); // Ensure previous resources are released

        if (avformat_open_input(&state.av_format_ctx, filepath.c_str(), nullptr, nullptr) != 0) {
            IS_CORE_ERROR("Failed to open video file: {}", filepath);
            return;
        }

        if (avformat_find_stream_info(state.av_format_ctx, nullptr) < 0) {
            IS_CORE_ERROR("Failed to find stream information");
            return;
        }

        state.video_stream_index = -1;
        for (unsigned int i = 0; i < state.av_format_ctx->nb_streams; i++) {
            if (state.av_format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                state.video_stream_index = i;
                break;
            }
        }
        if (state.video_stream_index == -1) {
            IS_CORE_ERROR("Failed to find a video stream");
            return;
        }

        AVCodecParameters* codec_params = state.av_format_ctx->streams[state.video_stream_index]->codecpar;
        const AVCodec* codec = avcodec_find_decoder(codec_params->codec_id);
        if (!codec) {
            IS_CORE_ERROR("Unsupported codec!");
            return;
        }

        state.av_codec_ctx = avcodec_alloc_context3(codec);
        if (!state.av_codec_ctx) {
            IS_CORE_ERROR("Failed to allocate codec context");
            return;
        }

        if (avcodec_parameters_to_context(state.av_codec_ctx, codec_params) < 0) {
            IS_CORE_ERROR("Failed to copy codec parameters");
            return;
        }

        if (avcodec_open2(state.av_codec_ctx, codec, nullptr) < 0) {
            IS_CORE_ERROR("Failed to open codec");
            return;
        }

        state.av_frame = av_frame_alloc();
        state.av_packet = av_packet_alloc();
        if (!state.av_frame || !state.av_packet) {
            IS_CORE_ERROR("Failed to allocate frame or packet");
            return;
        }

        state.width = state.av_codec_ctx->width;
        state.height = state.av_codec_ctx->height;
        prepareFrameBufferAndTexture();
        prepareScalerContext();
    }

    void VideoPlayer::update() {

        if (!state.av_format_ctx || !state.av_codec_ctx || !state.playing) return;

        // Receiving and decoding
        int response = av_read_frame(state.av_format_ctx, state.av_packet);

        if (response == AVERROR_EOF) {
            if (state.toLoop) {
                av_seek_frame(state.av_format_ctx, state.video_stream_index, 0, AVSEEK_FLAG_FRAME);
                avcodec_flush_buffers(state.av_codec_ctx);
            }
            else {
                state.playing = false;
            }
            return; // Exit if not looping
        }

        if (response >= 0 && state.av_packet->stream_index == state.video_stream_index) {
            response = avcodec_send_packet(state.av_codec_ctx, state.av_packet);
            if (response < 0) {
                IS_CORE_ERROR("Error sending packet to decoder: {}", response);
                return;
            }

            while (response >= 0) {
                response = avcodec_receive_frame(state.av_codec_ctx, state.av_frame);
                if (response == AVERROR(EAGAIN)) {
                    break;
                }
                else if (response < 0) {
                    IS_CORE_ERROR("Error receiving frame from decoder: {}", response);
                    return;
                }
                // Frame is ready
                if (state.av_frame->data[0] != nullptr) {
                    // Update texture with the new frame
                    updateTexture();
                }
            }
        }
        av_packet_unref(state.av_packet); // Release resources
    }

    void VideoPlayer::render() {
        if (!state.av_frame || !frameBuffer || !textureID || !state.playing) return;

        // Update and bind the texture
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, state.width, state.height, 0, GL_RGB, GL_UNSIGNED_BYTE, frameBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Setup transformation and rendering
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(state.transX, state.transY, 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(state.scaleX, state.scaleY, 1.0f));
        ISGraphics::videoShader.use();

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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(glGetUniformLocation(ISGraphics::videoShader.getHandle(), "uScreenTexture"), 0);

        glBindVertexArray(ISGraphics::meshes[6].vao_ID);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        ISGraphics::videoShader.unUse();
    }

    void VideoPlayer::cleanup() {
        if (state.av_frame) {
            av_frame_free(&state.av_frame);
        }
        if (state.av_packet) {
            av_packet_free(&state.av_packet);
        }
        if (state.av_codec_ctx) {
            avcodec_close(state.av_codec_ctx);
            avcodec_free_context(&state.av_codec_ctx);
        }
        if (state.av_format_ctx) {
            avformat_close_input(&state.av_format_ctx);
        }
        if (state.sws_scaler_ctx) {
            sws_freeContext(state.sws_scaler_ctx);
        }
        if (textureID) {
            glDeleteTextures(1, &textureID);
        }
        if (frameBuffer) {
            av_free(frameBuffer);
        }
    }

    void VideoPlayer::prepareFrameBufferAndTexture() {
        // Ensure previous buffer is freed
        if (frameBuffer) {
            av_free(frameBuffer);
        }
        frameBuffer = static_cast<uint8_t*>(av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB24, state.width, state.height, 1)));

        if (textureID != 0) {
            glDeleteTextures(1, &textureID); // Delete existing texture if it exists
        }
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void VideoPlayer::prepareScalerContext() {
        if (state.sws_scaler_ctx) {
            sws_freeContext(state.sws_scaler_ctx); // Free old context if it exists
        }
        state.sws_scaler_ctx = sws_getContext(state.width, state.height, state.av_codec_ctx->pix_fmt,
            state.width, state.height, AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr, nullptr, nullptr);
    }

    void VideoPlayer::updateTexture() {
        if (!state.sws_scaler_ctx) return; // Safety check

        uint8_t* dest[1] = { frameBuffer }; // Destination for the RGB data
        int lineSize[1] = { 3 * state.width }; // RGB stride

        sws_scale(state.sws_scaler_ctx, (const uint8_t* const*)state.av_frame->data, state.av_frame->linesize,
            0, state.height, dest, lineSize);
    }

    void VideoPlayer::createAndLoadVideo(const std::string& filepath, float widthScalar, float heightScalar, float xPosScalar, float yPosScalar, bool loop) {
        VideoPlayer video;
        video.initVideoPlayer(widthScalar, heightScalar, xPosScalar, yPosScalar, loop);
        video.loadVideo(filepath);

        ISGraphics::videos.emplace_back(video);
    }

    void VideoPlayer::pauseVideo(int index) {
        if (index < ISGraphics::videos.size() && index >= 0)
            ISGraphics::videos[index].state.playing = false;
    }

    void VideoPlayer::resumeVideo(int index) {
        if (index < ISGraphics::videos.size() && index >= 0)
            ISGraphics::videos[index].state.playing = true;
    }

    void VideoPlayer::restartVideo(int index) {
        if (index < ISGraphics::videos.size() && index >= 0)
            av_seek_frame(ISGraphics::videos[index].state.av_format_ctx, 
                          ISGraphics::videos[index].state.video_stream_index, 0, 
                          AVSEEK_FLAG_FRAME);
    }

    void VideoPlayer::unloadVideos() {
        for (auto& vid : ISGraphics::videos) {
            vid.cleanup();
        }
        ISGraphics::videos.clear();
    }

    void VideoPlayer::playVideos() {
        for (auto& vid : ISGraphics::videos) {
            vid.update();
            vid.render();
        }
    }

}