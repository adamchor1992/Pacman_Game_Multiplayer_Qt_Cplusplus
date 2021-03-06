#pragma once

#include "movable_character.h"

class Ghost : public MovableCharacter
{
public:
    static const int SCARED_WHITE_THRESHOLD = 750;
    static const int SCARED_TIMEOUT = 1000;

    Ghost();
    void Reset();
    void IncrementScaredStateTimer() {++m_ScaredStateTimer;}

    void SetScaredState(GhostScaredState scaredState) {m_ScaredState = scaredState;}
    void SetScaredStateTimer(int scaredStateTimer) {m_ScaredStateTimer = scaredStateTimer;}
    int GetScaredStateTimer() const {return m_ScaredStateTimer;}
    GhostScaredState GetScaredState() const {return m_ScaredState;}

private:
    GhostScaredState m_ScaredState;
    int m_ScaredStateTimer;
};
