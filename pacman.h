#pragma once

#include "common.h"

class Pacman
{
public:
    Pacman();
    void SetX(int x) {m_X = x;}
    void SetY(int y) {m_Y = y;}
    void SetDirection(Direction direction) {m_Direction = direction;}
    void SetNextDirection(Direction direction) {m_NextDirection = direction;}
    int GetX() const {return m_X;}
    int GetY() const {return m_Y;}
    Direction GetDirection() const {return m_Direction;}
    Direction GetNextDirection() const {return m_NextDirection;}
    void Reset();

private:
    int m_X, m_Y;
    Direction m_Direction, m_NextDirection;
};
