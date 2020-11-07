#pragma once

#include "movable_character.h"

class Ghost : public MovableCharacter
{
public:
    static const int SCARED_WHITE_THRESHOLD = 750;
    static const int SCARED_TIMEOUT = 1000;

    Ghost();
    void SetScaredStateBlue(bool isScaredBlue) {m_IsScaredBlue = isScaredBlue;}
    void SetScaredStateWhite(bool is_ScaredWhite) {m_IsScaredWhite = is_ScaredWhite;}
    void SetScaredStateTimer(int scaredStateTimer) {m_ScaredStateTimer = scaredStateTimer;}
    void IncrementScaredStateTimer() {++m_ScaredStateTimer;}
    int GetScaredStateTimer() const {return m_ScaredStateTimer;}
    bool GetScaredBlue() const {return m_IsScaredBlue;}
    bool GetScaredWhite() const {return m_IsScaredWhite;}

    void Reset();

private:
    const int START_X = 307;
    const int START_Y = 252;
    bool m_IsScaredBlue;
    bool m_IsScaredWhite;
    int m_ScaredStateTimer;
};
