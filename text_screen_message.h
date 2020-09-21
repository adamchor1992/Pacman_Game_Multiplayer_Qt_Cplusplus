#pragma once

#include <QMap>
#include <QDebug>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class TextScreenMessage : public QGraphicsItem
{
private:
    QMap <QString,int> m_TextStates;
    int m_CurrentTextState;
    int m_X, m_Y, m_W, m_H;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public:
    TextScreenMessage();
    void SetTextState(QString _textstate);
};
