#ifndef VLCINSTANCE_H
#define VLCINSTANCE_H

#include <QtCore/QObject>

struct libvlc_instance_t;

class VlcInstance : public QObject
{
Q_OBJECT
public:
    explicit VlcInstance(const QStringList &args, QObject *parent = NULL);
    ~VlcInstance();
    libvlc_instance_t *core();

    void setUserAgent(const QString &application, const QString &version);

private:
    libvlc_instance_t *_vlcInstance;
};

#endif // VLCQT_VLCINSTANCE_H_
