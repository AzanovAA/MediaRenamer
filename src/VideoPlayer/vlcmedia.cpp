#include <QtCore/QDebug>
#include <QtCore/QDir>

#include <vlc/vlc.h>

#include "vlcerror.h"
#include "vlcinstance.h"
#include "vlcmedia.h"

VlcMedia::VlcMedia(const QString &location, bool localFile, QObject *parent)
    : QObject(parent), _vlcMedia(NULL), _vlcEvents(NULL)
{
    _currentLocation = location;
    _localFile = localFile;
}

VlcMedia::VlcMedia(const QString &location, QObject *parent)
    : QObject(parent), _vlcMedia(NULL), _vlcEvents(NULL)
{
    _currentLocation = location;
    _localFile = false;
}

VlcMedia::VlcMedia(libvlc_media_t *media)
{
    _vlcMedia = libvlc_media_duplicate(media);

    VlcError::errmsg();
}

VlcMedia::~VlcMedia()
{
    removeCoreConnections();

    libvlc_media_release(_vlcMedia);

    VlcError::errmsg();
}

libvlc_media_t *VlcMedia::core()
{
    return _vlcMedia;
}

void VlcMedia::initVlcMedia(VlcInstance *instance)
{
    if(_vlcEvents && _vlcMedia)
        return;

    QString l = _currentLocation;
    if(_localFile)
        l = QDir::toNativeSeparators(l);

    if(_localFile)
        _vlcMedia = libvlc_media_new_path(instance->core(), l.toUtf8().data());
    else
        _vlcMedia = libvlc_media_new_location(instance->core(), l.toUtf8().data());

    _vlcEvents = libvlc_media_event_manager(_vlcMedia);

    createCoreConnections();

    VlcError::errmsg();
    qDebug() << "libvlc" << "Media:" << _currentLocation << "Local:" << _localFile;
}

void VlcMedia::createCoreConnections()
{
    QList<libvlc_event_e> list;
    list << libvlc_MediaMetaChanged
         << libvlc_MediaSubItemAdded
         << libvlc_MediaDurationChanged
         << libvlc_MediaParsedChanged
         << libvlc_MediaFreed
         << libvlc_MediaStateChanged;

    foreach(const libvlc_event_e &event, list)
        libvlc_event_attach(_vlcEvents, event, libvlc_callback, this);
}

void VlcMedia::removeCoreConnections()
{
    QList<libvlc_event_e> list;
    list << libvlc_MediaMetaChanged
         << libvlc_MediaSubItemAdded
         << libvlc_MediaDurationChanged
         << libvlc_MediaParsedChanged
         << libvlc_MediaFreed
         << libvlc_MediaStateChanged;

    foreach(const libvlc_event_e &event, list)
        libvlc_event_detach(_vlcEvents, event, libvlc_callback, this);
}

QString VlcMedia::currentLocation() const
{
    return _currentLocation;
}

QString VlcMedia::duplicate(const QString &name, const QString &path, const Vlc::Mux &mux)
{
    return record(name, path, mux, true);
}

QString VlcMedia::duplicate(const QString &name, const QString &path, const Vlc::Mux &mux, const Vlc::AudioCodec &audioCodec, const Vlc::VideoCodec &videoCodec)
{
    return record(name, path, mux, audioCodec, videoCodec, true);
}

QString VlcMedia::duplicate(const QString &name, const QString &path, const Vlc::Mux &mux, const Vlc::AudioCodec &audioCodec,
                            const Vlc::VideoCodec &videoCodec, int bitrate, int fps, int scale)
{
    return record(name, path, mux, audioCodec, videoCodec, bitrate, fps, scale, true);
}

QString VlcMedia::merge(const QString &name, const QString &path, const Vlc::Mux &mux)
{
    QString option1, option2, parameters;
    QString l = QDir::toNativeSeparators(path + "/" + name);

    parameters = "gather:std{access=file,mux=%1,dst='%2'}";
    parameters = parameters.arg(Vlc::mux()[mux], l + "." + Vlc::mux()[mux]);

    option1 = ":sout-keep";
    option2 = ":sout=#%1";
    option2 = option2.arg(parameters);

    setOption(option1);
    setOption(option2);

    VlcError::errmsg();

    return l + "." + Vlc::mux()[mux];
}

QString VlcMedia::record(const QString &name, const QString &path, const Vlc::Mux &mux, bool duplicate)
{
    QString option1, option2, parameters;
    QString l = QDir::toNativeSeparators(path + "/" + name);

    parameters = "std{access=file,mux=%1,dst='%2'}";
    parameters = parameters.arg(Vlc::mux()[mux], l + "." + Vlc::mux()[mux]);

    if(duplicate)
        option2 = ":sout=#duplicate{dst=display,dst=\"%1\"}";
    else
        option2 = ":sout=#%1";

    option1 = ":sout-all";
    option2 = option2.arg(parameters);

    setOption(option1);
    setOption(option2);

    VlcError::errmsg();

    return l + "." + Vlc::mux()[mux];
}

