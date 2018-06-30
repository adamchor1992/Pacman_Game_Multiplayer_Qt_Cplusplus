#ifndef GHOST_H
#define GHOST_H

class Ghost
{
private:
    bool is_scared;
    bool scared_white;
    int scarestate;
    int ghost_x,ghost_y;
    int ghostdirection;
    int nextghostdirection;

public:
    Ghost();

    void Reset();

    //setters
    void setGhost_X(int x) {ghost_x = x;}
    void setGhost_Y(int y) {ghost_y = y;}
    void setIsScared(bool option) {is_scared = option;}
    void setScaredWhite(bool option) {scared_white = option;}
    void setGhostDirection(int dir) {ghostdirection = dir;}
    void setNextGhostDirection(int dir) {nextghostdirection = dir;}
    void setScarestate(int _scarestate) {scarestate = _scarestate;}
    void incrementScarestate() {scarestate++;}

    //getters
    int getGhost_X() const {return ghost_x;}
    int getGhost_Y() const {return ghost_y;}
    int getGhostDirection() const {return ghostdirection;}
    int getNextGhostDirection() const {return nextghostdirection;}
    int getScarestate() const {return scarestate;}
    bool getIsScared() const {return is_scared;}
    bool getScaredWhite() const {return scared_white;}
};

#endif // GHOST_H
