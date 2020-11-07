#pragma once

#include "../common/common.h"
#include <map.h>

class MovableCharacter
{
public:
    MovableCharacter();
    void SetX(int x) {m_X = x;}
    void SetY(int y) {m_Y = y;}
    void SetDirection(Direction direction) {m_Direction = direction;}
    void SetNextDirection(Direction direction) {m_NextDirection = direction;}
    int GetX() const {return m_X;}
    int GetY() const {return m_Y;}
    Direction GetDirection() const {return m_Direction;}
    Direction GetNextDirection() const {return m_NextDirection;}
    void Move(Map const& map);

    virtual void Reset() = 0;

protected:
    int m_X, m_Y;
    Direction m_Direction, m_NextDirection;
};
