/*!
 * \file VideoPlayer.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2451
 * \date 15-03-2024
 * \brief
 * This header file defines the VideoPlayer class, which represents a dynamic video player
 * class for cutscenes and tutorials in Fragments.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                      guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_VIDEOPLAYER_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_VIDEOPLAYER_H

#include <string>
#include <glad/glad.h>
// #include "Graphics/Core/Graphics.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

namespace IS {
    class VideoPlayer {
    public:

        VideoPlayer() = default;
        ~VideoPlayer() = default;


        /**
         * Initializes the video player with given scaling and translation parameters.
         *
         * @param widthScalar Scaling factor for the width of the video.
         * @param heightScalar Scaling factor for the height of the video.
         * @param xPosScalar Translation factor on the X-axis.
         * @param yPosScalar Translation factor on the Y-axis.
         * @param loop Whether the video should loop when it reaches the end.
         */
        // scale and translate values are relative scalars
        // scale: [0 to 1]
        // translate: [-1 to 1]
        void initVideoPlayer(float widthScalar, float heightScalar, float xPosScalar, float yPosScalar, bool loop = false);

        /**
         * Loads a video from the specified file path.
         *
         * @param filepath The path to the video file.
         */
        void loadVideo(const std::string& filepath);

        /**
         * Updates the video player state, should be called every frame.
         *
         * @param deltaTime Time elapsed since the last frame.
         */
        void update(double deltaTime);

        /**
         * Renders the current frame of the video.
         */
        void render();

        /**
         * Cleans up resources used by the video player.
         */
        void cleanup();

        /**
         * Prepares the frame buffer and texture for video rendering.
         */
        void prepareFrameBufferAndTexture();

        /**
         * Prepares the scaler context for video frame scaling.
         */
        void prepareScalerContext();

        /**
         * Updates the texture with the current video frame.
         */
        void updateTexture();

        // static helper functions for C#

        /**
         * Static function to create and load a video.
         *
         * @param filepath Path to the video file.
         * @param widthScalar Scaling factor for the width.
         * @param heightScalar Scaling factor for the height.
         * @param xPosScalar Horizontal position scalar.
         * @param yPosScalar Vertical position scalar.
         * @param loop Whether to loop the video.
         */
        static void createAndLoadVideo(const std::string& filepath, float widthScalar, float heightScalar, float xPosScalar, float yPosScalar, bool loop);
        
        /**
         * Pauses the video playback.
         *
         * @param index Index of the video to pause.
         */
        static void pauseVideo(int index);

        /**
         * Resumes the video playback.
         *
         * @param index Index of the video to resume.
         */
        static void resumeVideo(int index);

        /**
         * Restarts the video playback from the beginning.
         *
         * @param index Index of the video to restart.
         */
        static void restartVideo(int index);

        /**
         * Unloads all loaded videos and frees their resources.
         */
        static void unloadVideos();

        /**
         * Plays all videos, updating them based on the elapsed time.
         *
         * @param deltaTime Time elapsed since the last update.
         */
        static void playVideos(double deltaTime);

    private:
        struct VideoReaderState {
            int width, height{};
            AVRational time_base{};
            AVFormatContext* av_format_ctx{};
            AVCodecContext* av_codec_ctx{};
            int video_stream_index{};
            AVFrame* av_frame{};
            AVPacket* av_packet{};
            SwsContext* sws_scaler_ctx{};
            float scaleX{}, scaleY{}, transX{}, transY{};

            // to sync video with framerate
            double frame_last_pts = 0; // Last presentation timestamp for the frame
            double frame_last_delay = 40e-3; // Initial assumption of frame delay (25 fps)
            double video_clock = 0; // Video clock to track actual video time
            bool frame_ready = false;
            bool toLoop = true;
            bool playing = true;

            double timeSinceLastFrame = 0.0; // Time elapsed since the last frame was shown
            double frameDuration = 0.0;      // Duration of a single frame
        } state;

        GLuint textureID{};
        unsigned char* frameBuffer{};

        //void closeVideo();
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_VIDEOPLAYER_H