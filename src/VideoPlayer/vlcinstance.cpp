#include <QtCore/QDebug>
#include <QtCore/QStringList>

#include <vlc/vlc.h>

#include "vlcconfig.h"
#include "vlcerror.h"
#include "vlcinstance.h"

VlcInstance::VlcInstance(const QStringList &args, QObject *parent)
    : QObject(parent), _vlcInstance(0)
{
#if defined(Q_OS_WIN32)
    char **argv = (char **)malloc(sizeof(char **) * args.count());
    for (int i = 0; i < args.count(); ++i)
        argv[i] = (char *)qstrdup(args.at(i).toUtf8().data());
#else
    char *argv[args.count()];
    for (int i = 0; i < args.count(); ++i)
        argv[i] = (char *)qstrdup(args.at(i).toUtf8().data());
#endif

    _vlcInstance = libvlc_new(args.count(), argv);

    qRegisterMetaType<Vlc::Meta>("Vlc::Meta");
    qRegisterMetaType<Vlc::State>("Vlc::State");

    VlcError::errmsg();

    if(!_vlcInstance)
    {
        qDebug() << "vlc failed to load!";
        abort();
    }
}

VlcInstance::~VlcInstance()
{
    libvlc_release(_vlcInstance);
}

libvlc_instance_t *VlcInstance::core()
{
    return _vlcInstance;
}

void VlcInstance::setUserAgent(const QString &application, const QString &version)
{
    QString applicationOutput = application + " " + version;
    QString httpOutput = application + "/" + version + " " + "VLC" + "/" + libvlc_get_version();
    libvlc_set_user_agent(_vlcInstance, applicationOutput.toUtf8().data(), httpOutput.toUtf8().data());
}
