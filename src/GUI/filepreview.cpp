#include "GUI/filepreview.h"
#include "GUI/fileslistmodel.h"
#include "fileslist.h"

#include <QtWidgets>

#define REMOVE_BUTTON_RECT(opt) QRect(option.rect.right()-30, option.rect.top(), option.rect.right() - (option.rect.right()-20), option.rect.height())

FilePreview::FilePreview(QObject *parent)
    :QItemDelegate(parent)
{
    removeButtonState = None;
}

void FilePreview::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    static QPixmap removeButton(":/resources/remove_button.png");
    static QPixmap removeButtonHovered(":/resources/remove_button_hovered.png");
    static QPixmap removeButtonPressed(":/resources/remove_button_pressed.png");

    QStyleOptionViewItem opt = setOptions(index, option);
    QRect removeButtonRect = REMOVE_BUTTON_RECT(opt);
    const FileEntry &file = qobject_cast<const FilesListModel*>(index.model())->getFilesList()->getFile(index.row());

    painter->save();

    drawBackground(painter, opt, index);
    drawDecoration(painter, opt, QRect(QPoint(20, opt.rect.top()+5), PREVIEW_SIZE), QPixmap::fromImage(file.getPreview()));
    drawDisplay(painter, opt, QRect(60, opt.rect.top(), opt.rect.width()-100, opt.rect.height()), file.getOriginalName());

    if (removeButtonRect.contains(qobject_cast<QWidget*>(parent())->mapFromGlobal(QCursor::pos())))
    {
        switch (removeButtonState)
        {
        case Hovered:
            drawDecoration(painter, opt, removeButtonRect, removeButtonHovered);
            break;

        case Pressed:
            drawDecoration(painter, opt, removeButtonRect, removeButtonPressed);
            break;

        default:
            drawDecoration(painter, opt, removeButtonRect, removeButton);
            break;
        }
    }
    else
        drawDecoration(painter, opt, removeButtonRect, removeButton);

    painter->restore();
}

bool FilePreview::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QRect removeButtonRect = REMOVE_BUTTON_RECT(opt);

    switch (event->type())
    {
    case QEvent::MouseMove:
        if (removeButtonRect.contains(((QMouseEvent*)event)->pos()))
        {
            if (((QMouseEvent*)event)->buttons() & Qt::LeftButton)
            {
                removeButtonState = Pressed;
                emit updated(index);
            }
            else if (removeButtonState == None)
            {
                removeButtonState = Hovered;
                emit updated(index);
            }
        }
        else if (removeButtonState != None)
        {
            removeButtonState = None;
            emit updated(index);
        }

        break;

    case QEvent::MouseButtonPress:
        if (removeButtonRect.contains(((QMouseEvent*)event)->pos()))
        {
            if (removeButtonState != Pressed)
            {
                removeButtonState = Pressed;
                emit updated(index);
            }
        }

        break;

    case QEvent::MouseButtonRelease:
    {
        if (((QMouseEvent*)event)->button() == Qt::LeftButton && removeButtonRect.contains(((QMouseEvent*)event)->pos()))
        {
            static QTime lastClick;

            if (lastClick.addMSecs(25) < QTime::currentTime())
            {
                lastClick = QTime::currentTime();
                removeButtonState = None;

                emit removeButtonClicked(index);
            }
        }

        break;
    }

    default:
        break;
    }

    return QItemDelegate::editorEvent(event, model, option, index);
}
