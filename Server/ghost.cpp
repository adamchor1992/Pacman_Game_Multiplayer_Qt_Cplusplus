#include "ghost.h"

Ghost::Ghost()
{
    SetX(START_X);
    SetY(START_Y);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
    SetScaredStateBlue(false);
    SetScaredStateWhite(false);
    SetScaredStateTimer(0);
}

void Ghost::Reset()
{
    SetX(START_X);
    SetY(START_Y);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
    SetScaredStateBlue(false);
    SetScaredStateWhite(false);
    SetScaredStateTimer(0);
}
