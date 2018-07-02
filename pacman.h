#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsItem>

class Pacman : public QGraphicsItem
{
private:
    int pac_x;
    int pac_y;
    int direction;
    int animestate;
    int animation_modify_factor;

    QPixmap pacman_pixmap;

    QPixmap left1,left2,left3,left4;
    QPixmap up1,up2,up3,up4;
    QPixmap down1,down2,down3,down4;
    QPixmap right1,right2,right3,right4;

    void LoadPacmanImages();
    //redefined pure virtual methods
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    /*! Initialize member variables and load pacman images */
    Pacman();
    /*! Increment pacman's animation state */
    void advance();
    /*! Set pacman x coordinate */
    void setPac_X(int);
    /*! Set pacman y coordinate */
    void setPac_Y(int);
    /*! Set pacman direction of movement */
    void setDirection(int dir);
};

#endif // PACMAN_H
