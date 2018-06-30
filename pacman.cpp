#include "map.h"
#include "pacman.h"

Pacman::Pacman()
{
    setPac_X(320);
    setPac_Y(514);
    setDirection(0); //pacman does not move after game start
}

void Pacman::Reset()
{
    setPac_X(320);
    setPac_Y(514);
    setDirection(0); //pacman does not move after game start
}

