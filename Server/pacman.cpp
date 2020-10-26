#include "map.h"
#include "pacman.h"

Pacman::Pacman()
{
    SetX(320);
    SetY(514);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
}

void Pacman::Reset()
{
    SetX(320);
    SetY(514);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
}

