#pragma once

class Pacman
{
public:
    Pacman();
    void Reset();

    void SetX(int x) {m_X = x;}
    void SetY(int y) {m_Y = y;}
    void SetDirection(int dir) {m_Direction = dir;}
    void SetNextDirection(int dir) {m_NextDirection = dir;}

    int GetX() const {return m_X;}
    int GetY() const {return m_Y;}
    int GetDirection() const {return m_Direction;}
    int GetNextDirection() const {return m_NextDirection;}

private:
    int m_X;
    int m_Y;
    int m_Direction;
    int m_NextDirection;
};
