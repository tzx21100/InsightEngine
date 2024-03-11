

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

        // scale and translate values are relative scalars
        // scale: [0 to 1]
        // translate: [-1 to 1]
        void initVideoPlayer(float widthScalar, float heightScalar, float xPosScalar, float yPosScalar, bool loop = false);
        void loadVideo(const std::string& filepath);
        void update(double deltaTime);

        
        void render();
        void cleanup();

        void prepareFrameBufferAndTexture();
        void prepareScalerContext();
        void updateTexture();

        // static helper functions for C#
        static void createAndLoadVideo(const std::string& filepath, float widthScalar, float heightScalar, float xPosScalar, float yPosScalar, bool loop);
        static void pauseVideo(int index);
        static void resumeVideo(int index);
        static void restartVideo(int index);
        static void unloadVideos();

        static void playVideos(double deltaTime);
        

        //static Shader videoShader;

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