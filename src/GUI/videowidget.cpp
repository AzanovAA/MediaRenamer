#include <QtGlobal>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QApplication>
    #include <QtWidgets/QDesktopWidget>
    #include <QtWidgets/QHBoxLayout>
    #include <QtWidgets/QToolBar>
#else
    #include <QtGui/QApplication>
    #include <QtGui/QDesktopWidget>
    #include <QtGui/QHBoxLayout>
    #include <QtGui/QToolBar>
#endif

#if defined(Q_WS_X11)
    #include <X11/Xlib.h>
    #include <qx11info_x11.h>
#endif

#include "VideoPlayer/vlcerror.h"
#include "VideoPlayer/vlcmediaplayer.h"
#include "VideoPlayer/vlcvideo.h"

#include "videowidget.h"

VideoWidget::VideoWidget(VlcMediaPlayer *player, QWidget *parent)
    : QWidget(parent)
{
    setMediaPlayer(player);
    initWidgetVideo();
}

VideoWidget::VideoWidget(QWidget *parent)
    : QWidget(parent), _vlcMediaPlayer(0)
{
    initWidgetVideo();
}

VideoWidget::~VideoWidget()
{
    release();
}

void VideoWidget::initWidgetVideo()
{
    _enableSettings = false;
    _defaultAspectRatio = Vlc::Original;
    _defaultCropRatio = Vlc::Original;
    _defaultDeinterlacing = Vlc::Disabled;
    _defaultScale = Vlc::NoScale;
    _currentAspectRatio = Vlc::Original;
    _currentCropRatio = Vlc::Original;
    _currentDeinterlacing = Vlc::Disabled;
    _currentScale = Vlc::NoScale;

    /*_layout = new QHBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _video = 0;*/

    /*QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::transparent);
    setPalette(plt);
    setAutoFillBackground(true);
    setMouseTracking(true);
#ifndef Q_WS_X11
    setAttribute( Qt::WA_PaintOnScreen, true );
#endif*/
}

void VideoWidget::setMediaPlayer(VlcMediaPlayer *player)
{
    player->setVideoWidget(this);
    _vlcMediaPlayer = player;
    connect(_vlcMediaPlayer, SIGNAL(vout(int)), this, SLOT(applyPreviousSettings()));
}

void VideoWidget::setCurrentAspectRatio(const Vlc::Ratio &ratio)
{
    _currentAspectRatio = ratio;
}

void VideoWidget::setCurrentCropRatio(const Vlc::Ratio &ratio)
{
    _currentCropRatio = ratio;
}

void VideoWidget::setCurrentDeinterlacing(const Vlc::Deinterlacing &deinterlacing)
{
    _currentDeinterlacing = deinterlacing;
}

void VideoWidget::setCurrentScale(const Vlc::Scale &scale)
{
    _currentScale = scale;
}

void VideoWidget::setDefaultAspectRatio(const Vlc::Ratio &ratio)
{
    _defaultAspectRatio = ratio;
}

void VideoWidget::setDefaultCropRatio(const Vlc::Ratio &ratio)
{
    _defaultCropRatio = ratio;
}

void VideoWidget::setDefaultDeinterlacing(const Vlc::Deinterlacing &deinterlacing)
{
    _defaultDeinterlacing = deinterlacing;
}

void VideoWidget::setDefaultScale(const Vlc::Scale &scale)
{
    _defaultScale = scale;
}

void VideoWidget::enableDefaultSettings()
{
    /*initDefaultSettings();

    enablePreviousSettings();*/
}

void VideoWidget::enablePreviousSettings()
{
    //_enableSettings = true;
}

void VideoWidget::initDefaultSettings()
{
    /*_currentAspectRatio = defaultAspectRatio();
    _currentCropRatio = defaultCropRatio();
    _currentDeinterlacing = defaultDeinterlacing();
    _currentScale = defaultScale();*/
}

void VideoWidget::applyPreviousSettings()
{
   /* if(!_enableSettings)
        return;

    if(!_vlcMediaPlayer)
        return;

    _vlcMediaPlayer->video()->setAspectRatio(_currentAspectRatio);
    _vlcMediaPlayer->video()->setCropGeometry(_currentCropRatio);
    _vlcMediaPlayer->video()->setScale(_currentScale);
    _vlcMediaPlayer->video()->setDeinterlace(_currentDeinterlacing);

    _enableSettings = false;*/
}

void VideoWidget::setAspectRatio(const Vlc::Ratio &ratio)
{
    /*if(_vlcMediaPlayer)
    {
        _currentAspectRatio = ratio;
        _vlcMediaPlayer->video()->setAspectRatio(ratio);
    }*/
}

void VideoWidget::setCropRatio(const Vlc::Ratio &ratio)
{
    /*if(_vlcMediaPlayer)
    {
        _currentCropRatio = ratio;
        _vlcMediaPlayer->video()->setCropGeometry(ratio);
    }*/
}

void VideoWidget::setDeinterlacing(const Vlc::Deinterlacing &deinterlacing)
{
   /* if(_vlcMediaPlayer)
    {
        _currentDeinterlacing = deinterlacing;
        _vlcMediaPlayer->video()->setDeinterlace(deinterlacing);
    }*/
}

void VideoWidget::setScale(const Vlc::Scale &scale)
{
   /* if(_vlcMediaPlayer)
    {
        _currentScale = scale;
        _vlcMediaPlayer->video()->setScale(scale);
    }*/
}

void VideoWidget::sync()
{
#if defined(Q_WS_X11)
    XSync(QX11Info::display(), False);
#endif
}

WId VideoWidget::request()
{
    /*if(_video)
        return 0;

    _video = new QWidget();
    QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::transparent);
    _video->setPalette(plt);
    _video->setAutoFillBackground(true);
    _video->setMouseTracking(true);
#ifndef Q_WS_X11
    _video->setAttribute( Qt::WA_PaintOnScreen, true );
#endif

    _layout->addWidget(_video);

    sync();
    return _video->winId();*/

    sync();
    return winId();
}

void VideoWidget::release()
{
    /*if(_video)
    {
        _layout->removeWidget(_video);
        _video->deleteLater();
        _video = NULL;
    }
    updateGeometry();*/
}
