#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QtCore/QTimer>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QFrame>
#else
    #include <QtGui/QFrame>
#endif

#include "VideoPlayer/vlcconfig.h"
#include "VideoPlayer/videodelegate.h"

class VlcMediaPlayer;

class VideoWidget : public QWidget, public VideoDelegate
{
Q_OBJECT
public:
    explicit VideoWidget(VlcMediaPlayer *player, QWidget *parent = 0);
    explicit VideoWidget(QWidget *parent = 0);
    ~VideoWidget();
    inline Vlc::Ratio currentAspectRatio() const { return _currentAspectRatio; }
    inline Vlc::Ratio defaultAspectRatio() const { return _defaultAspectRatio; }
    void setCurrentAspectRatio(const Vlc::Ratio &ratio);
    void setDefaultAspectRatio(const Vlc::Ratio &ratio);
    inline Vlc::Ratio currentCropRatio() const { return _currentCropRatio; }
    inline Vlc::Ratio defaultCropRatio() const { return _defaultCropRatio; }
    void setCurrentCropRatio(const Vlc::Ratio &ratio);
    void setDefaultCropRatio(const Vlc::Ratio &ratio);
    inline Vlc::Deinterlacing currentDeinterlacing() const { return _currentDeinterlacing; }
    inline Vlc::Deinterlacing defaultDeinterlacing() const { return _defaultDeinterlacing; }
    void setCurrentDeinterlacing(const Vlc::Deinterlacing &deinterlacing);
    void setDefaultDeinterlacing(const Vlc::Deinterlacing &deinterlacing);
    inline Vlc::Scale currentScale() const { return _currentScale; }
    inline Vlc::Scale defaultScale() const { return _defaultScale; }
    void setCurrentScale(const Vlc::Scale &scale);
    void setDefaultScale(const Vlc::Scale &scale);
    void setMediaPlayer(VlcMediaPlayer *player);
    WId request();
    void release();

public slots:
    void enableDefaultSettings();
    void enablePreviousSettings();
    void initDefaultSettings();
    void setAspectRatio(const Vlc::Ratio &ratio);
    void setCropRatio(const Vlc::Ratio &ratio);
    void setDeinterlacing(const Vlc::Deinterlacing &deinterlacing);
    void setScale(const Vlc::Scale &scale);

private slots:
    void applyPreviousSettings();

private:
    void initWidgetVideo();
    void sync();

    VlcMediaPlayer *_vlcMediaPlayer;

    /*QWidget *_video;
    QLayout *_layout;*/

    bool _enableSettings;

    Vlc::Ratio _defaultAspectRatio;
    Vlc::Ratio _defaultCropRatio;
    Vlc::Deinterlacing _defaultDeinterlacing;
    Vlc::Scale _defaultScale;

    Vlc::Ratio _currentAspectRatio;
    Vlc::Ratio _currentCropRatio;
    Vlc::Deinterlacing _currentDeinterlacing;
    Vlc::Scale _currentScale;
};

#endif // VIDEOWIDGET_H
