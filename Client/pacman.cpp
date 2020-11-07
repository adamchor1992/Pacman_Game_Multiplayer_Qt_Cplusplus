#include "map.h"
#include "pacman.h"
#include <QGraphicsPixmapItem>
#include <QPainter>

Pacman::Pacman()
{
    m_AnimationState = 0;
    m_AnimationModifyFactor = 6;
    LoadImages();
}

void Pacman::paint(QPainter *painter, const QStyleOptionGraphicsItem* /* unused */, QWidget* /* unused */)
{
    const int X_OFFSET = 15;
    const int Y_OFFSET = 15;
    const int WIDTH = 30;
    const int HEIGHT = 30;

    switch(m_Direction)
    {
    case 0:
        painter->drawPixmap(m_X - X_OFFSET , m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Left1Pixmap);
        break;
    case 1:
        if(m_AnimationState < 2 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Left1Pixmap);
        }
        else if(m_AnimationState < 4 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Left2Pixmap);
        }
        else if(m_AnimationState < 6 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Left3Pixmap);
        }
        else if(m_AnimationState < 8 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Left4Pixmap);
        }
        break;
    case 4:
        if(m_AnimationState < 2 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Right1Pixmap);
        }
        else if(m_AnimationState < 4 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Right2Pixmap);
        }
        else if(m_AnimationState < 6 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Right3Pixmap);
        }
        else if(m_AnimationState < 8 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Right4Pixmap);
        }
        break;
    case 3:
        if(m_AnimationState < 2 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Down1Pixmap);
        }
        else if(m_AnimationState < 4 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Down2Pixmap);
        }
        else if(m_AnimationState < 6 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Down3Pixmap);
        }
        else if(m_AnimationState < 8 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Down4Pixmap);
        }
        break;
    case 2:
        if(m_AnimationState < 2 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Up1Pixmap);
        }
        else if(m_AnimationState < 4 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Up2Pixmap);
        }
        else if(m_AnimationState < 6 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Up3Pixmap);
        }
        else if(m_AnimationState < 8 * m_AnimationModifyFactor)
        {
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Up4Pixmap);
        }
        break;
    }
}

void Pacman::AdvanceAnimation()
{
    if(m_AnimationState > 8 * m_AnimationModifyFactor - 2)
    {
        m_AnimationState = 0;
    }
    else
    {
        m_AnimationState++;
    }
}

void Pacman::LoadImages()
{
    m_Right1Pixmap.load(":/pacman/images/pacman_images/pacclose.png");
    m_Right2Pixmap.load(":/pacman/images/pacman_images/pacopen1.png");
    m_Right3Pixmap.load(":/pacman/images/pacman_images/pacopen2.png");
    m_Right4Pixmap.load(":/pacman/images/pacman_images/pacopen3.png");

    m_Up1Pixmap.load(":/pacman/images/pacman_images/paccloseu.png");
    m_Up2Pixmap.load(":/pacman/images/pacman_images/pacopen1u.png");
    m_Up3Pixmap.load(":/pacman/images/pacman_images/pacopen2u.png");
    m_Up4Pixmap.load(":/pacman/images/pacman_images/pacopen3u.png");

    m_Down1Pixmap.load(":/pacman/images/pacman_images/pacclosed.png");
    m_Down2Pixmap.load(":/pacman/images/pacman_images/pacopen1d.png");
    m_Down3Pixmap.load(":/pacman/images/pacman_images/pacopen2d.png");
    m_Down4Pixmap.load(":/pacman/images/pacman_images/pacopen3d.png");

    m_Left1Pixmap.load(":/pacman/images/pacman_images/pacclosel.png");
    m_Left2Pixmap.load(":/pacman/images/pacman_images/pacopen1l.png");
    m_Left3Pixmap.load(":/pacman/images/pacman_images/pacopen2l.png");
    m_Left4Pixmap.load(":/pacman/images/pacman_images/pacopen3l.png");
}
