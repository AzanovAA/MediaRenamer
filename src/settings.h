#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
public:
    class IncrementSettings
    {
    public:
        IncrementSettings():beginNumber(1), digitsCount(3), place(After), separator(' ') {}

        enum Place
        {
            After,
            Befor
        };

        int beginNumber;
        int digitsCount;
        Place place;
        QString separator;
    }incrementSettings;
};

#endif // SETTINGS_H
