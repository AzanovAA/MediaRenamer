#ifndef FILESLISTWIDGET_H
#define FILESLISTWIDGET_H

#include <QTableWidget>

class FilesList;

class FilesListWidget : public QTableView
{Q_OBJECT
public:
    FilesListWidget(FilesList *filesList, QWidget *parent = nullptr);

    bool event(QEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

signals:
    void fileSelected(int index);

public slots:
    void copyDown();
    void resizeSection(int section, int oldSize, int newSize);

protected:
    void mouseReleaseEvent(QMouseEvent *event);

    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void updateGeometries();

private:
    void selectRow(int row);
    void openEditor(const QModelIndex &index);
};

#endif // FILESLISTWIDGET_H
