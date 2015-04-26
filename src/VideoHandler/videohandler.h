#ifndef VIDEOHANDLER_H
#define VIDEOHANDLER_H

#include <string>

extern "C" {
#include <libavcodec/avcodec.h>
}

class VideoData;

struct AVFormatContext;
struct SwsContext;

class VideoHandler
{
public:
    VideoHandler();

    int openInputFile(VideoData &video);
    int initPreview(VideoData &video);
    int initCodecs(VideoData &video);

private:
    int allocSwsContext(SwsContext **swsContext, AVFrame *previewFrameRGB, AVCodecContext *codecContext, uint8_t **buffer);
    int getFirstVideoStreamNumber(AVFormatContext *inputContext);
    int getFirstFrame(AVFormatContext *inputContext, AVFrame *source, AVFrame *resultFrame, AVCodecContext *codecContext, SwsContext *swsContext, int videoStreamNumber);
};

#endif // VIDEOHANDLER_H