QString VlcMedia::record(const QString &name, const QString &path, const Vlc::Mux &mux, const Vlc::AudioCodec &audioCodec, const Vlc::VideoCodec &videoCodec,
                         bool duplicate)
{
    QString option1, option2, parameters;
    QString l = QDir::toNativeSeparators(path + "/" + name);

    parameters = "transcode{vcodec=%1,acodec=%2}:std{access=file,mux=%3,dst='%4'}";
    parameters = parameters.arg(Vlc::videoCodec()[videoCodec], Vlc::audioCodec()[audioCodec], Vlc::mux()[mux], l + "." + Vlc::mux()[mux]);

    if(duplicate)
        option2 = ":sout=#duplicate{dst=display,dst=\"%1\"}";
    else
        option2 = ":sout=#%1";

    option1 = ":sout-all";
    option2 = option2.arg(parameters);

    setOption(option1);
    setOption(option2);

    VlcError::errmsg();

    return l + "." + Vlc::mux()[mux];
}

QString VlcMedia::record(const QString &name, const QString &path, const Vlc::Mux &mux, const Vlc::AudioCodec &audioCodec, const Vlc::VideoCodec &videoCodec,
                         int bitrate, int fps, int scale, bool duplicate)
{
    QString option1, option2, parameters;
    QString l = QDir::toNativeSeparators(path + "/" + name);

    parameters = "transcode{vcodec=%1,vb=%2,fps=%3,scale=%4,acodec=%5}:std{access=file,mux=%6,dst='%7'}";
    parameters = parameters.arg(Vlc::videoCodec()[videoCodec], QString::number(bitrate), QString::number(fps), QString::number(scale), Vlc::audioCodec()[audioCodec], Vlc::mux()[mux], l + "." + Vlc::mux()[mux]);

    if(duplicate)
        option2 = ":sout=#duplicate{dst=display,dst=\"%1\"}";
    else
        option2 = ":sout=#%1";

    option1 = ":sout-all";
    option2 = option2.arg(parameters);

    setOption(option1);
    setOption(option2);

    VlcError::errmsg();

    return l + "." + Vlc::mux()[mux];
}

void VlcMedia::setProgram(int program)
{
    QString option = "program=%1";
    setOption(option.arg(program));
}

void VlcMedia::setOption(const QString &option)
{
    libvlc_media_add_option(_vlcMedia, option.toUtf8().data());

    VlcError::errmsg();
}

void VlcMedia::setOptions(const QStringList &options)
{
    foreach(const QString &option, options)
        libvlc_media_add_option(_vlcMedia, option.toUtf8().data());

    VlcError::errmsg();
}

Vlc::TrackType VlcMedia::trackType()
{
//    libvlc_media_track_info_t *info;
    libvlc_media_track_t **info;

    libvlc_media_parse(_vlcMedia);

//    if(!libvlc_media_get_tracks_info(_vlcMedia, &info))
    if(!libvlc_media_tracks_get(_vlcMedia, &info))
    {
        libvlc_media_tracks_release(info, libvlc_media_tracks_get(_vlcMedia, &info));
//        free(info);
        return Vlc::UnknownType;
    }

    libvlc_track_type_t type = (*info)->i_type;

//    free(info);
    libvlc_media_tracks_release(info, libvlc_media_tracks_get(_vlcMedia, &info));

    VlcError::errmsg();

    if(type == libvlc_track_audio)
        return Vlc::Audio;
    else if(type == libvlc_track_video)
        return Vlc::Video;
    else if(type == libvlc_track_text)
        return Vlc::Text;
    else
        return Vlc::UnknownType;
}

void VlcMedia::libvlc_callback(const libvlc_event_t *event, void *data)
{
    VlcMedia *core = (VlcMedia *)data;

    switch(event->type)
    {
    case libvlc_MediaMetaChanged:
        emit core->metaChanged(Vlc::Meta(event->u.media_meta_changed.meta_type));
        break;
    case libvlc_MediaSubItemAdded:
        emit core->subitemAdded(event->u.media_subitem_added.new_child);
        break;
    case libvlc_MediaDurationChanged:
        emit core->durationChanged(event->u.media_duration_changed.new_duration);
        break;
    case libvlc_MediaParsedChanged:
        emit core->parsedChanged(event->u.media_parsed_changed.new_status);
        break;
    case libvlc_MediaFreed:
        emit core->freed(event->u.media_freed.md);
        break;
    case libvlc_MediaStateChanged:
        emit core->stateChanged(Vlc::State(event->u.media_state_changed.new_state));
        break;
    default:
        break;
    }
}
