#ifndef PACMAN_H
#define PACMAN_H

class Pacman
{
private:
    int pac_x;
    int pac_y;
    int direction;
    int nextdirection;

public:
    Pacman();

    void setPac_X(int);
    void setPac_Y(int);
    void setDirection(int dir);
    void setNextDirection(int dir);

    int getPac_X() {return pac_x;}
    int getPac_Y() {return pac_y;}
    int getDirection() {return direction;}
    int getNextDirection() {return nextdirection;}
};

#endif // PACMAN_H
