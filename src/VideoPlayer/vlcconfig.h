#ifndef VLCCONFIG_H
#define VLCCONFIG_H

#include <QtCore/QStringList>

namespace Vlc
{
    enum ActionsType
    {
        AudioTrack,
        Subtitles,
        VideoTrack,
        Other
    };

    enum AudioCodec
    {
        NoAudio,
        MPEG2Audio,
        MP3,
        MPEG4Audio,
        Vorbis,
        Flac
    };

    enum AudioOutput
    {
        DefaultAout
    };

    enum Deinterlacing
    {
        Disabled,
        Discard,
        Blend,
        Mean,
        Bob,
        Linear,
        X
    };

    enum Meta
    {
        Title,
        Artist,
        Genre,
        Copyright,
        Album,
        TrackNumber,
        Description,
        Rating,
        Date,
        Setting,
        URL,
        Language,
        NowPlaying,
        Publisher,
        EncodedBy,
        ArtworkURL,
        TrackID
    };

    enum Mux
    {
        TS,
        PS,
        MP4,
        OGG,
        AVI
    };

    enum PlaybackMode
    {
        DefaultPlayback,
        Loop,
        Repeat
    };

    enum Ratio
    {
        Original,
        R_16_9,
        R_16_10,
        R_185_100,
        R_221_100,
        R_235_100,
        R_239_100,
        R_4_3,
        R_5_4,
        R_5_3,
        R_1_1
    };

    enum Scale
    {
        NoScale,
        S_1_05,
        S_1_1,
        S_1_2,
        S_1_3,
        S_1_4,
        S_1_5,
        S_1_6,
        S_1_7,
        S_1_8,
        S_1_9,
        S_2_0
    };

    enum State
    {
        Idle,
        Opening,
        Buffering,
        Playing,
        Paused,
        Stopped,
        Ended,
        Error
    };

    enum TrackType
    {
        UnknownType,
        Audio,
        Video,
        Text
    };

    enum VideoCodec
    {
        NoVideo,
        MPEG2Video,
        MPEG4Video,
        H264,
        Theora
    };

    enum VideoOutput
    {
#if defined(Q_OS_LINUX)
        X11,
        XVideo,
        GLX,
#elif defined(Q_OS_WIN32)
        DirectX,
        Direct3D,
        OpenGL,
#endif
        DefaultVout
    };


    QStringList audioCodec();
    QStringList audioOutput();
    QStringList audioOutputHuman();
    QStringList deinterlacing();
    QStringList mux();
    QStringList ratio();
    QStringList ratioHuman();
    QList<float> scale();
    QStringList videoCodec();
    QStringList videoOutput();
    QStringList videoOutputHuman();
    QStringList videoPlayerArgs();
}

#endif // VLCQT_ENUMS_H_
