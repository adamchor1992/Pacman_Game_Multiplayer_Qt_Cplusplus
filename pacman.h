#pragma once

class Pacman
{
public:
    Pacman();
    void SetX(int x) {m_X = x;}
    void SetY(int y) {m_Y = y;}
    void SetDirection(int direction) {m_Direction = direction;}
    void SetNextDirection(int direction) {m_NextDirection = direction;}
    int GetX() const {return m_X;}
    int GetY() const {return m_Y;}
    int GetDirection() const {return m_Direction;}
    int GetNextDirection() const {return m_NextDirection;}
    void Reset();

private:
    int m_X, m_Y;
    int m_Direction, m_NextDirection;
};
