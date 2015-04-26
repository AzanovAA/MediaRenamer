#include "videodata.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavcodec/avcodec.h>
}

VideoData::VideoData(const std::string &fileName) :
    codecsInitialised(false), fileName(fileName), inputContext(NULL), outputContext(NULL), filterContext(NULL)
{
}

VideoData::~VideoData()
{
    if(outputContext)
    {
        av_write_trailer(outputContext);

        if(!(outputContext->oformat->flags & AVFMT_NOFILE))
            avio_close(outputContext->pb);

        avformat_free_context(outputContext);
    }
    if(inputContext)
    {
        for(unsigned int i=0; i < inputContext->nb_streams; i++)
        {
            avcodec_close(inputContext->streams[i]->codec);
            if(outputContext && outputContext->nb_streams > i && outputContext->streams[i] && outputContext->streams[i]->codec)
                avcodec_close(outputContext->streams[i]->codec);
            if(filterContext && filterContext[i].filterGraph)
                avfilter_graph_free(&filterContext[i].filterGraph);
        }

        if(filterContext)
            av_free(filterContext);

        avformat_close_input(&inputContext);

    }

    if(preview.data)
    {
        av_free(preview.data);
        preview.data = NULL;
    }

    if(preview.buffer)
    {
        av_free(preview.buffer);
        preview.buffer = NULL;
    }
}

void VideoData::closeOutput()
{
    if(outputContext)
    {
        av_write_trailer(outputContext);

        if(!(outputContext->oformat->flags & AVFMT_NOFILE))
            avio_close(outputContext->pb);

        avformat_free_context(outputContext);
    }
}

void VideoData::closeInput()
{
    if(inputContext)
    {
        for(unsigned int i=0; i < inputContext->nb_streams; i++)
        {
            avcodec_close(inputContext->streams[i]->codec);
            if(outputContext && outputContext->nb_streams > i && outputContext->streams[i] && outputContext->streams[i]->codec)
                avcodec_close(outputContext->streams[i]->codec);
            if(filterContext && filterContext[i].filterGraph)
                avfilter_graph_free(&filterContext[i].filterGraph);
        }

        if(filterContext)
            av_free(filterContext);

        avformat_close_input(&inputContext);
    }
}

AVFormatContext *VideoData::getInputContext() const
{
    return inputContext;
}

void VideoData::setInputContext(AVFormatContext *value)
{
    inputContext = value;
}
AVFormatContext *VideoData::getOutputContext() const
{
    return outputContext;
}

void VideoData::setOutputContext(AVFormatContext *value)
{
    outputContext = value;
}

int VideoData::initSourceFileContext()
{
    if(avformat_open_input(&inputContext, fileName.c_str(), NULL, NULL) < 0)
        return -2;

    if(avformat_find_stream_info(inputContext, NULL) < 0)
        return -1;

    return 0;
}
const std::string &VideoData::getFileName() const
{
    return fileName;
}
bool VideoData::isCodecsInitialised() const
{
    return codecsInitialised;
}

void VideoData::setCodecsInitialised(bool value)
{
    codecsInitialised = value;
}

const Preview& VideoData::getPreview() const
{
    return preview;
}

void VideoData::setPreview(AVFrame *value, uint8_t *buffer, int width, int height)
{
    preview.data = value;
    preview.buffer = buffer;
    preview.width = width;
    preview.height = height;
}



FilteringContext *VideoData::getFilterContext() const
{
    return filterContext;
}

void VideoData::setFilterContext(FilteringContext *value)
{
    filterContext = value;
}





