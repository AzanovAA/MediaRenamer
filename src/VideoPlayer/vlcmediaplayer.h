#ifndef VLCMEDIAPLAYER_H
#define VLCMEDIAPLAYER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include <QtGui/qwindowdefs.h>

#include "vlcconfig.h"

class VlcAudio;
class VlcInstance;
class VlcMedia;
class VlcVideo;
class VideoDelegate;

struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_media_player_t;

class VlcMediaPlayer : public QObject
{
Q_OBJECT
public:
    explicit VlcMediaPlayer(const QStringList &args, QObject *parent = NULL);
    ~VlcMediaPlayer();
    libvlc_media_player_t *core();
    VlcAudio *audio();
    VlcVideo *video();
    int length() const;
    VlcMedia *currentMedia();
    libvlc_media_t *currentMediaCore();
    void open(VlcMedia *media);
    void open(const QString &filePath, bool localFile = true);
    void openOnly(VlcMedia *media);
    void openOnly(const QString &filePath, bool localFile = true);
    int time() const;
    void setVideoWidget(VideoDelegate *widget);
    bool hasVout() const;
    Vlc::State state() const;
    VideoDelegate *videoWidget();
    float position();
    void setPosition(float pos);

public slots:
    void setTime(int time);
    void play();
    void pause();
    void togglePause();
    void resume();
    void stop();

signals:
    void backward();
    void buffering(float);
    void end();
    void error();
    void forward();
    void lengthChanged(int);
    void mediaChanged(libvlc_media_t *);
    void nothingSpecial();
    void opening();
    void pausableChanged(bool);
    void paused();
    void playing();
    void positionChanged(float);
    void seekableChanged(bool);
    void snapshotTaken(const QString &);
    void stopped();
    void timeChanged(int);
    void titleChanged(int);
    void vout(int);
    void stateChanged();

    void someTralala();

private:
    static void libvlc_callback(const libvlc_event_t *event, void *data);

    void createCoreConnections();
    void removeCoreConnections();

    VlcInstance *_instance;
    libvlc_media_player_t *_vlcMediaPlayer;
    libvlc_event_manager_t *_vlcEvents;

    VlcMedia *_media;

    VlcAudio *_vlcAudio;
    VlcVideo *_vlcVideo;

    VideoDelegate *_videoWidget;
    WId _currentWId;
};

#endif // VLCMEDIAPLAYER_H
