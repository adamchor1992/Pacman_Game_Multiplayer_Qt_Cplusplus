#include "text_screen_message.h"

TextScreenMessage::TextScreenMessage()
{
    m_TextStates.insert("start", 1);
    m_TextStates.insert("paused", 2);
    m_TextStates.insert("end_pacman", 3);
    m_TextStates.insert("end_ghost", 4);
    m_TextStates.insert("disconnected", 5);
    m_TextStates.insert("null", 6);

    m_CurrentTextState = 0;
    SetTextState("start");

    m_W=600;
    m_H=200;
    m_X=614/2-m_W/2;
    m_Y=714/2-m_H/2;
}

void TextScreenMessage::SetTextState(QString _textstate)
{
    if(m_TextStates.contains(_textstate))
    {
        m_CurrentTextState = m_TextStates.value(_textstate);
    }
    else
    {
        qDebug() << "Wrong text state";
    }
}

QRectF TextScreenMessage::boundingRect() const
{
    return QRect(m_X, m_Y, m_W, m_H);
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

    switch(m_CurrentTextState)
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
