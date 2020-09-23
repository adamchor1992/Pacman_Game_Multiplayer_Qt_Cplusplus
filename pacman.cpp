#include "map.h"
#include "pacman.h"

Pacman::Pacman()
{
    SetX(320);
    SetY(514);
    SetDirection(0); //pacman does not move after game start
    SetNextDirection(0);
}

void Pacman::Reset()
{
    SetX(320);
    SetY(514);
    SetDirection(0); //pacman does not move after game start
    SetNextDirection(0);
}

