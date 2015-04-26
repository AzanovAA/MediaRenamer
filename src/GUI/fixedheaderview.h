#ifndef FIXEDHEADERVIEW_H
#define FIXEDHEADERVIEW_H

#include <QHeaderView>
#include "custombuttoneventer.h"

class FixedHeaderView : public QHeaderView
{Q_OBJECT
public:
    FixedHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

private slots:
    void updateStretch(int oldCount, int newCount);

private:
    QRect settingsButtonRect(const QRect &sourceRect) const;

private:
    CustomButtonEventer *buttonEvent;
    
signals:
    void newRect(QRect) const;
    void settingsButtonClicked();
};


#endif // FIXEDHEADERVIEW_H
