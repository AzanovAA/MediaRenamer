#include "videohandler.h"
#include "videodata.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

VideoHandler::VideoHandler()
{
}

int VideoHandler::initCodecs(VideoData &video)
{
    if(video.isCodecsInitialised())
        return 0;

    AVFormatContext *inputContext = video.getInputContext();

    for(unsigned int i=0; i < inputContext->nb_streams; i++)
    {
        AVStream *stream = inputContext->streams[i];
        AVCodecContext *codecContext = stream->codec;

        if(codecContext->codec_type == AVMEDIA_TYPE_VIDEO || codecContext->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            if(avcodec_open2(codecContext, avcodec_find_decoder(codecContext->codec_id), NULL) < 0)
                return -1;
        }
    }

    video.setCodecsInitialised(true);

    return 0;
}

int VideoHandler::initPreview(VideoData &video)
{
    if(openInputFile(video) < 0)
        return -1;

    AVCodecContext *codecContext = NULL;
    AVFrame *previewFrame = NULL, *previewFrameRGB = NULL;
    AVFormatContext *inputContext = video.getInputContext();
    uint8_t *buffer = 0;

    SwsContext *swsContext = NULL;

    int videoStreamNumber = getFirstVideoStreamNumber(inputContext);
    codecContext = inputContext->streams[videoStreamNumber]->codec;

    previewFrame = av_frame_alloc();
    previewFrameRGB = av_frame_alloc();

    if(!previewFrame && ! previewFrameRGB)
    {
        av_free(previewFrame);
        av_free(previewFrameRGB);

        return -1;
    }

    if(allocSwsContext(&swsContext, previewFrameRGB, codecContext, &buffer) < 0)
        return -1;

    if(getFirstFrame(inputContext, previewFrame, previewFrameRGB, codecContext, swsContext, videoStreamNumber) < 0)
    {
        av_free(previewFrameRGB);
//        av_free(previewFrame);
        sws_freeContext(swsContext);

        return -1;
    }

    video.setPreview(previewFrameRGB, buffer, codecContext->width, codecContext->height);

    av_free(previewFrame);
//    sws_freeContext(swsContext);

    return 0;
}

int VideoHandler::allocSwsContext(SwsContext **swsContext, AVFrame *previewFrameRGB, AVCodecContext *codecContext, uint8_t **buffer)
{
    int pictureSize;

    pictureSize = avpicture_get_size(AV_PIX_FMT_RGB24, codecContext->width, codecContext->height);

    if(pictureSize < 0)
        return -1;

    *buffer=(uint8_t *)av_malloc(pictureSize*sizeof(uint8_t));

    *swsContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt, codecContext->width, codecContext->height,
                                AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

    if(!swsContext)
        return -1;


    int status = avpicture_fill((AVPicture *)previewFrameRGB, *buffer, AV_PIX_FMT_RGB24, codecContext->width, codecContext->height);

//    av_free(buffer);

    return status;
}

int VideoHandler::getFirstFrame(AVFormatContext *inputContext, AVFrame *sourceFrame, AVFrame *resultFrame, AVCodecContext *codecContext, SwsContext *swsContext, int videoStreamNumber)
{
    int gotPicture;
    AVPacket packet;
    while(av_read_frame(inputContext, &packet) >= 0)
    {
        if(packet.stream_index == videoStreamNumber)
        {

            avcodec_decode_video2(codecContext, sourceFrame, &gotPicture, &packet);

            if(gotPicture)
            {
                sws_scale(swsContext, (uint8_t const* const*)sourceFrame->data, sourceFrame->linesize, 0, codecContext->height,
                          resultFrame->data, resultFrame->linesize);

                av_free_packet(&packet);

                return 0;
            }
        }

        av_free_packet(&packet);
    }

    return -1;
}

int VideoHandler::openInputFile(VideoData &video)
{
    if(!video.getInputContext())
        if(video.initSourceFileContext() < 0)
            return -1;

    return initCodecs(video);
}

int VideoHandler::getFirstVideoStreamNumber(AVFormatContext *inputContext)
{
    for(unsigned int i = 0; i < inputContext->nb_streams; i++)
        if(inputContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            return i;

    return -1;
}

