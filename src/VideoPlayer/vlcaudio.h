#ifndef VLCAUDIO_H
#define VLCAUDIO_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

class VlcMediaPlayer;

struct libvlc_media_player_t;

class VlcAudio : public QObject
{
Q_OBJECT
public:
    explicit VlcAudio(VlcMediaPlayer *player);
    ~VlcAudio();

    bool getMute() const;
    void setVolume(int volume);
    void setTrack(int track);
    bool toggleMute() const;
    int track() const;
    int trackCount() const;
    QStringList trackDescription() const;
    QList<int> trackIds() const;
    int volume() const;

private:
    libvlc_media_player_t *_vlcMediaPlayer;
};

#endif // VLCAUDIO_H
