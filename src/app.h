#ifndef APP_H
#define APP_H

#include <QApplication>
#include "settings.h"

class FilesList;
class MainWindow;
class IncrementSettingsWindow;

class App : public QApplication
{Q_OBJECT
public:
    App(int argc, char **argv);
    ~App();

    inline const Settings& getSettings() const {return settings;}

    int exec();

private slots:
    void setIncrementSettings(const Settings::IncrementSettings &settings);

private:
    FilesList *filesList;

    MainWindow *mainWindow;
    IncrementSettingsWindow *incrementSettingsWindow;

    Settings settings;
};

#define APP ((App*)qApp)

#endif // APP_H
