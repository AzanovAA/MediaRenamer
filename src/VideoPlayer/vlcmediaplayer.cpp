#include <QtCore/QDebug>

#include <vlc/vlc.h>

#include "vlcaudio.h"
#include "vlcerror.h"
#include "vlcinstance.h"
#include "vlcmedia.h"
#include "vlcmediaplayer.h"
#include "vlcvideo.h"
#include "videodelegate.h"

#include <QApplication>

VlcMediaPlayer::VlcMediaPlayer(const QStringList &args, QObject *parent)
    : QObject(parent)
{
    _instance = new VlcInstance(args, this);
    _vlcMediaPlayer = libvlc_media_player_new(_instance->core());
    _vlcEvents = libvlc_media_player_event_manager(_vlcMediaPlayer);

    /* Disable mouse and keyboard events */
    libvlc_video_set_key_input(_vlcMediaPlayer, false);
    libvlc_video_set_mouse_input(_vlcMediaPlayer, false);

    VlcError::errmsg();

    _vlcAudio = new VlcAudio(this);
    _vlcVideo = new VlcVideo(this);

    _videoWidget = 0;
    _media = 0;

    createCoreConnections();

    VlcError::errmsg();
}

VlcMediaPlayer::~VlcMediaPlayer()
{
    removeCoreConnections();

    delete _vlcAudio;
    delete _vlcVideo;

    _vlcAudio = NULL;
    _vlcVideo = NULL;

    if(_media != NULL)
    {
        delete _media;
        _media = NULL;
    }

    libvlc_media_player_release(_vlcMediaPlayer);

    VlcError::errmsg();
}

libvlc_media_player_t *VlcMediaPlayer::core()
{
    return _vlcMediaPlayer;
}

VlcAudio *VlcMediaPlayer::audio()
{
    return _vlcAudio;
}

VlcVideo *VlcMediaPlayer::video()
{
    return _vlcVideo;
}

void VlcMediaPlayer::createCoreConnections()
{
    QList<libvlc_event_e> list;
    list << libvlc_MediaPlayerMediaChanged
         << libvlc_MediaPlayerNothingSpecial
         << libvlc_MediaPlayerOpening
         << libvlc_MediaPlayerBuffering
         << libvlc_MediaPlayerPlaying
         << libvlc_MediaPlayerPaused
         << libvlc_MediaPlayerStopped
         << libvlc_MediaPlayerForward
         << libvlc_MediaPlayerBackward
         << libvlc_MediaPlayerEndReached
         << libvlc_MediaPlayerEncounteredError
         << libvlc_MediaPlayerTimeChanged
         << libvlc_MediaPlayerPositionChanged
         << libvlc_MediaPlayerSeekableChanged
         << libvlc_MediaPlayerPausableChanged
         << libvlc_MediaPlayerTitleChanged
         << libvlc_MediaPlayerSnapshotTaken
         << libvlc_MediaPlayerLengthChanged
         << libvlc_MediaPlayerVout;

    foreach(const libvlc_event_e &event, list)
        libvlc_event_attach(_vlcEvents, event, libvlc_callback, this);
}

void VlcMediaPlayer::removeCoreConnections()
{
    QList<libvlc_event_e> list;
    list << libvlc_MediaPlayerMediaChanged
         << libvlc_MediaPlayerNothingSpecial
         << libvlc_MediaPlayerOpening
         << libvlc_MediaPlayerBuffering
         << libvlc_MediaPlayerPlaying
         << libvlc_MediaPlayerPaused
         << libvlc_MediaPlayerStopped
         << libvlc_MediaPlayerForward
         << libvlc_MediaPlayerBackward
         << libvlc_MediaPlayerEndReached
         << libvlc_MediaPlayerEncounteredError
         << libvlc_MediaPlayerTimeChanged
         << libvlc_MediaPlayerPositionChanged
         << libvlc_MediaPlayerSeekableChanged
         << libvlc_MediaPlayerPausableChanged
         << libvlc_MediaPlayerTitleChanged
         << libvlc_MediaPlayerSnapshotTaken
         << libvlc_MediaPlayerLengthChanged
         << libvlc_MediaPlayerVout;

    foreach(const libvlc_event_e &event, list)
        libvlc_event_detach(_vlcEvents, event, libvlc_callback, this);
}

