#pragma once

#include <QGraphicsItem>
#include "../common/common.h"

class MovableCharacter : public QGraphicsItem
{
public:
    void SetX(int x) {m_X = x;}
    void SetY(int y) {m_Y = y;}
    void SetDirection(Direction direction) {m_Direction = direction;}
    virtual void AdvanceAnimation() = 0;
    virtual void LoadImages() = 0;

    QRectF boundingRect() const {return QRect(m_X - IMAGE_OFFSET_X, m_Y - IMAGE_OFFSET_Y, IMAGE_WIDTH, IMAGE_HEIGHT);}

protected:
    const int IMAGE_OFFSET_X = 15;
    const int IMAGE_OFFSET_Y = 15;
    const int IMAGE_WIDTH = 30;
    const int IMAGE_HEIGHT = 30;

    int m_X, m_Y;
    Direction m_Direction;
    int m_AnimationState;
    int m_AnimationModifyFactor;
};
