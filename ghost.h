#ifndef GHOST_H
#define GHOST_H
#include <QGraphicsItem>
#include <QPainter>

class Ghost : public QGraphicsItem
{
private:
    QPixmap ghost_pixmap;
    QPixmap ghost_scared_blue;
    QPixmap ghost_scared_white;

    QPixmap left1,left2;
    QPixmap up1,up2;
    QPixmap down1,down2;
    QPixmap right1,right2;
    QPixmap scaredblue, scaredblue1;
    QPixmap scaredwhite, scaredwhite1;

    bool is_scared;
    bool scared_white;

    int animestate;
    int animation_modify_factor;

    int ghost_x,ghost_y;
    int ghostdirection;
    int nextghostdirection;

public:
    Ghost();
    void LoadGhostImages();
    void advance();

    void setGhost_X(int);
    void setGhost_Y(int);
    void setIsScared(bool option) {is_scared=option;}
    void setScaredWhite(bool option) {scared_white=option;}
    void setGhostDirection(int dir) {ghostdirection=dir;}
    void setGhostColor(QString col);

    //redefined pure virtual methods
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GHOST_H
