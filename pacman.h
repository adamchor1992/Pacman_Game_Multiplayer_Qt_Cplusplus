#pragma once

#include <QGraphicsItem>

class Pacman : public QGraphicsItem
{
public:
    Pacman();
    void AdvanceAnimation();
    void SetX(int x);
    void SetY(int y);
    void SetDirection(int direction);

private:
    const int IMAGE_OFFSET_X = 15;
    const int IMAGE_OFFSET_Y = 15;
    const int IMAGE_WIDTH = 30;
    const int IMAGE_HEIGHT = 30;

    int m_X;
    int m_Y;
    int m_Direction;
    int m_AnimationState;
    int m_AnimationModifyFactor;

    QPixmap m_Pixmap;
    QPixmap m_Left1Pixmap, m_Left2Pixmap, m_Left3Pixmap, m_Left4Pixmap;
    QPixmap m_Up1Pixmap, m_Up2Pixmap, m_Up3Pixmap, m_Up4Pixmap;
    QPixmap m_Down1Pixmap, m_Down2Pixmap, m_Down3Pixmap, m_Down4Pixmap;
    QPixmap m_Right1Pixmap, m_Right2Pixmap, m_Right3Pixmap, m_Right4Pixmap;

    void LoadPacmanImages();

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};
