#include "fileslist.h"
#include "app.h"

#include <QtCore>
#include <QStandardPaths>
#include <QImage>
#include <QImageReader>

#include "VideoHandler/videodata.h"
#include "VideoHandler/videohandler.h"
#include "Utils/jpegthumbnailfetcher.h"

#define USER_DATA_FOLDER (QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/Visual Renamer")
#define SAVE_FILE (USER_DATA_FOLDER + "/saved_data")

#define KNOWN_FORMATS {"mxf"}

//QStringList renameLogs_;

FileEntry::FileEntry()
{
    reset();
}

FileEntry::FileEntry(const QString &filePath)
    :FileEntry(QFileInfo(filePath))
{
}

FileEntry::FileEntry(const QFileInfo &fileInfo)
    :file(fileInfo)
{
    reset();
    initType();
    //initPreview();
}

FileEntry::FileEntry(const FileEntry &other)
{
    type = other.getType();
    preview = other.getPreview();
    file = other.file;
    newName = other.getNewName();
    increment = other.increment;
}

FileEntry& FileEntry::operator= (const FileEntry &other)
{
    type = other.getType();
    preview = other.getPreview();
    file = other.file;
    newName = other.getNewName();
    increment = other.increment;

    return *this;
}

bool FileEntry::operator== (const FileEntry &other) const
{
    return file == other.file;
}

bool FileEntry::rename()
{
    if (APP->getSettings().incrementSettings.digitsCount == 0)// && QFile::rename(file.absoluteFilePath(), file.absolutePath() + QDir::separator() + QString("%1.").arg(newName) + file.completeSuffix()))
    {
        QString log = "Renamed from: " + file.absoluteFilePath() + " to " + file.absolutePath() + QDir::separator() + QString("%1.").arg(newName) + file.completeSuffix();
        bool bRes = QFile::rename(file.absoluteFilePath(), file.absolutePath() + QDir::separator() + QString("%1.").arg(newName) + file.completeSuffix());

        if (bRes) log += " Ok";
        else log += " Failed";
        //renameLogs_ << log;

        return bRes;
    }
    else
    {
        QString name;
        name.sprintf(("%0" + QString::number(APP->getSettings().incrementSettings.digitsCount) + "d" ).toLatin1().data(),
                     getIncrement() + APP->getSettings().incrementSettings.beginNumber - 1);

        switch (APP->getSettings().incrementSettings.place)
        {
        case Settings::IncrementSettings::After:
            name = newName + APP->getSettings().incrementSettings.separator + name + "." + file.suffix();
            break;

        case Settings::IncrementSettings::Befor:
            name = name + APP->getSettings().incrementSettings.separator + newName + "." + file.suffix();
            break;

        default:
            name = newName + APP->getSettings().incrementSettings.separator + name + "." + file.suffix();
            break;
        }

        QString log = "Renamed from: " + file.absoluteFilePath() + " to " + file.absolutePath() + QDir::separator() + name;
        bool bRes = QFile::rename(file.absoluteFilePath(), file.absolutePath() + QDir::separator() + name);

        if (bRes) log += " Ok";
        else log += " Failed";
        //renameLogs_ << log;

        return bRes;
    }
}

void FileEntry::reset()
{
    newName = file.completeBaseName();
    increment = 1;
    type = None;
}

void FileEntry::setPreview(const QImage &image)
{
    QMutexLocker locker(&previewMutex);

    preview = image;
}

const QImage& FileEntry::getPreview() const
{
    QMutexLocker locker(&previewMutex);

    return preview;
}

void FileEntry::initType()
{
    static QMimeDatabase mimeDB;
    QMimeType mimeType = mimeDB.mimeTypeForFile(file);

    bool isVideo = mimeType.name().contains("video");

    if (!isVideo)
    {
        for (const QString &format: mimeType.suffixes())
        {
            for (const char *knownFormat: KNOWN_FORMATS)
            {
                if (format == knownFormat)
                {
                    isVideo = true;
                    break;
                }
            }
        }
    }

    if (isVideo)
        type = Video;
    else if (mimeType.name().contains("audio") && !mimeType.name().contains("x-mpegurl"))
        type = Audio;
    else if (mimeType.name().contains("image") && !mimeType.name().contains("photoshop") && !mimeType.name().contains("x-icns"))
        type = Image;
    else
        type = File;
}

