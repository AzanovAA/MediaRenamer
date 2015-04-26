#ifndef FILESLISTMODEL_H
#define FILESLISTMODEL_H

#include <QAbstractTableModel>

class FilesList;

class FilesListModel : public QAbstractTableModel
{Q_OBJECT
public:
    FilesListModel(FilesList *filesList, QObject *parent = nullptr);

    inline FilesList* getFilesList() const { return filesList; }

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public slots:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    void copyDown(const QModelIndex &index);

private:
    FilesList *filesList;
};

#endif // FILESLISTMODEL_H
