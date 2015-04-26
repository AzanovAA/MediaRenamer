#include "GUI/fileslistmodel.h"
#include "fileslist.h"
#include "app.h"

#include <QSize>

FilesListModel::FilesListModel(FilesList *filesList, QObject *parent)
    :QAbstractTableModel(parent)
{
    this->filesList = filesList;

    connect(this->filesList, SIGNAL(filesAdded(int,int)), SLOT(insertRows(int,int)));
    connect(this->filesList, &FilesList::reseted, this, [=](){beginResetModel(); endResetModel();});
    connect(this->filesList, &FilesList::changed, this, [=](){emit dataChanged(index(0, 0), index(rowCount()-1, 2));});
    connect(this->filesList, &FilesList::previewLoaded, this, [=](int index){emit dataChanged(this->index(index, 0), this->index(index, 0));});
}

int FilesListModel::rowCount(const QModelIndex &) const
{
    return filesList->getSize();
}

int FilesListModel::columnCount(const QModelIndex &) const
{
    return 3;
}

Qt::ItemFlags FilesListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    else if (index.column() == 1)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant FilesListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (section == 0)
            return "Original Name";
        else if (section == 1)
            return "New Name";
        else if (section == 2)
            return "Increment";
        else
            return QVariant();
    }
    else
        return QVariant();
}

QVariant FilesListModel::data(const QModelIndex &index, int role) const
{
    FileEntry &file = filesList->getFile(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == 0)
            return file.getOriginalName();
        else if (index.column() == 1)
            return file.getNewName();
        else if (index.column() == 2)
        {
            QString increment;
            increment.sprintf(("%0" + QString::number(APP->getSettings().incrementSettings.digitsCount) + "d" ).toLatin1().data(),
                              file.getIncrement() + APP->getSettings().incrementSettings.beginNumber - 1);

            return increment;
        }
        else
            return QVariant();
    }
    else if (role == Qt::ToolTipRole)
        return file.getFullPath();
    else
        return QVariant();
}

bool FilesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (value.isValid() && !value.toString().isEmpty() && (role == Qt::DisplayRole || role == Qt::EditRole))
    {
        filesList->setNewName(index.row(), value.toString());

        if (index.row() < (rowCount()-1))
            emit dataChanged(this->index(index.row()+1, 1), this->index(rowCount()-1, 2), QVector<int>() << Qt::DisplayRole << Qt::EditRole);

        return true;
    }
    else
        return false;
}

bool FilesListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();

    return true;
}

bool FilesListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    filesList->removeFile(row);
    endRemoveRows();

    return true;
}

void FilesListModel::copyDown(const QModelIndex &index)
{
    if (index.row() >= 0 && index.row() < rowCount())
    {
        filesList->copyDown(index.row());

        emit dataChanged(index, this->index(rowCount()-1, 2), QVector<int>() << Qt::DisplayRole << Qt::EditRole);
    }
}