/*********************************FilesList*********************************/
FilesList::FilesList(QObject *parent)
    :QThread(parent), bFinish_(false)
{
    autoRenameState = false;

    resetUndo();
}

FilesList::~FilesList()
{
    bFinish_ = true;
    wait();
}

void FilesList::addFiles(const QStringList &files)
{
    if (!files.isEmpty())
    {
        int previousSize = this->files.size();

        filesLock.lock();
        {
            for (const QString &file: files)
            {
                FileEntry entry(file);

                if (!this->files.contains(entry))
                    this->files.append(entry);
            }
        }
        filesLock.unlock();

        if (previousSize != this->files.size())
        {
            updateIncrements();

            emit filesAdded(previousSize, this->files.size() - previousSize);
        }

        start();
    }
}

void FilesList::addFolder(const QString &folder)
{
    if (!folder.isEmpty())
    {
        QStringList files = QDir(folder).entryList(QDir::Files);

        for (QString &file: files)
            file = folder + QDir::separator() + file;

        addFiles(files);
    }
}

void FilesList::removeFile(int index)
{
    FileEntry &file = getFile(index);

    for (UndoStack::iterator it = undoStack.end() - 1; it != undoStack.begin();)
    {
        if (it->contains(file.getFullPath()))
        {
            it->remove(file.getFullPath());

            if (it->isEmpty())
            {
                if (it == currentUndo)
                    --currentUndo;

                undoStack.erase(it--);
            }
            else
                --it;
        }
        else
            --it;
    }

    if (undoStack.isEmpty())
        resetUndo();

    filesLock.lock();
    files.removeAt(index);
    filesLock.unlock();

    updateIncrements();
    emit removed();
}

void FilesList::renameFiles()
{
    if (files.isEmpty())
        return;

    //renameLogs_.clear();

#ifndef MACOSX
    emit reseted();
#endif

    int renameCount = 0, size = files.size();

    renameLock.lock();
    {
        for (FileEntry &file: files)
        {
            if (file.rename())
                ++renameCount;
        }

    }

    reset();
    renameLock.unlock();

    emit renamed(size, renameCount);
}

void FilesList::reset()
{
    filesLock.lock();
    files.clear();
    filesLock.unlock();

    resetUndo();

    emit reseted();
}

void FilesList::copyDown(int from)
{
    UndoStack::iterator previosUndo = currentUndo;

    addUndo();

    QList<FileEntry>::iterator fromIt = files.begin() + from;
    for (QList<FileEntry>::iterator it = fromIt + 1; it != files.end(); ++it)
    {
        (*previosUndo)[it->getFullPath()] = it->getNewName();

        it->setNewName(fromIt->getNewName());

        (*currentUndo)[it->getFullPath()] = it->getNewName();
    }

    updateIncrements();
}

void FilesList::setAutoRename(bool state)
{
    autoRenameState = state;
}

void FilesList::setNewName(int index, const QString &name)
{
    FileEntry &file = getFile(index);

    if (name != file.getNewName())
    {
        (*currentUndo)[file.getFullPath()] = file.getNewName();

        file.setNewName(name);

        if (autoRenameState)
            copyDown(index);
        else
        {
            updateIncrements();
            addUndo();
        }

        (*currentUndo)[file.getFullPath()] = file.getNewName();
    }
}

void FilesList::save()
{
    QDir().mkdir(USER_DATA_FOLDER);

    QFile file(SAVE_FILE);
    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        for (const FileEntry &entry: files)
            file.write(QByteArray().append(entry.getFullPath() + ";" + entry.getNewName() + "\n"));

        file.close();
    }
}

void FilesList::load()
{
    filesLock.lock();
    {
        files.clear();

        resetUndo();

        QFile file(SAVE_FILE);
        if (file.open(QFile::ReadOnly))
        {
            QList<QByteArray> savedData = file.readAll().split('\n');

            for (const QByteArray &savedEntry: savedData)
            {
                QFileInfo fileInfo(savedEntry.split(';').first());
                if (fileInfo.exists())
                {
                    FileEntry entry(fileInfo);
                    entry.setNewName(savedEntry.split(';').last());

                    files.append(entry);
                }
            }

            file.close();
        }
    }
    filesLock.unlock();

    start();
    updateIncrements();

    emit reseted();
}

