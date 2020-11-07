#pragma once

enum class Direction
{
    NO_DIRECTION = 0,
    LEFT = 1,
    UP = 2,
    DOWN = 3,
    RIGHT = 4
};

const int SIGNAL_READY = 5;
const int SIGNAL_PAUSE = 6;
const int SIGNAL_RESTART = 7;
