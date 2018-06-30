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

    void Reset();

    //setters
    void setPac_X(int x) {pac_x = x;}
    void setPac_Y(int y) {pac_y = y;}
    void setDirection(int dir) {direction = dir;}
    void setNextDirection(int dir) {nextdirection = dir;}

    //getters
    int getPac_X() const {return pac_x;}
    int getPac_Y() const {return pac_y;}
    int getDirection() const {return direction;}
    int getNextDirection() const {return nextdirection;}
};

#endif // PACMAN_H