bool VlcMediaPlayer::hasVout() const
{
    bool status = false;
    if(_vlcMediaPlayer)
        status = libvlc_media_player_has_vout(_vlcMediaPlayer);

    return status;
}

int VlcMediaPlayer::length() const
{
    libvlc_time_t length = libvlc_media_player_get_length(_vlcMediaPlayer);

    VlcError::errmsg();

    return length;
}

VlcMedia *VlcMediaPlayer::currentMedia()
{
    return _media;
}

libvlc_media_t *VlcMediaPlayer::currentMediaCore()
{
    libvlc_media_t *media = libvlc_media_player_get_media(_vlcMediaPlayer);

    VlcError::errmsg();

    return media;
}

void VlcMediaPlayer::open(VlcMedia *media)
{
    if(_media != NULL)
    {
        delete _media;
        _media = NULL;
    }

    _media = media;

    _media->initVlcMedia(_instance);
    libvlc_media_player_set_media(_vlcMediaPlayer, media->core());

    VlcError::errmsg();

    play();
}

void VlcMediaPlayer::open(const QString &filePath, bool localFile)
{
    if(_media != NULL)
    {
        delete _media;
        _media = NULL;
    }

    _media = new VlcMedia(filePath, localFile);
    _media->initVlcMedia(_instance);

    libvlc_media_player_set_media(_vlcMediaPlayer, _media->core());

    VlcError::errmsg();

    play();
}

void VlcMediaPlayer::openOnly(VlcMedia *media)
{
    _media = media;
    libvlc_media_player_set_media(_vlcMediaPlayer, media->core());

    VlcError::errmsg();
}

void VlcMediaPlayer::openOnly(const QString &filePath, bool localFile)
{
    _media = new VlcMedia(filePath, localFile);;
    _media->initVlcMedia(_instance);

    libvlc_media_player_set_media(_vlcMediaPlayer, _media->core());

    VlcError::errmsg();
}

void VlcMediaPlayer::play()
{
    if(!_vlcMediaPlayer)
        return;

    if(_videoWidget)
        _currentWId = _videoWidget->request();
    else
        _currentWId = 0;

    if(_currentWId)
    {
#if defined(Q_OS_WIN32)
        libvlc_media_player_set_hwnd(_vlcMediaPlayer, (void *)_currentWId);
#elif defined(Q_OS_DARWIN)
        libvlc_media_player_set_nsobject(_vlcMediaPlayer, (void *)_currentWId);
#elif defined(Q_OS_UNIX)
        libvlc_media_player_set_xwindow(_vlcMediaPlayer, _currentWId);
#endif
    }

    libvlc_media_player_play(_vlcMediaPlayer);

    VlcError::errmsg();
}

void VlcMediaPlayer::pause()
{
    if(!_vlcMediaPlayer)
        return;

    if(libvlc_media_player_can_pause(_vlcMediaPlayer))
        libvlc_media_player_set_pause(_vlcMediaPlayer, true);

    VlcError::errmsg();
}

void VlcMediaPlayer::togglePause()
{
    if(!_vlcMediaPlayer)
        return;

    if(libvlc_media_player_can_pause(_vlcMediaPlayer))
        libvlc_media_player_pause(_vlcMediaPlayer);

    VlcError::errmsg();
}

void VlcMediaPlayer::resume()
{
    if(!_vlcMediaPlayer)
        return;

    if(libvlc_media_player_can_pause(_vlcMediaPlayer))
        libvlc_media_player_set_pause(_vlcMediaPlayer, false);

    VlcError::errmsg();
}

void VlcMediaPlayer::setTime(int time)
{
    libvlc_media_player_set_time(_vlcMediaPlayer, time);

    VlcError::errmsg();
}

