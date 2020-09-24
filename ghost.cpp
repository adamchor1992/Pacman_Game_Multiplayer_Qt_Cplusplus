#include "ghost.h"

Ghost::Ghost()
{
    SetX(307);
    SetY(252);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
    SetScaredStateBlue(false);
    SetScaredStateWhite(false);
    SetScaredStateTimer(0);
}

void Ghost::Reset()
{
    SetX(307);
    SetY(252);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
    SetScaredStateBlue(false);
    SetScaredStateWhite(false);
    SetScaredStateTimer(0);
}
