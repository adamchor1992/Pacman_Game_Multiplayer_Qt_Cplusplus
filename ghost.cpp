#include "ghost.h"

Ghost::Ghost()
{
    SetX(307);
    SetY(252);
    SetDirection(0); //ghost does not move after game start
    SetNextDirection(0);
    SetScaredBlue(false);
    SetScaredWhite(false);
    SetScaredStateTimer(0);
}

void Ghost::Reset()
{
    SetX(307);
    SetY(252);
    SetDirection(0); //ghost does not move after game start
    SetNextDirection(0);
    SetScaredBlue(false);
    SetScaredWhite(false);
    SetScaredStateTimer(0);
}
