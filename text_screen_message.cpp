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
}

void TextScreenMessage::SetTextState(QString textState)
{
    if(m_TextStates.contains(textState))
    {
        m_CurrentTextState = m_TextStates.value(textState);
    }
    else
    {
        qDebug() << "Wrong text state";
    }
}

QRectF TextScreenMessage::boundingRect() const
{
    return QRect(X, Y, WIDTH, HEIGHT);
}

void TextScreenMessage::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPen redPen(Qt::red);
    QPen yellowPen(Qt::yellow);
    QPen magentaPen(Qt::magenta);

    painter->setPen(redPen);

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
        painter->setPen(yellowPen);
        painter->drawText(boundingRect(),Qt::AlignCenter, "PACMAN WINS\nPRESS SPACE TO RESTART");
        break;
    case 4:
        painter->setPen(magentaPen);
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
