#pragma once

#include "movable_character.h"
#include <QPainter>

class Ghost : public MovableCharacter
{
public:
    Ghost();
    void SetScaredState(GhostScaredState scaredState) {m_ScaredState = scaredState;}
    void SetColor(QString color);
    void AdvanceAnimation() override;

private:
    GhostScaredState m_ScaredState;

    QPixmap m_Left1Pixmap, m_Left2Pixmap;
    QPixmap m_Up1Pixmap, m_Up2Pixmap;
    QPixmap m_Down1Pixmap, m_Down2Pixmap;
    QPixmap m_Right1Pixmap, m_Right2Pixmap;
    QPixmap m_ScaredBlue1Pixmap, m_ScaredBlue2Pixmap;
    QPixmap m_ScaredWhite1Pixmap, m_ScaredWhite2Pixmap;

    void LoadImages() override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
};
