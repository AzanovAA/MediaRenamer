#ifndef INCREMENTSETTINGSWINDOW_H
#define INCREMENTSETTINGSWINDOW_H

#include <QDialog>
#include "settings.h"

class QLabel;
class QFrame;

class IncrementSettingsWindow : public QDialog
{Q_OBJECT
public:
    IncrementSettingsWindow();

    inline Settings::IncrementSettings getSettings() const {return settings;}

signals:
    void settingsChanged(const Settings::IncrementSettings &settings);

private:
    void initInterface();
    QFrame* initSettingsFrame();

    void updatePreview();

private slots:
    void beginNumberChanged(const QString &number);
    void digitsCountChanged(const QString &count);
    void incrementPlaceChanged(const QString &place);
    void separatorChanged(const QString &separator);

private:
    Settings::IncrementSettings settings;

    QLabel *previewLabel;
};

#endif // INCREMENTSETTINGSWINDOW_H
