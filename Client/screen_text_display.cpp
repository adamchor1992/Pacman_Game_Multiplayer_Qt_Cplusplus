#include "screen_text_display.h"
#include "../common/log_manager.h"

ScreenTextDisplay::ScreenTextDisplay()
{
    SetTextDisplayState(TextDisplayState::NONE);
}

void ScreenTextDisplay::SetTextDisplayState(TextDisplayState textState)
{
    LogManager::LogToFile("ScreenTextDisplay changed to state: " + std::to_string(static_cast<int>(textState)));
    m_TextState = textState;
    show();
}

QRectF ScreenTextDisplay::boundingRect() const
{
    return QRect(X, Y, WIDTH, HEIGHT);
}

void ScreenTextDisplay::paint(QPainter* painter, const QStyleOptionGraphicsItem* /* unused */, QWidget* /* unused */)
{
    QPen redPen(Qt::red);
    QPen yellowPen(Qt::yellow);
    QPen magentaPen(Qt::magenta);

    painter->setPen(redPen);

    QFont font=painter->font();
    font.setPointSize(25);
    painter->setFont(font);

    switch(m_TextState)
    {
    case TextDisplayState::START:
        painter->drawText(boundingRect(),Qt::AlignCenter, "GET READY AND PRESS SPACE");
        break;
    case TextDisplayState::PAUSED:
        painter->drawText(boundingRect(),Qt::AlignCenter, "PAUSED");
        break;
    case TextDisplayState::END_PACMAN:
        painter->setPen(yellowPen);
        painter->drawText(boundingRect(),Qt::AlignCenter, "PACMAN WINS\nPRESS SPACE TO RESTART");
        break;
    case TextDisplayState::END_GHOST:
        painter->setPen(magentaPen);
        painter->drawText(boundingRect(),Qt::AlignCenter, "GHOST WINS\nPRESS SPACE TO RESTART");
        break;
    case TextDisplayState::DISCONNECTED:
        painter->drawText(boundingRect(),Qt::AlignCenter, "DISCONNECTED");
        break;
    case TextDisplayState::NONE:
        painter->drawText(boundingRect(),Qt::AlignCenter, "");
        break;
    default:
        qDebug() << "WRONG TEXT STATE";
    }
}
