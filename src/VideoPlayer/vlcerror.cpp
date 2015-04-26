#include <QtCore/QDebug>

#include <vlc/vlc.h>

#include "vlcerror.h"

void VlcError::errmsg()
{
    if(libvlc_errmsg())
    {
        qDebug() << "libvlc" << "Error:" << libvlc_errmsg();
		clearerr();
	}
}

void VlcError::clearerr()
{
    libvlc_clearerr();
}
