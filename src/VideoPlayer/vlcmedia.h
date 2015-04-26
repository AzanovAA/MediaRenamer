#ifndef VLCMEDIA_H
#define VLCMEDIA_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

#include "vlcconfig.h"

class VlcInstance;

struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;

class VlcMedia : public QObject
{
Q_OBJECT
public:
    explicit VlcMedia(const QString &location, bool localFile, QObject *parent = NULL);
    explicit VlcMedia(const QString &location, QObject *parent = NULL);

    VlcMedia(libvlc_media_t *media);
    ~VlcMedia();

    libvlc_media_t *core();
    QString currentLocation() const;

    QString duplicate(const QString &name, const QString &path, const Vlc::Mux &mux);
    QString duplicate(const QString &name, const QString &path, const Vlc::Mux &mux, const Vlc::AudioCodec &audioCodec,
                      const Vlc::VideoCodec &videoCodec);
    QString duplicate(const QString &name, const QString &path, const Vlc::Mux &mux, const Vlc::AudioCodec &audioCodec,
                      const Vlc::VideoCodec &videoCodec, int bitrate, int fps, int scale);
    QString merge(const QString &name, const QString &path, const Vlc::Mux &mux);

    QString record(const QString &name, const QString &path, const Vlc::Mux &mux, bool duplicate = false);
    QString record(const QString &name, const QString &path,const Vlc::Mux &mux, const Vlc::AudioCodec &audioCodec,
                   const Vlc::VideoCodec &videoCodec, bool duplicate = false);

    QString record(const QString &name, const QString &path, const Vlc::Mux &mux, const Vlc::AudioCodec &audioCodec,
                   const Vlc::VideoCodec &videoCodec, int bitrate, int fps, int scale, bool duplicate = false);
    void setProgram(int program);
    void setOption(const QString &option);
    void setOptions(const QStringList &options);
    Vlc::TrackType trackType();
    void initVlcMedia(VlcInstance *instance);

signals:
    void metaChanged(const Vlc::Meta &);
    void subitemAdded(libvlc_media_t *);
    void durationChanged(int);
    void parsedChanged(int);
    void freed(libvlc_media_t *);
    void stateChanged(const Vlc::State &);

private:
    static void libvlc_callback(const libvlc_event_t *event, void *data);

    void createCoreConnections();
    void removeCoreConnections();

    libvlc_media_t * _vlcMedia;
    libvlc_event_manager_t *_vlcEvents;

    QString _currentLocation;
    bool _localFile;
};

#endif // VLCMEDIA_H
