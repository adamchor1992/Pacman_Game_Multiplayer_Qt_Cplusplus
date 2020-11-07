#include "map.h"
#include "pacman.h"

Pacman::Pacman()
{
    SetX(START_X);
    SetY(START_Y);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
}

void Pacman::Reset()
{
    SetX(START_X);
    SetY(START_Y);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
}
