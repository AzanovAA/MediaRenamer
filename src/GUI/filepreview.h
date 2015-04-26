#ifndef FILEPREVIEW_H
#define FILEPREVIEW_H

#include <QItemDelegate>

class FilePreview : public QItemDelegate
{Q_OBJECT
    enum ButtonState
    {
        None,
        Hovered,
        Pressed
    };

public:
    FilePreview(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void removeButtonClicked(const QModelIndex &index);
    void updated(const QModelIndex &index);

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    ButtonState removeButtonState;
};

#endif // FILEPREVIEW_H
