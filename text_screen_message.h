#pragma once

#include <QMap>
#include <QDebug>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class TextScreenMessage : public QGraphicsItem
{
public:
    TextScreenMessage();
    void SetTextState(QString textState);

private:
    QMap <QString, int> m_TextStates;
    int m_CurrentTextState;


    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    static constexpr int WIDTH = 600;
    static constexpr int HEIGHT = 200;
    static constexpr int X = 614 / 2 - WIDTH / 2;
    static constexpr int Y = 714 / 2 - HEIGHT / 2;
};