void VlcMediaPlayer::setVideoWidget(VideoDelegate *widget)
{
    _videoWidget = widget;
}

Vlc::State VlcMediaPlayer::state() const
{
    if(!libvlc_media_player_get_media(_vlcMediaPlayer))
        return Vlc::Idle;

    libvlc_state_t state;
    state = libvlc_media_player_get_state(_vlcMediaPlayer);

    VlcError::errmsg();

    return Vlc::State(state);
}

void VlcMediaPlayer::stop()
{
    if(!_vlcMediaPlayer)
        return;

    if(_videoWidget)
        _videoWidget->release();
    _currentWId = 0;

    libvlc_media_player_stop(_vlcMediaPlayer);

    VlcError::errmsg();
}

int VlcMediaPlayer::time() const
{
    libvlc_time_t time = libvlc_media_player_get_time(_vlcMediaPlayer);

    VlcError::errmsg();

    return time;
}

VideoDelegate *VlcMediaPlayer::videoWidget()
{
    return _videoWidget;
}

void VlcMediaPlayer::libvlc_callback(const libvlc_event_t *event, void *data)
{
    VlcMediaPlayer *core = (VlcMediaPlayer *)data;

    switch(event->type)
    {
    case libvlc_MediaPlayerMediaChanged:
        emit core->mediaChanged(event->u.media_player_media_changed.new_media);
        break;
    case libvlc_MediaPlayerNothingSpecial:
        emit core->nothingSpecial();
        break;
    case libvlc_MediaPlayerOpening:
        emit core->opening();
        break;
    case libvlc_MediaPlayerBuffering:
        emit core->buffering(event->u.media_player_buffering.new_cache);
    case libvlc_MediaPlayerPlaying:
        emit core->playing();
        break;
    case libvlc_MediaPlayerPaused:
        emit core->paused();
        break;
    case libvlc_MediaPlayerStopped:
        emit core->stopped();
        break;
    case libvlc_MediaPlayerForward:
        emit core->forward();
        break;
    case libvlc_MediaPlayerBackward:
        emit core->backward();
        break;
    case libvlc_MediaPlayerEndReached:
        emit core->end();
        break;
    case libvlc_MediaPlayerEncounteredError:
        emit core->error();
        break;
    case libvlc_MediaPlayerTimeChanged:
        emit core->timeChanged(event->u.media_player_time_changed.new_time);
        break;
    case libvlc_MediaPlayerPositionChanged:
        emit core->positionChanged(event->u.media_player_position_changed.new_position);
        break;
    case libvlc_MediaPlayerSeekableChanged:
        emit core->seekableChanged(event->u.media_player_seekable_changed.new_seekable);
        break;
    case libvlc_MediaPlayerPausableChanged:
        emit core->pausableChanged(event->u.media_player_pausable_changed.new_pausable);
        break;
    case libvlc_MediaPlayerTitleChanged:
        emit core->titleChanged(event->u.media_player_title_changed.new_title);
        break;
    case libvlc_MediaPlayerSnapshotTaken:
        emit core->snapshotTaken(event->u.media_player_snapshot_taken.psz_filename);
        break;
    case libvlc_MediaPlayerLengthChanged:
        emit core->lengthChanged(event->u.media_player_length_changed.new_length);
        break;
    case libvlc_MediaPlayerVout:
        emit core->vout(event->u.media_player_vout.new_count);
        break;
    default:
        break;
    }

    if(event->type >= libvlc_MediaPlayerNothingSpecial && event->type <= libvlc_MediaPlayerEncounteredError)
        emit core->stateChanged();
}

float VlcMediaPlayer::position()
{
    if(!_vlcMediaPlayer)
        return -1;

    return libvlc_media_player_get_position(_vlcMediaPlayer);
}

void VlcMediaPlayer::setPosition(float pos)
{
    libvlc_media_player_set_position(_vlcMediaPlayer, pos);

    VlcError::errmsg();
}
