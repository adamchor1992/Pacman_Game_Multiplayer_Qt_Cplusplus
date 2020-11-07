#pragma once

#include "movable_character.h"

class Pacman : public MovableCharacter
{
public:
    Pacman();
    void Reset();

private:
    const int START_X = 320;
    const int START_Y = 514;
};
