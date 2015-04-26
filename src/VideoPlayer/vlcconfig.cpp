#include "vlcconfig.h"

QStringList Vlc::audioCodec()
{
    QStringList list;
    list << "none"
         << "mpga"
         << "mp3"
         << "mp4a"
         << "vorb"
         << "flac";

    return list;
}

QStringList Vlc::audioOutput()
{
    QStringList list;

    list << "default";

    return list;
}

QStringList Vlc::audioOutputHuman()
{
    QStringList list;

    list << "default";

    return list;
}

QStringList Vlc::deinterlacing()
{
    QStringList list;
    list << ""
         << "discard"
         << "blend"
         << "man"
         << "bob"
         << "linear"
         << "x";

    return list;
}

QStringList Vlc::mux()
{
    QStringList list;
    list << "ts"
         << "ps"
         << "mp4"
         << "ogg"
         << "avi";

    return list;
}

QStringList Vlc::ratio()
{
    QStringList list;
    list << ""
         << "16:9"
         << "16:10"
         << "185:100"
         << "221:100"
         << "235:100"
         << "239:100"
         << "4:3"
         << "5:4"
         << "5:3"
         << "1:1";

    return list;
}

QStringList Vlc::ratioHuman()
{
    QStringList list;
    list << ""
         << "16:9"
         << "16:10"
         << "1.85:1"
         << "2.21:1"
         << "2.35:1"
         << "2.39:1"
         << "4:3"
         << "5:4"
         << "5:3"
         << "1:1";

    return list;
}

QList<float> Vlc::scale()
{
    QList<float> list;
    list << (float) 0
         << (float) 1.05
         << (float) 1.1
         << (float) 1.2
         << (float) 1.3
         << (float) 1.4
         << (float) 1.5
         << (float) 1.6
         << (float) 1.7
         << (float) 1.8
         << (float) 1.9
         << (float) 2.0;

    return list;
}

QStringList Vlc::videoCodec()
{
    QStringList list;
    list << "none"
         << "mpgv"
         << "mp4v"
         << "h264"
         << "theora";

    return list;
}

QStringList Vlc::videoOutput()
{
    QStringList list;

#if defined(Q_OS_LINUX)
    list << "x11"
         << "xvideo"
         << "glx";
#elif defined(Q_OS_WIN32)
    list << "directx"
         << "direct3d"
         << "opengl";
#endif

    list << "default";

    return list;
}

QStringList Vlc::videoOutputHuman()
{
    QStringList list;

#if defined(Q_OS_LINUX)
    list << "X11"
         << "XVideo"
         << "OpenGL";
#elif defined(Q_OS_WIN32)
    list << "DirectX"
         << "Direct3D"
         << "OpenGL";
#endif

    list << "default";

    return list;
}

QStringList Vlc::videoPlayerArgs()
{
    QStringList args;

    args << "--intf=dummy"
         << "--no-media-library"
         << "--no-stats"
         << "--no-osd"
         << "--no-loop"
         << "--no-video-title-show"
         << "--file-logging"
         << "--no-plugins-cache"
    #if defined(Q_OS_DARWIN)
         << "--vout=macosx"
    #endif
         << "--drop-late-frames";

    return args;
}

