#include "ghost.h"

Ghost::Ghost()
{
    m_AnimationState = 0;
    m_AnimationModifyFactor = 6;

    m_Direction = 1;
    m_IsScaredBlue = false;
    m_IsScaredWhite = false;

    LoadImages();
}

QRectF Ghost::boundingRect() const
{
    return QRect(m_X - IMAGE_OFFSET_X, m_Y - IMAGE_OFFSET_Y, IMAGE_WIDTH, IMAGE_HEIGHT);
}

void Ghost::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const int X_OFFSET = 15;
    const int Y_OFFSET = 15;
    const int WIDTH = 30;
    const int HEIGHT = 30;

    if(!m_IsScaredBlue)
    {
        switch(m_Direction)
        {
        case 0:
            painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Down1Pixmap);
            break;
        case 1:
            if(m_AnimationState == 0)
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Left1Pixmap);
            }
            else
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Left2Pixmap);
            }
            break;
        case 4:
            if(m_AnimationState == 0)
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Right1Pixmap);
            }
            else
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Right2Pixmap);
            }
            break;
        case 3:
            if(m_AnimationState == 0)
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Down1Pixmap);
            }
            else
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Down2Pixmap);
            }
            break;
        case 2:
            if(m_AnimationState == 0)
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Up1Pixmap);
            }
            else
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, HEIGHT, m_Up2Pixmap);
            }
            break;
        }
    }
    else
    {
        if(m_IsScaredWhite)
        {
            if(m_AnimationState == 0)
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, 30, m_ScaredWhite1Pixmap);
            }
            else
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, 30, m_ScaredWhite2Pixmap);
            }
        }
        else
        {
            if(m_AnimationState == 0)
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, 30, m_ScaredBlue1Pixmap);
            }
            else
            {
                painter->drawPixmap(m_X - X_OFFSET, m_Y - Y_OFFSET, WIDTH, 30, m_ScaredBlue2Pixmap);
            }
        }
    }
}

void Ghost::LoadImages()
{
    m_Right1Pixmap.load(":/ghosts/images/ghost_images/ghostright1.png");
    m_Right2Pixmap.load(":/ghosts/images/ghost_images/ghostright2.png");
    m_Up1Pixmap.load(":/ghosts/images/ghost_images/ghostup1.png");
    m_Up2Pixmap.load(":/ghosts/images/ghost_images/ghostup2.png");
    m_Down1Pixmap.load(":/ghosts/images/ghost_images/ghostdown1.png");
    m_Down2Pixmap.load(":/ghosts/images/ghost_images/ghostdown2.png");
    m_Left1Pixmap.load(":/ghosts/images/ghost_images/ghostleft1.png");
    m_Left2Pixmap.load(":/ghosts/images/ghost_images/ghostleft2.png");
    m_ScaredBlue1Pixmap.load(":/ghosts/images/ghost_images/ghostscaredblue1.png");
    m_ScaredBlue2Pixmap.load(":/ghosts/images/ghost_images/ghostscaredblue2.png");
    m_ScaredWhite1Pixmap.load(":/ghosts/images/ghost_images/ghostscaredwhite1.png");
    m_ScaredWhite2Pixmap.load(":/ghosts/images/ghost_images/ghostscaredwhite2.png");
}

void Ghost::AdvanceAnimation()
{
    if(m_AnimationState > 2)
    {
        m_AnimationState = 0;
    }
    else
    {
        m_AnimationState++;
    }
}

void Ghost::SetColor(QString color)
{
    if(color == "blue")
    {
        m_Right1Pixmap.load(":/ghosts/images/ghost_images/ghostrightblue1.png");
        m_Right2Pixmap.load(":/ghosts/images/ghost_images/ghostrightblue2.png");
        m_Up1Pixmap.load(":/ghosts/images/ghost_images/ghostupblue1.png");
        m_Up2Pixmap.load(":/ghosts/images/ghost_images/ghostupblue2.png");
        m_Down1Pixmap.load(":/ghosts/images/ghost_images/ghostdownblue1.png");
        m_Down2Pixmap.load(":/ghosts/images/ghost_images/ghostdownblue2.png");
        m_Left1Pixmap.load(":/ghosts/images/ghost_images/ghostleftblue1.png");
        m_Left2Pixmap.load(":/ghosts/images/ghost_images/ghostleftblue2.png");
    }
    else if(color == "orange")
    {
        m_Right1Pixmap.load(":/ghosts/images/ghost_images/ghostrightorange1.png");
        m_Right2Pixmap.load(":/ghosts/images/ghost_images/ghostrightorange2.png");
        m_Up1Pixmap.load(":/ghosts/images/ghost_images/ghostuporange1.png");
        m_Up2Pixmap.load(":/ghosts/images/ghost_images/ghostuporange2.png");
        m_Down1Pixmap.load(":/ghosts/images/ghost_images/ghostdownorange1.png");
        m_Down2Pixmap.load(":/ghosts/images/ghost_images/ghostdownorange2.png");
        m_Left1Pixmap.load(":/ghosts/images/ghost_images/ghostleftorange1.png");
        m_Left2Pixmap.load(":/ghosts/images/ghost_images/ghostleftorange2.png");
    }
    else if(color == "red")
    {
        m_Right1Pixmap.load(":/ghosts/images/ghost_images/ghostrightred1.png");
        m_Right2Pixmap.load(":/ghosts/images/ghost_images/ghostrightred2.png");
        m_Up1Pixmap.load(":/ghosts/images/ghost_images/ghostupred1.png");
        m_Up2Pixmap.load(":/ghosts/images/ghost_images/ghostupred2.png");
        m_Down1Pixmap.load(":/ghosts/images/ghost_images/ghostdownred1.png");
        m_Down2Pixmap.load(":/ghosts/images/ghost_images/ghostdownred2.png");
        m_Left1Pixmap.load(":/ghosts/images/ghost_images/ghostleftred1.png");
        m_Left2Pixmap.load(":/ghosts/images/ghost_images/ghostleftred2.png");
    }
}
