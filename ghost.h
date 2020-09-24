#pragma once

#include "../common/common.h"

class Ghost
{
public:
    Ghost();
    void SetX(int x) {m_X = x;}
    void SetY(int y) {m_Y = y;}
    void SetScaredStateBlue(bool isScaredBlue) {m_IsScaredBlue = isScaredBlue;}
    void SetScaredStateWhite(bool is_ScaredWhite) {m_IsScaredWhite = is_ScaredWhite;}
    void SetDirection(Direction direction) {m_Direction = direction;}
    void SetNextDirection(Direction nextDirection) {m_NextDirection = nextDirection;}
    void Reset();
    void SetScaredStateTimer(int scaredStateTimer) {m_ScaredStateTimer = scaredStateTimer;}
    void IncrementScaredStateTimer() {++m_ScaredStateTimer;}
    int GetX() const {return m_X;}
    int GetY() const {return m_Y;}
    Direction GetDirection() const {return m_Direction;}
    Direction GetNextDirection() const {return m_NextDirection;}
    int GetScaredStateTimer() const {return m_ScaredStateTimer;}
    bool GetScaredBlue() const {return m_IsScaredBlue;}
    bool GetScaredWhite() const {return m_IsScaredWhite;}

private:
    bool m_IsScaredBlue;
    bool m_IsScaredWhite;
    int m_X, m_Y;
    Direction m_Direction, m_NextDirection;
    int m_ScaredStateTimer;
};
