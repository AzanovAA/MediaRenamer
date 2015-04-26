#ifndef PLAYER_H
#define PLAYER_H

#include <QFrame>

class VlcMediaPlayer;
class VideoWidget;
class QSlider;
class QPushButton;
class QLabel;

class Player : public QFrame
{Q_OBJECT
public:
    Player(QWidget *parent = nullptr);

public slots:
    void open(const QString &filePath);
    void openImage(const QString &filePath);
    void openAudio(const QString &filePath);

    void stop();
    void pause();
    void togglePause();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QFrame* initProgressFrame();

private slots:
    void resizeVideo(const QSize &size);

    void firstPause();
    void firstPaused();

private:
    VlcMediaPlayer *player;
    VideoWidget *videoWidget;
    QLabel *audioLabel;

    QSlider *progressSlider;
    QPushButton *playButton;
};

#endif // PLAYER_H
