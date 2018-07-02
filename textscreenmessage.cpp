#include "textscreenmessage.h"

TextScreenMessage::TextScreenMessage()
{
    text_states.insert("start", 1);
    text_states.insert("paused", 2);
    text_states.insert("end_pacman", 3);
    text_states.insert("end_ghost", 4);
    text_states.insert("disconnected", 5);
    text_states.insert("null", 6);

    current_text_state = 0;
    setTextState("start");

    w=600;
    h=200;
    x=614/2-w/2;
    y=714/2-h/2;
}

void TextScreenMessage::setTextState(QString _textstate)
{
    if(text_states.contains(_textstate))
    {
        current_text_state = text_states.value(_textstate);
    }
    else
    {
        qDebug() << "Wrong text state";
    }
}

QRectF TextScreenMessage::boundingRect() const
{
    return QRect(x, y, w, h);
}

void TextScreenMessage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen_red(Qt::red);
    QPen pen_yellow(Qt::yellow);
    QPen pen_magenta(Qt::magenta);

    painter->setPen(pen_red);

    QFont font=painter->font();
    font.setPointSize (25);
    painter->setFont(font);

    switch(current_text_state)
    {
    case 1:
        painter->drawText(boundingRect(),Qt::AlignCenter, "GET READY AND PRESS SPACE");
        break;
    case 2:
        painter->drawText(boundingRect(),Qt::AlignCenter, "PAUSED");
        break;
    case 3:
        painter->setPen(pen_yellow);
        painter->drawText(boundingRect(),Qt::AlignCenter, "PACMAN WINS\nPRESS SPACE TO RESTART");
        break;
    case 4:
        painter->setPen(pen_magenta);
        painter->drawText(boundingRect(),Qt::AlignCenter, "GHOST WINS\nPRESS SPACE TO RESTART");
        break;
    case 5:
        painter->drawText(boundingRect(),Qt::AlignCenter, "DISCONNECTED");
        break;
    case 6:
        painter->drawText(boundingRect(),Qt::AlignCenter, "");
        break;
    default:
        painter->drawText(boundingRect(),Qt::AlignCenter, "WRONG TEXT STATE");
        break;
    }
}
