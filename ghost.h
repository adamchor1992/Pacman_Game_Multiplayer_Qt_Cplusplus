#pragma once

class Ghost
{
public:
    Ghost();
    void SetX(int x) {m_X = x;}
    void SetY(int y) {m_Y = y;}
    void SetScaredStateBlue(bool isScaredBlue) {m_IsScaredBlue = isScaredBlue;}
    void SetScaredStateWhite(bool is_ScaredWhite) {m_IsScaredWhite = is_ScaredWhite;}
    void SetDirection(int direction) {m_Direction = direction;}
    void Reset();
    void SetNextDirection(int nextDirection) {m_NextDirection = nextDirection;}
    void SetScaredStateTimer(int scaredStateTimer) {m_ScaredStateTimer = scaredStateTimer;}
    void IncrementScaredStateTimer() {++m_ScaredStateTimer;}
    int GetX() const {return m_X;}
    int GetY() const {return m_Y;}
    int GetDirection() const {return m_Direction;}
    int GetNextDirection() const {return m_NextDirection;}
    int GetScaredStateTimer() const {return m_ScaredStateTimer;}
    bool GetScaredBlue() const {return m_IsScaredBlue;}
    bool GetScaredWhite() const {return m_IsScaredWhite;}

private:
    bool m_IsScaredBlue;
    bool m_IsScaredWhite;
    int m_X, m_Y;
    int m_Direction, m_NextDirection;
    int m_ScaredStateTimer;
};