void FilesList::undo()
{
    if (currentUndo != undoStack.begin() && currentUndo != undoStack.end())
    {
        --currentUndo;

        for (FileEntry &file: files)
        {
            if (currentUndo->contains(file.getFullPath()))
                file.setNewName((*currentUndo)[file.getFullPath()]);
        }

        updateIncrements();

        emit changed();
    }
}

void FilesList::redo()
{
    if (currentUndo != undoStack.end() && currentUndo + 1 != undoStack.end())
    {
        ++currentUndo;

        for (FileEntry &file: files)
        {
            if (currentUndo->contains(file.getFullPath()))
                file.setNewName((*currentUndo)[file.getFullPath()]);
        }

        updateIncrements();

        emit changed();
    }
}

void FilesList::updateIncrements()
{
    QMap<QString, int> lastNamesIncrements;

    for (FileEntry &file: files)
    {
        if (lastNamesIncrements.contains(file.getNewName()))
            file.setIncrement(++lastNamesIncrements[file.getNewName()]);
        else
        {
            lastNamesIncrements[file.getNewName()] = 1;
            file.setIncrement(1);
        }
    }
}

void FilesList::addUndo()
{
    if (currentUndo != undoStack.end())
        undoStack.erase(currentUndo + 1, undoStack.end());

    undoStack.append(QMap<QString, QString>());
    currentUndo = undoStack.end() - 1;
}

void FilesList::resetUndo()
{
    undoStack.clear();
    currentUndo = undoStack.end();

    addUndo();
}

void FilesList::run()
{
    try
    {
        while (!bFinish_)
        {
            FileEntry emptyFile;
            JpegThumbnailFetcher fetcher;
            QImage img;
            QImageReader imageReader;

            filesLock.lock();
            {
                for (FileEntry &file: files)
                {
                    if (file.getPreview().isNull())
                    {
                        emptyFile = file;
                        break;
                    }
                }
            }
            filesLock.unlock();

            if (emptyFile.getType() == FileEntry::None)
                break;

            renameLock.lock();
            {
                if(QFile::exists(emptyFile.getFullPath()))
                {
                    switch (emptyFile.getType())
                    {
                    case FileEntry::Video:
                    {
                        VideoData data(emptyFile.getFullPath().toUtf8().constData());
                        VideoHandler handler;

                        handler.initPreview(data);

                        Preview preview = data.getPreview();
                        AVFrame *frameRGB = preview.data;

                        QImage image = QImage(preview.width, preview.height, QImage::Format_RGB32);

                        uint8_t *src = (uint8_t *)(frameRGB->data[0]);
                        for (int y = 0; y < preview.height; y++)
                        {
                            QRgb *scanLine = (QRgb *)image.scanLine(y);
                            for (int x = 0; x < preview.width; x++)
                                scanLine[x] = qRgb(src[3*x], src[3*x+1], src[3*x+2]);

                            src += frameRGB->linesize[0];
                        }

                        emptyFile.setPreview(image.scaled(PREVIEW_SIZE));

                        break;
                    }

                    case FileEntry::Image:
                        imageReader.setFileName(emptyFile.getFullPath());
                        imageReader.setAutoDetectImageFormat( true );
                        imageReader.setScaledSize(PREVIEW_SIZE);
                        imageReader.setQuality(10);
                        emptyFile.setPreview(imageReader.read());
                        //emptyFile.setPreview(QImage(emptyFile.getFullPath()).scaled(PREVIEW_SIZE));
                        break;

                    case FileEntry::Audio:
                        static QImage audioImage = QImage(":/resources/audio_thumbnail.png").scaled(QSize(40,40)/*PREVIEW_SIZE*/, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                        emptyFile.setPreview(audioImage);
                        break;
                    default:
                        static QImage defaultImage = QImage(":/resources/document_thumbnail.png").scaled(QSize(40,40)/*PREVIEW_SIZE*/, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                        emptyFile.setPreview(defaultImage);
                        break;
                    }
                }
            }
            renameLock.unlock();

            filesLock.lock();
            {
                int index = files.indexOf(emptyFile);
                if (index >= 0)
                {
                    files[index].setPreview(emptyFile.getPreview());

                    emit previewLoaded(index);
                }
            }
            filesLock.unlock();
        }
    }
    catch (std::exception &ex)
    {
        qDebug() << "\n\n\nLOOOL\n" << ex.what() << "\n\n\n\n";
    }
}
