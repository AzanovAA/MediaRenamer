#ifndef FILESLIST_H
#define FILESLIST_H

#include <QThread>
#include <QFileInfo>
#include <QImage>
#include <QMap>
#include <QLinkedList>
#include <QMutex>
#include <QStringList>

class FileEntry
{
public:
    enum Type
    {
        None,
        Video,
        Audio,
        Image,
        File
    };

    FileEntry();
    FileEntry(const QString &filePath);
    FileEntry(const QFileInfo &fileInfo);
    FileEntry(const FileEntry &other);

    FileEntry& operator= (const FileEntry &other);
    bool operator== (const FileEntry &other) const;

    bool rename();
    void reset();

    inline Type getType() const { return type; }

    inline QString getFullPath() const { return file.absoluteFilePath(); }
    inline QString getOriginalName() const { return file.fileName(); }

    inline void setNewName(const QString &name) { newName = name; }
    inline const QString& getNewName() const { return newName; }

    inline void setIncrement(int i) { increment = i; }
    inline int getIncrement() const { return increment; }

    void setPreview(const QImage &image);
    const QImage& getPreview() const;

private:
    void initType();

private:
    Type type;

    QImage preview;
    mutable QMutex previewMutex;

    QFileInfo file;
    QString newName;
    int increment;
};

class FilesList : public QThread
{Q_OBJECT
    typedef QLinkedList<QMap<QString, QString>> UndoStack;

public:
    FilesList(QObject *parent = nullptr);
    ~FilesList();

    inline bool isAutoRename() const { return autoRenameState; }
    inline int getSize() const { return files.size(); }

    inline FileEntry& getFile(int index) { return files[index]; }
    inline const FileEntry& getFile(int index) const { return files[index]; }

public slots:
    void addFiles(const QStringList &files);
    void addFolder(const QString &folder);
    void removeFile(int index);

    void renameFiles();

    void reset();

    void copyDown(int from);
    void setAutoRename(bool state);
    void setNewName(int index, const QString &name);

    void save();
    void load();

    void undo();
    void redo();

signals:
    void filesAdded(int from, int count);
    void reseted();
    void removed();
    void changed();
    void previewLoaded(int index);
    void renamed(int count, int successCount);

protected:
    void run();

private:
    void updateIncrements();

    void addUndo();
    void resetUndo();

private:
    QList<FileEntry> files;
    QMutex filesLock;
    QMutex renameLock;

    bool autoRenameState;
    bool bFinish_;

    UndoStack undoStack;
    UndoStack::iterator currentUndo;

};

#endif // FILESLIST_H
