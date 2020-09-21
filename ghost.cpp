#include "ghost.h"

Ghost::Ghost()
{
    m_AnimationState = 0;
    m_AnimationModifyFactor = 6;

    m_GhostDirection=1;
    m_IsScared=false;
    m_IsScaredWhite=false;

    LoadGhostImages();
}

QRectF Ghost::boundingRect() const
{
    return QRect(m_GhostX-15,m_GhostY-15,20,20);
}

void Ghost::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!m_IsScared)
    {
        switch(m_GhostDirection)
        {
        case 0:
            painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,down1);
            break;
        case 1:
            if(m_AnimationState==0)
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,left1);
            }
            else
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,left2);
            }
            break;
        case 4:
            if(m_AnimationState==0)
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,right1);
            }
            else
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,right2);
            }
            break;
        case 3:
            if(m_AnimationState==0)
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,down1);
            }
            else
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,down2);
            }
            break;
        case 2:
            if(m_AnimationState==0)
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,up1);
            }
            else
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,up2);
            }
            break;
        }
    }
    else
    {
        if(m_IsScaredWhite)
        {
            if(m_AnimationState==0)
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,scaredwhite);
            }
            else
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,scaredwhite1);
            }
        }
        else
        {
            if(m_AnimationState==0)
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,scaredblue);
            }
            else
            {
                painter->drawPixmap(m_GhostX-15,m_GhostY-15,30,30,scaredblue1);
            }
        }
    }
}

void Ghost::LoadGhostImages()
{
    right1.load(":/ghosts/images/ghost_images/ghostright1.png");
    right2.load(":/ghosts/images/ghost_images/ghostright2.png");
    up1.load(":/ghosts/images/ghost_images/ghostup1.png");
    up2.load(":/ghosts/images/ghost_images/ghostup2.png");
    down1.load(":/ghosts/images/ghost_images/ghostdown1.png");
    down2.load(":/ghosts/images/ghost_images/ghostdown2.png");
    left1.load(":/ghosts/images/ghost_images/ghostleft1.png");
    left2.load(":/ghosts/images/ghost_images/ghostleft2.png");
    scaredblue.load(":/ghosts/images/ghost_images/ghostscaredblue1.png");
    scaredblue1.load(":/ghosts/images/ghost_images/ghostscaredblue2.png");
    scaredwhite.load(":/ghosts/images/ghost_images/ghostscaredwhite1.png");
    scaredwhite1.load(":/ghosts/images/ghost_images/ghostscaredwhite2.png");
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

void Ghost::SetX(int x)
{
    m_GhostX = x;
}

void Ghost::SetY(int y)
{
    m_GhostY = y;
}

void Ghost::SetColor(QString col)
{
    if(col == "blue")
    {
        right1.load(":/ghosts/images/ghost_images/ghostrightblue1.png");
        right2.load(":/ghosts/images/ghost_images/ghostrightblue2.png");
        up1.load(":/ghosts/images/ghost_images/ghostupblue1.png");
        up2.load(":/ghosts/images/ghost_images/ghostupblue2.png");
        down1.load(":/ghosts/images/ghost_images/ghostdownblue1.png");
        down2.load(":/ghosts/images/ghost_images/ghostdownblue2.png");
        left1.load(":/ghosts/images/ghost_images/ghostleftblue1.png");
        left2.load(":/ghosts/images/ghost_images/ghostleftblue2.png");
    }
    else if(col == "orange")
    {
        right1.load(":/ghosts/images/ghost_images/ghostrightorange1.png");
        right2.load(":/ghosts/images/ghost_images/ghostrightorange2.png");
        up1.load(":/ghosts/images/ghost_images/ghostuporange1.png");
        up2.load(":/ghosts/images/ghost_images/ghostuporange2.png");
        down1.load(":/ghosts/images/ghost_images/ghostdownorange1.png");
        down2.load(":/ghosts/images/ghost_images/ghostdownorange2.png");
        left1.load(":/ghosts/images/ghost_images/ghostleftorange1.png");
        left2.load(":/ghosts/images/ghost_images/ghostleftorange2.png");
    }
    else if(col == "red")
    {
        right1.load(":/ghosts/images/ghost_images/ghostrightred1.png");
        right2.load(":/ghosts/images/ghost_images/ghostrightred2.png");
        up1.load(":/ghosts/images/ghost_images/ghostupred1.png");
        up2.load(":/ghosts/images/ghost_images/ghostupred2.png");
        down1.load(":/ghosts/images/ghost_images/ghostdownred1.png");
        down2.load(":/ghosts/images/ghost_images/ghostdownred2.png");
        left1.load(":/ghosts/images/ghost_images/ghostleftred1.png");
        left2.load(":/ghosts/images/ghost_images/ghostleftred2.png");
    }
}
