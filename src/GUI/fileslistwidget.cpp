#include "GUI/fileslistwidget.h"
#include "GUI/fileslistmodel.h"
#include "GUI/filepreview.h"

#include <QtWidgets>

FilesListWidget::FilesListWidget(FilesList *filesList, QWidget *parent)
    :QTableView(parent)
{
    FilePreview *filePreview = new FilePreview(this);

    setModel(new FilesListModel(filesList, this));
    setItemDelegateForColumn(0, filePreview);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QTableWidget::SingleSelection);
    setSelectionBehavior(QTableView::SelectRows);
    setTabKeyNavigation(false);
    setStyleSheet("QTableView {"
                  "border: none;"
                  "selection-background-color: rgb(235, 235, 235);"
                  "selection-color: rgb(58, 58, 58);"
                  "}"
                  "QWidget {"
                  "font: bold 11px \"Arial\";"
                  "color: rgb(58, 58, 58);"
                  "}");

    horizontalHeader()->setFixedHeight(30);
    horizontalHeader()->setMinimumSectionSize(150);
    horizontalHeader()->resizeSection(0, 250);
    horizontalHeader()->resizeSection(1, 400);
    horizontalHeader()->hide();

    verticalHeader()->hide();
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(35);

    setFixedHeight(viewportSizeHint().height());

    connect(filePreview, &FilePreview::removeButtonClicked, this, [=](const QModelIndex &index){model()->removeRow(index.row());});
    connect(filePreview, SIGNAL(updated(QModelIndex)), SLOT(update(QModelIndex)));
}

bool FilesListWidget::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = (QKeyEvent*)event;

        if (keyEvent->key() == Qt::Key_Tab)
        {
            if (currentIndex().row()+1 < model()->rowCount())
                selectRow(currentIndex().row()+1);

            return true;
        }
        else if (keyEvent->key() == Qt::Key_Backtab)
        {
            if (currentIndex().row()-1 >= 0)
                selectRow(currentIndex().row()-1);

            return true;
        }
        else if (keyEvent->key() == Qt::Key_Space)
        {
            event->ignore();

            return true;
        }
        else if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            openEditor(currentIndex());
    }

    return QTableView::event(event);
}

bool FilesListWidget::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QKeyEvent::KeyPress)
    {
        QKeyEvent *keyEvent = (QKeyEvent*)event;

        if (keyEvent->key() == Qt::Key_Tab)
        {
            if (currentIndex().row()+1 < model()->rowCount())
                selectRow(currentIndex().row()+1);

            return true;
        }
        else if (keyEvent->key() == Qt::Key_Backtab)
        {
            if (currentIndex().row()-1 >= 0)
                selectRow(currentIndex().row()-1);

            return true;
        }
        else if (keyEvent->key() == Qt::Key_Up)
        {
            qobject_cast<QLineEdit*>(object)->home(false);

            return true;
        }
        else if (keyEvent->key() == Qt::Key_Down)
        {
            qobject_cast<QLineEdit*>(object)->end(false);

            return true;
        }
        else if (keyEvent->matches(QKeySequence::Undo) || keyEvent->matches(QKeySequence::Redo))
        {
            event->ignore();

            return true;
        }
    }
    else if (event->type() == QEvent::FocusOut)
        object->deleteLater();

    return QTableView::eventFilter(object, event);
}

void FilesListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QTableView::mouseReleaseEvent(event);

    QModelIndex index = indexAt(event->pos());

    if (index.column() == 1)
        openEditor(index);
}

void FilesListWidget::copyDown()
{
    qobject_cast<FilesListModel*>(model())->copyDown(currentIndex());
}

void FilesListWidget::resizeSection(int section, int /* oldSize */, int newSize)
{
    setColumnWidth(section, newSize);
}

void FilesListWidget::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (current.row() != previous.row())
        emit fileSelected(current.row());

    if (current.column() == 1)
        QTableView::currentChanged(current, previous);
    else
        setCurrentIndex(model()->index(current.row(), 1));
}

void FilesListWidget::updateGeometries()
{
    setFixedHeight(viewportSizeHint().height());

    QTableView::updateGeometries();
}

void FilesListWidget::selectRow(int row)
{
    QTableView::selectRow(row);
    setCurrentIndex(model()->index(row, 1));
    openEditor(currentIndex());
}

void FilesListWidget::openEditor(const QModelIndex &index)
{
    if (index.isValid())
    {
        openPersistentEditor(index);

        QLineEdit *edit = qobject_cast<QLineEdit*>(indexWidget(index));
        edit->setFocus();
        edit->selectAll();
        edit->installEventFilter(this);

        connect(edit, &QLineEdit::textChanged, this, [=](const QString &text){model()->setData(index, text);});
    }
}
