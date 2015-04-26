#ifndef CUSTOMBUTTONEVENTER_H
#define CUSTOMBUTTONEVENTER_H

#include <QEvent>
#include <QMouseEvent>
#include <QTime>

#include <QDebug>

enum ButtonState
{
    None,
    Hovered,
    Pressed
};

class CustomButtonEventer : public QObject
{Q_OBJECT
public:
   CustomButtonEventer(QObject * parent) : QObject(parent){}

   ButtonState getButtonState() const { return _buttonState; }
   void setButtonRect(const QRect &settingsButtonRect) { _buttonRect = settingsButtonRect; }

signals:
   void repaint();
   void buttonClicked();

protected:
   bool eventFilter(QObject *object, QEvent *event)
   {
       switch (event->type())
       {
       case QEvent::MouseMove:
       {
           if(_buttonRect.contains(((QMouseEvent*)event)->pos()))
           {
               if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
               {
                   _buttonState = Pressed;
                   emit repaint();
               }
               else if(_buttonState == None)
               {
                   _buttonState = Hovered;
                   emit repaint();

               }
           }
           else if(_buttonState != None)
           {
               _buttonState = None;
               emit repaint();
           }

           break;
       }

       case QEvent::MouseButtonPress:
       {
           if(_buttonRect.contains(((QMouseEvent*)event)->pos()))
           {
               if(_buttonState != Pressed)
               {
                   _buttonState = Pressed;
                   emit repaint();
               }
           }

           break;
       }

       case QEvent::MouseButtonRelease:
       {
           if(((QMouseEvent*)event)->button() == Qt::LeftButton && _buttonRect.contains(((QMouseEvent*)event)->pos()))
           {
               static QTime lastClick;

               if(lastClick.addMSecs(25) < QTime::currentTime())
               {
                   lastClick = QTime::currentTime();
                   _buttonState = Hovered;

                   emit buttonClicked();
               }
           }

           break;
       }

       case QEvent::Leave:
       {
            if(_buttonState != None)
            {
                _buttonState = None;
                emit repaint();
            }

            break;
       }

       default:
           break;
       }

       return QObject::eventFilter(object, event);
   }

private:
   QRect _buttonRect;
   ButtonState _buttonState;
};

#endif // CUSTOMBUTTONEVENTER_H
