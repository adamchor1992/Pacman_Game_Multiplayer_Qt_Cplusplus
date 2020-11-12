#pragma once

#include <QMap>
#include <QDebug>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class ScreenTextDisplay : public QGraphicsItem
{
public:
    enum class TextDisplayState
    {
        NONE = 0,
        START = 1,
        PAUSED = 2,
        END_PACMAN = 3,
        END_GHOST = 4,
        DISCONNECTED = 5,
    };

    ScreenTextDisplay();
    void SetTextDisplayState(TextDisplayState textState);

private:
    TextDisplayState m_TextState;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    static constexpr int WIDTH = 600;
    static constexpr int HEIGHT = 200;
    static constexpr int X = 614 / 2 - WIDTH / 2;
    static constexpr int Y = 714 / 2 - HEIGHT / 2;
};
