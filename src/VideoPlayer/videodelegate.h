#ifndef VIDEODELEGATE_H
#define VIDEODELEGATE_H

#include <QtCore/QObject>
#include <QtGui/qwindowdefs.h>

class VideoDelegate
{
public:
    VideoDelegate() { }
    virtual ~VideoDelegate() { }
    virtual WId request() = 0;
    virtual void release() = 0;
};

#endif // VIDEODELEGATE_H
