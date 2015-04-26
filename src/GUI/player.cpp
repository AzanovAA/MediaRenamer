#include "player.h"
#include "videowidget.h"
#include "VideoPlayer/vlcmediaplayer.h"
#include "VideoPlayer/vlcvideo.h"
#include "VideoPlayer/vlcaudio.h"

#include <QtWidgets>

Player::Player(QWidget *parent)
    :QFrame(parent)
{
    player = new VlcMediaPlayer(Vlc::videoPlayerArgs(), this);
    videoWidget = new VideoWidget(player, this);
    progressSlider = new QSlider(Qt::Horizontal, this);
    playButton = new QPushButton(this);
    audioLabel = new QLabel(this);

    audioLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    audioLabel->setScaledContents(true);

#ifdef MACOSX
    audioLabel->setPixmap(QPixmap(qApp->applicationDirPath().append("/../Resources/audio_placeholder.jpg")));
#else
    audioLabel->setPixmap(QPixmap(qApp->applicationDirPath().append("/audio_placeholder.jpg")));
#endif

    audioLabel->adjustSize();
    audioLabel->hide();

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setMargin(0);
    layout->addWidget(videoWidget, 1, Qt::AlignHCenter | Qt::AlignTop);
    layout->addWidget(audioLabel, 1, Qt::AlignHCenter | Qt::AlignTop);
    layout->addWidget(initProgressFrame(), 0, Qt::AlignBottom);

    connect(player, &VlcMediaPlayer::lengthChanged, this, [=](int length){progressSlider->setRange(0, length);});
    connect(player, SIGNAL(timeChanged(int)), progressSlider, SLOT(setValue(int)));
    connect(progressSlider, SIGNAL(sliderMoved(int)), player, SLOT(setTime(int)));
}

void Player::open(const QString &filePath)
{
    connect(player, SIGNAL(playing()), SLOT(firstPause()));

    player->open(filePath);
    playButton->setChecked(false);
    progressSlider->setValue(0);

    layout()->itemAt(0)->widget()->show();
    layout()->itemAt(1)->widget()->hide();
    layout()->itemAt(2)->widget()->show();
}

void Player::openImage(const QString &filePath)
{
    connect(player, SIGNAL(playing()), SLOT(firstPause()));

    player->open(filePath);
    playButton->setChecked(false);
    progressSlider->setValue(0);

    layout()->itemAt(0)->widget()->show();
    layout()->itemAt(1)->widget()->hide();
    layout()->itemAt(2)->widget()->hide();
}

void Player::openAudio(const QString &filePath)
{
    connect(player, SIGNAL(playing()), SLOT(firstPause()));

    player->open(filePath);
    playButton->setChecked(false);
    progressSlider->setValue(0);

    layout()->itemAt(0)->widget()->hide();
    layout()->itemAt(1)->widget()->show();
    layout()->itemAt(2)->widget()->show();
}

void Player::stop()
{
    player->stop();
}

void Player::pause()
{
    player->pause();
}

void Player::togglePause()
{
    player->togglePause();
}

void Player::resizeEvent(QResizeEvent *event)
{
    resizeVideo(event->size());

    QFrame::resizeEvent(event);
}

QFrame* Player::initProgressFrame()
{
    QFrame *frame = new QFrame(this);
    QHBoxLayout *layout = new QHBoxLayout(frame);

    playButton->setCheckable(true);
    playButton->setStyleSheet("QPushButton {"
                              "background: transparent;"
                              "background-repeat: no-repeat;"
                              "background-image: url(:/resources/pause_button.png);"
                              "}"
                              "QPushButton:pressed {"
                              "background-image: url(:/resources/pause_button_pressed.png);"
                              "}"
                              "QPushButton:checked {"
                              "background-image: url(:/resources/play_button.png);"
                              "}"
                              "QPushButton:checked:pressed {"
                              "background-image: url(:/resources/play_button_pressed.png);"
                              "}");

    progressSlider->setStyleSheet("QSlider::groove {"
                                  "height: 10px;"
                                  "border-top: 1px solid rgb(18, 18, 18);"
                                  "border-left: 1px solid rgb(31, 31, 31);"
                                  "border-right: 1px solid rgb(31, 31, 31);"
                                  "border-bottom: 1px solid rgb(93, 93, 93);"
                                  "border-radius: 5px;"
                                  "background: rgb(39, 39, 39);"
                                  "}"
                                  "QSlider::handle {"
                                  "width: 17px;"
                                  "image: url(:/resources/slider_handle.png);"
                                  "margin: -9px 0;"
                                  "padding-left: -2px;"
                                  "padding-right: -2px;"
                                  "}");

    layout->addWidget(playButton);
    layout->addWidget(progressSlider);

    connect(playButton, SIGNAL(clicked()), SLOT(togglePause()));
    connect(player, &VlcMediaPlayer::playing, this, [=](){if (player->state() == Vlc::Playing) playButton->setChecked(false);});
    connect(player, &VlcMediaPlayer::paused, this, [=](){playButton->setChecked(true);});

    return frame;
}

void Player::resizeVideo(const QSize &size)
{
    if (progressSlider->isVisible())
    {
        videoWidget->setFixedSize(player->video()->size().scaled(size.width(), size.height() - layout()->itemAt(2)->widget()->height(), Qt::KeepAspectRatio));
        audioLabel->setFixedSize(audioLabel->size().scaled(size.width(), size.height() - layout()->itemAt(2)->widget()->height(), Qt::KeepAspectRatio));
    }
    else
        videoWidget->setFixedSize(player->video()->size().scaled(size, Qt::KeepAspectRatio));

    videoWidget->updateGeometry();
    audioLabel->updateGeometry();
}

void Player::firstPause()
{
    if (player->state() == Vlc::Playing && disconnect(player, SIGNAL(playing()), this, SLOT(firstPause())))
    {
        resizeVideo(size());

        player->setProperty("track", player->audio()->track());
        player->audio()->setTrack(0);

        connect(player, SIGNAL(paused()), SLOT(firstPaused()));
        player->pause();
    }
}

void Player::firstPaused()
{
    disconnect(player, SIGNAL(paused()), this, SLOT(firstPaused()));

    player->setTime(0);
    player->audio()->setTrack(player->property("track").toInt());
}
