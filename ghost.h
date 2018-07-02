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
    /*! Initialize ghost position and direction of movement */
    Ghost();
    /*! Reset ghost position and direction of movement */
    void Reset();

    //setters
        /*! Set ghost x coordinate */
    void setGhost_X(int x) {ghost_x = x;}
        /*! Set ghost y coordinate */
    void setGhost_Y(int y) {ghost_y = y;}
    /*! Set if ghost is scared blue */
    void setIsScared(bool option) {is_scared = option;}
    /*! Set if ghost is scared white */
    void setScaredWhite(bool option) {scared_white = option;}
    /*! Set ghost direction of movement */
    void setGhostDirection(int dir) {ghostdirection = dir;}
    /*! Set next ghost direction of movement */
    void setNextGhostDirection(int dir) {nextghostdirection = dir;}
    /*! Set ghost scarestate */
    void setScarestate(int _scarestate) {scarestate = _scarestate;}
    /*! Increment ghost scarestate */
    void incrementScarestate() {scarestate++;}

    //getters
    /*! Get ghost x coordinate in form of int */
    int getGhost_X() const {return ghost_x;}
    /*! Get ghost y coordinate in form of int */
    int getGhost_Y() const {return ghost_y;}
    /*! Get ghost direction of movement in form of int */
    int getGhostDirection() const {return ghostdirection;}
    /*! Get next ghost direction of movement in form of int */
    int getNextGhostDirection() const {return nextghostdirection;}
    /*! Get ghost scarestate in form of int */
    int getScarestate() const {return scarestate;}
    /*! Check if ghost is scared blue and return bool */
    bool getIsScared() const {return is_scared;}
    /*! Check if ghost is scared white and return bool */
    bool getScaredWhite() const {return scared_white;}
};

#endif // GHOST_H
