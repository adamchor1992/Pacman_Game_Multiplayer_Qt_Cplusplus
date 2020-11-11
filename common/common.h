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

enum class GhostScaredState
{
    NO_SCARED,
    SCARED_BLUE,
    SCARED_WHITE
};

static const char MOVEMENT_LEFT = '1';
static const char MOVEMENT_UP = '2';
static const char MOVEMENT_DOWN = '3';
static const char MOVEMENT_RIGHT = '4';
static const char SIGNAL_START = '5';
static const char SIGNAL_PAUSE = '6';
static const int PACMAN_START_X = 320;
static const int PACMAN_START_Y = 514;
static const int GHOST_START_X = 307;
static const int GHOST_START_Y = 252;
