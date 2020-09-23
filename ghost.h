#pragma once

#include <QGraphicsItem>
#include <QPainter>

class Ghost : public QGraphicsItem
{
public:
    Ghost();
    void SetX(int x) {m_X = x;}
    void SetY(int y) {m_Y = y;}
    void SetScaredStateBlue(bool isScaredBlue) {m_IsScaredBlue = isScaredBlue;}
    void SetScaredStateWhite(bool is_ScaredWhite) {m_IsScaredWhite = is_ScaredWhite;}
    void SetDirection(int direction) {m_Direction = direction;}
    void AdvanceAnimation();
    void SetColor(QString color);

private:
    bool m_IsScaredBlue;
    bool m_IsScaredWhite;
    int m_X, m_Y;
    int m_Direction, m_NextDirection;
    int m_AnimationState;
    int m_AnimationModifyFactor;

    const int IMAGE_OFFSET_X = 15;
    const int IMAGE_OFFSET_Y = 15;
    const int IMAGE_WIDTH = 30;
    const int IMAGE_HEIGHT = 30;

    QPixmap m_Pixmap;
    QPixmap m_Left1Pixmap, m_Left2Pixmap;
    QPixmap m_Up1Pixmap, m_Up2Pixmap;
    QPixmap m_Down1Pixmap, m_Down2Pixmap;
    QPixmap m_Right1Pixmap, m_Right2Pixmap;
    QPixmap m_ScaredBlue1Pixmap, m_ScaredBlue2Pixmap;
    QPixmap m_ScaredWhite1Pixmap, m_ScaredWhite2Pixmap;

    void LoadImages();

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};
