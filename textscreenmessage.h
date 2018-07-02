#ifndef TEXTSCREENMESSAGE_H
#define TEXTSCREENMESSAGE_H

#include <QMap>
#include <QDebug>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

/*! Class responsible for displaying result of game */

class TextScreenMessage : public QGraphicsItem
{
private:
    QMap <QString,int> text_states;
    int current_text_state;
    int x,y,w,h;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    /*! Initialize states of all member variables */
    TextScreenMessage();
    /*! Set what is supposed to be displayed by class */
    void setTextState(QString _textstate);
};

#endif // TEXTSCREENMESSAGE_H
