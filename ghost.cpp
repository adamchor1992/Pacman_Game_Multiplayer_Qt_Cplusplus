#include "ghost.h"

Ghost::Ghost()
{
    setGhost_X(307);
    setGhost_Y(252);
    setGhostDirection(0); //ghost does not move after game start
    setIsScared(false);
    setScaredWhite(false);
    setScarestate(0);
}

void Ghost::Reset()
{
    setGhost_X(307);
    setGhost_Y(252);
    setGhostDirection(0); //ghost does not move after game start
    setIsScared(false);
    setScaredWhite(false);
    setScarestate(0);
}
