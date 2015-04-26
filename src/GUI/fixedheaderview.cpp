#include "fixedheaderview.h"

#include <cmath>

#include <QResizeEvent>
#include <QPainter>
#include <QTime>

#include <QDebug>

FixedHeaderView::FixedHeaderView(Qt::Orientation orientation, QWidget *parent)
    :QHeaderView(orientation, parent)
{
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);

    buttonEvent = new CustomButtonEventer(this);
    viewport()->installEventFilter(buttonEvent);

    connect(this, SIGNAL(sectionCountChanged(int,int)), SLOT(updateStretch(int,int)));
    connect(this, &FixedHeaderView::newRect, this, [=](QRect result){ buttonEvent->setButtonRect(result); });
    connect(buttonEvent, SIGNAL(repaint()), this, SLOT(repaint()));
    connect(buttonEvent, &CustomButtonEventer::buttonClicked, this, [=](){ emit settingsButtonClicked(); });
}

void FixedHeaderView::mouseMoveEvent(QMouseEvent *event)
{
    int previosLength = length();
    QList<int> previosSizes;
    for (int i = 0; i < count(); ++i)
        previosSizes.append(sectionSize(i));

    QHeaderView::mouseMoveEvent(event);
    resizeSections();

    if (previosLength < length())
    {
        for (int i = 0; i < count(); ++i)
        {
            if (sectionSize(i) > previosSizes.at(i))
            {
                previosSizes.removeAt(i);
                previosSizes.removeLast();

                int size = 0;
                for (int y: previosSizes)
                    size += y;

                resizeSection(i, previosLength - size - minimumSectionSize());

                break;
            }
        }
    }
}

void FixedHeaderView::resizeEvent(QResizeEvent *event)
{
    double factor = (double)event->size().width() / (double)event->oldSize().width();

    if (factor > 0)
    {
        double additionalSize = 0;
        for (int i = count() - 1; i >= 0; --i)
        {
            int newSize = std::round((sectionSize(i) * factor) - additionalSize);

            if (newSize > minimumSectionSize())
                resizeSection(i, newSize);
            else
            {
                resizeSection(i, minimumSectionSize());

                if (i > 0)
                    additionalSize += (minimumSectionSize() - newSize + 2) / (double)i;
            }
        }
    }

    QHeaderView::resizeEvent(event);
}

void FixedHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();

    if(logicalIndex == 2)
    {
        static QPixmap settingsButton(":/resources/config_button.png");
        static QPixmap settingsButtonHovered(":/resources/config_button_hovered.png");
        static QPixmap settingsButtonPressed(":/resources/config_button_pressed.png");
        QStyleOptionButton option;

        QRect settingsRect = settingsButtonRect(rect);
        emit newRect(settingsRect);

        style()->drawItemPixmap(painter, settingsRect, 0, settingsButton);

        if(settingsRect.contains(qobject_cast<QWidget*>(parent())->mapFromGlobal(QCursor::pos())))
        {
            switch(buttonEvent->getButtonState())
            {
            case Hovered:
                style()->drawItemPixmap(painter, settingsRect, 0, settingsButtonHovered);
                break;

            case Pressed:
                style()->drawItemPixmap(painter, settingsRect, 0, settingsButtonPressed);
                break;

            default:
                style()->drawItemPixmap(painter, settingsRect, 0, settingsButton);
                break;
            }
        }
        else
            style()->drawItemPixmap(painter, settingsRect, 0, settingsButton);

    }
}

void FixedHeaderView::updateStretch(int oldCount, int newCount)
{
    if (newCount > 0)
    {
        if (oldCount > 0)
            setSectionResizeMode(oldCount - 1, QHeaderView::Interactive);

        setSectionResizeMode(newCount - 1, QHeaderView::Stretch);
    }
}

QRect FixedHeaderView::settingsButtonRect(const QRect &sourceRect) const
{
    static QSize size(16, 17);

    QPoint checkBoxPoint(sourceRect.x() + sourceRect.width()/2 -
                         size.width() / 2 + 38,
                         sourceRect.y() + sourceRect.height() / 2 -
                         size.height()/2 + 1
                         );
    return QRect(checkBoxPoint, size);
}
