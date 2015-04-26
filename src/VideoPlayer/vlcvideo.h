#ifndef VLCVIDEO_H
#define VLCVIDEO_H

#include <QtCore/QList>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "vlcconfig.h"

class VlcMediaPlayer;

struct libvlc_media_player_t;

class VlcVideo : public QObject
{
Q_OBJECT
public:
    explicit VlcVideo(VlcMediaPlayer *player);
    ~VlcVideo();
    Vlc::Ratio aspectRatio() const;
    Vlc::Ratio cropGeometry() const;
    void hideLogo();
    void hideMarquee();
    Vlc::Scale scale() const;
    void setAspectRatio(const Vlc::Ratio &ratio);
    void setCropGeometry(const Vlc::Ratio &ratio);
    void setDeinterlace(const Vlc::Deinterlacing &filter);
    void setScale(const Vlc::Scale &scale);
    void setSubtitle(int subtitle);
    void setSubtitleFile(const QString &subtitle);
    void setTeletextPage(int page);
    void setTrack(int track);
    void showLogo(const QString &file, int x, int y, int opacity = 255);
    void showMarquee(const QString &text, int x, int y, int timeout = 0, int opacity = 255, int size = -1,
                     int color = 0xFFFFFF, int refresh = 0);
    QSize size() const;
    int subtitle() const;
    int subtitleCount() const;
    QStringList subtitleDescription() const;
    QList<int> subtitleIds() const;
    bool takeSnapshot(const QString &path) const;
    int teletextPage() const;
    void toggleTeletextTransparency();
    int track() const;
    int trackCount() const;
    QStringList trackDescription() const;
    QList<int> trackIds() const;

private:
    libvlc_media_player_t *_vlcMediaPlayer;
};

#endif // VLCVIDEO_H
