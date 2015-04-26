#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QFrame;
class QAbstractButton;
class FilesList;
class FilesListWidget;
class QLabel;
class Player;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(FilesList *filesList);
    ~MainWindow();

    bool eventFilter(QObject *object, QEvent *event);

signals:
    void incrementSettingsClicked();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void fileSelected(int index);
    void filesRenamed(int count, int successCount);
    void showAbout();
    void showLog();

private:
    QFrame* initVideoFrame();
    QFrame* initControlFrame();
    QFrame* initFilesFrame(QWidget *parent);
    QFrame* initRenameFrame(QWidget *parent);
    QFrame* initFilesListFrame(QWidget *parent);
    QFrame* initStatusBarFrame(QWidget *parent);
    QFrame* initDragFrame(QWidget *parent);
    //void initSettingsFrame();

    void initMenu();

    QAbstractButton* createIconButton(const QString &iconPath, const QString &text, QWidget *parent) const;

private:
    Player *player;

    FilesList *filesList;
    FilesListWidget *filesListWidget;

    QLabel *filesCountLabel;
};

#endif // MAINWINDOW_H
