#include "map.h"
#include "pacman.h"

Pacman::Pacman()
{

}

void Pacman::setPac_X(int x)
{
    pac_x=x;
}

void Pacman::setPac_Y(int y)
{
    pac_y=y;
}

void Pacman::setDirection(int dir)
{
    direction=dir;
}

void Pacman::setNextDirection(int dir)
{
    nextdirection=dir;
}


