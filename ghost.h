#pragma once

class Ghost
{
public:
    Ghost();
    void Reset();
    void SetX(int x) {m_GhostX = x;}
    void SetY(int y) {m_GhostY = y;}
    void SetScaredBlue(bool isScaredBlue) {m_IsScaredBlue = isScaredBlue;}
    void SetScaredWhite(bool is_ScaredWhite) {m_IsScaredWhite = is_ScaredWhite;}
    void SetDirection(int direction) {m_Direction = direction;}
    void SetNextDirection(int nextDirection) {m_NextDirection = nextDirection;}
    void SetScaredStateTimer(int scaredStateTimer) {m_ScaredStateTimer = scaredStateTimer;}
    void IncrementScaredStateTimer() {++m_ScaredStateTimer;}

    int GetX() const {return m_GhostX;}
    int GetY() const {return m_GhostY;}
    int GetDirection() const {return m_Direction;}
    int GetNextDirection() const {return m_NextDirection;}
    int GetScaredStateTimer() const {return m_ScaredStateTimer;}
    bool GetScaredBlue() const {return m_IsScaredBlue;}
    bool GetScaredWhite() const {return m_IsScaredWhite;}

private:
    bool m_IsScaredBlue;
    bool m_IsScaredWhite;
    int m_ScaredStateTimer;
    int m_GhostX, m_GhostY;
    int m_Direction;
    int m_NextDirection;
};
