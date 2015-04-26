#ifndef JPEGTHUMBNAILFETCHER_H
#define JPEGTHUMBNAILFETCHER_H

#include <QIODevice>
#include <QImage>

class JpegThumbnailFetcher
{

public:
    JpegThumbnailFetcher() {}
    ~JpegThumbnailFetcher() {}

    static QImage fetchThumbnail(QIODevice &jpegFile);
    static QImage fetchThumbnail(QString filePath);

protected:
    static bool readWord( QIODevice &sdevice, unsigned short *target, bool invert=true );
    static bool exifScanloop( QIODevice &jpegFile, unsigned int &tnOffset, unsigned int &tnLength );

};

#endif // JPEGTHUMBNAILFETCHER_H
