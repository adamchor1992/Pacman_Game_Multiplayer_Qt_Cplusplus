#pragma once

#include "movable_character.h"

class Pacman : public MovableCharacter
{
public:
    Pacman();
    void AdvanceAnimation() override;

private:
    QPixmap m_Left1Pixmap, m_Left2Pixmap, m_Left3Pixmap, m_Left4Pixmap;
    QPixmap m_Up1Pixmap, m_Up2Pixmap, m_Up3Pixmap, m_Up4Pixmap;
    QPixmap m_Down1Pixmap, m_Down2Pixmap, m_Down3Pixmap, m_Down4Pixmap;
    QPixmap m_Right1Pixmap, m_Right2Pixmap, m_Right3Pixmap, m_Right4Pixmap;

    void LoadImages() override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
};
