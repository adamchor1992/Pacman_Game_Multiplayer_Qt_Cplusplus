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
    /*! Initialize pacman position and direction of movement */
    Pacman();
    /*! Reset pacman position and direction of movement */
    void Reset();

    //setters
    /*! Set pacman x coordinate */
    void setPac_X(int x) {pac_x = x;}
    /*! Set pacman y coordinate */
    void setPac_Y(int y) {pac_y = y;}
    /*! Set pacman direction of movement */
    void setDirection(int dir) {direction = dir;}
    /*! Set next pacman direction of movement */
    void setNextDirection(int dir) {nextdirection = dir;}

    //getters
    /*! Get pacman x coordinate in form of int */
    int getPac_X() const {return pac_x;}
    /*! Get pacman y coordinate in form of int */
    int getPac_Y() const {return pac_y;}
    /*! Get pacman direction of movement in form of int */
    int getDirection() const {return direction;}
    /*! Set pacman next direction of movement in form of int */
    int getNextDirection() const {return nextdirection;}
};

#endif // PACMAN_H
