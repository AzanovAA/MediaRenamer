#include "app.h"
#include "GUI/mainwindow.h"
#include "GUI/incrementsettingswindow.h"
#include "fileslist.h"

#include <QtCore>

extern "C" {
#include <libavformat/avformat.h>
}

#define SAVE_INTERVAL (5 * 60 * 1000)

App::App(int argc, char **argv)
    :QApplication(argc, argv)
{
#ifdef MACOSX
    setenv("VLC_PLUGIN_PATH", qApp->applicationDirPath().append("/PlugIns").toLatin1(), 1);
#endif

    av_register_all();

    filesList = new FilesList();
    mainWindow = new MainWindow(filesList);
    incrementSettingsWindow = new IncrementSettingsWindow();

    QTimer *saveTimer = new QTimer(this);

    connect(saveTimer, SIGNAL(timeout()), filesList, SLOT(save()));
    saveTimer->start(SAVE_INTERVAL);

    connect(mainWindow, SIGNAL(incrementSettingsClicked()), incrementSettingsWindow, SLOT(exec()));
    connect(incrementSettingsWindow, SIGNAL(settingsChanged(Settings::IncrementSettings)), SLOT(setIncrementSettings(Settings::IncrementSettings)));
}

App::~App()
{
    delete mainWindow;
    delete incrementSettingsWindow;
    delete filesList;
}

int App::exec()
{
    mainWindow->show();

    return QApplication::exec();
}

void App::setIncrementSettings(const Settings::IncrementSettings &settings)
{
    this->settings.incrementSettings = settings;
}
