#pragma once

enum class Direction
{
    NO_DIRECTION = 0,
    LEFT = 1,
    UP = 2,
    DOWN = 3,
    RIGHT = 4
};

enum class GameState
{
    BeforeFirstRun = 0,
    Running = 1,
    Paused = 2,
    Aborted = 3,
    PacmanWin = 4,
    GhostWin = 5
};

static const int SIGNAL_READY = 5;
static const int SIGNAL_PAUSE = 6;
static const int SIGNAL_RESTART = 7;
