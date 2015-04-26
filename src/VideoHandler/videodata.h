#ifndef VIDEODATA_H
#define VIDEODATA_H

#include <string>

struct AVFormatContext;
struct AVFilterContext;
struct AVFilterGraph;
struct AVFrame;

struct FilteringContext
{
    AVFilterContext *buffersinkContext;
    AVFilterContext *buffersrcContext;
    AVFilterGraph *filterGraph;
};

struct Preview
{
    uint8_t *buffer;
    AVFrame *data;
    int width;
    int height;
};

class VideoData
{
public:
    VideoData(const std::string &fileName);
    ~VideoData();

    AVFormatContext* getInputContext() const;
    void setInputContext(AVFormatContext *value);

    AVFormatContext* getOutputContext() const;
    void setOutputContext(AVFormatContext *value);

    AVFilterContext* getBuffersinkContext() const;
    void setBuffersinkContext(AVFilterContext *value);

    AVFilterContext* getBuffersrcContext() const;
    void setBuffersrcContext(AVFilterContext *value);

    AVFilterGraph* getFilterGraph() const;
    void setFilterGraph(AVFilterGraph *value);

    FilteringContext* getFilterContext() const;
    void setFilterContext(FilteringContext *value);

    const Preview &getPreview() const;
    void setPreview(AVFrame *value, uint8_t *buffer, int width, int height);

    int initSourceFileContext();

    const std::string &getFileName() const;

    bool isCodecsInitialised() const;
    void setCodecsInitialised(bool value);

    void closeOutput();
    void closeInput();
private:
    bool codecsInitialised;
    const std::string fileName;

    int width, height;

    Preview preview;

    AVFormatContext *inputContext;
    AVFormatContext *outputContext;
    FilteringContext *filterContext;
};

#endif // VIDEODATA_H
