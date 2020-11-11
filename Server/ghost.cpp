#include "ghost.h"

Ghost::Ghost()
{
    SetX(GHOST_START_X);
    SetY(GHOST_START_Y);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
    m_ScaredState = GhostScaredState::NO_SCARED;
    SetScaredStateTimer(0);
}

void Ghost::Reset()
{
    SetX(GHOST_START_X);
    SetY(GHOST_START_Y);
    SetDirection(Direction::NO_DIRECTION);
    SetNextDirection(Direction::NO_DIRECTION);
    m_ScaredState = GhostScaredState::NO_SCARED;
    SetScaredStateTimer(0);
}
