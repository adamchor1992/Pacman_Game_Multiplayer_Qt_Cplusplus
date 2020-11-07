#include "map.h"
#include "pacman.h"

Pacman::Pacman()
{
    SetX(PACMAN_START_X);
    SetY(PACMAN_START_Y);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
}

void Pacman::Reset()
{
    SetX(PACMAN_START_X);
    SetY(PACMAN_START_Y);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
}
