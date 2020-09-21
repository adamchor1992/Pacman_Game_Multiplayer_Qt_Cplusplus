#pragma once

#include <QGraphicsItem>
#include <QPainter>

class Ghost : public QGraphicsItem
{
private:
    QPixmap m_GhostPixmap;
    QPixmap m_GhostScaredBluePixmap;
    QPixmap m_GhostScaredWhitePixmap;

    QPixmap left1,left2;
    QPixmap up1,up2;
    QPixmap down1,down2;
    QPixmap right1,right2;
    QPixmap scaredblue, scaredblue1;
    QPixmap scaredwhite, scaredwhite1;

    bool m_IsScared;
    bool m_IsScaredWhite;

    int m_AnimationState;
    int m_AnimationModifyFactor;

    int m_GhostX, m_GhostY;
    int m_GhostDirection;
    int m_NextGhostDirection;

    void LoadGhostImages();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public:
    Ghost();
    void AdvanceAnimation();
    void SetX(int);
    void SetY(int);
    void SetScaredStateBlue(bool option) {m_IsScared = option;}
    void SetScaredStateWhite(bool option) {m_IsScaredWhite = option;}
    void SetDirection(int dir) {m_GhostDirection = dir;}
    void SetColor(QString col);
};
