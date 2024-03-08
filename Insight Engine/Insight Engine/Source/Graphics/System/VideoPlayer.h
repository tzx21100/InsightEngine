

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
        ~VideoPlayer();
        void initVideoPlayer();
        bool loadVideo(const std::string& filepath);
        void update(float deltaTime);
        void render(float scaleX, float scaleY, float translateX, float translateY);
        void cleanup();

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

            // to sync video with framerate
            double frame_last_pts = 0; // Last presentation timestamp for the frame
            double frame_last_delay = 40e-3; // Initial assumption of frame delay (25 fps)
            double video_clock = 0; // Video clock to track actual video time
        } state;

        GLuint textureID{};
        unsigned char* frameBuffer{};

        //void closeVideo();
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_VIDEOPLAYER_H