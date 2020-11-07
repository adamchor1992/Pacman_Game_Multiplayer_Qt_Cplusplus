#include "movable_character.h"

MovableCharacter::MovableCharacter()
{

}

void MovableCharacter::Move(Map const& map)
{
    QPoint point;

    int x = m_X;
    int y = m_Y;
    Direction direction = m_Direction;
    Direction nextDirection = m_NextDirection;

    if(nextDirection != direction)
    {
        switch(nextDirection)
        {
        case Direction::LEFT:
            point.setX(x - 1);
            point.setY(y);

            if(map.IsPointAvailable(point))
            {
                direction = nextDirection;
            }
            break;

        case Direction::UP:
            point.setX(x);
            point.setY(y - 1);
            if(map.IsPointAvailable(point))
            {
                direction = nextDirection;
            }
            break;

        case Direction::DOWN:
            point.setX(x);
            point.setY(y + 1);
            if(map.IsPointAvailable(point))
            {
                direction = nextDirection;
            }
            break;

        case Direction::RIGHT:
            point.setX(x + 1);
            point.setY(y);
            if(map.IsPointAvailable(point))
            {
                direction = nextDirection;
            }
            break;

        default:
            break;
        }
    }

    switch(direction)
    {
    case Direction::LEFT:
        point.setX(x - 1);
        point.setY(y);
        SetDirection(direction);

        if(map.IsPointAvailable(point))
        {
            x = x - 1;
        }

        break;

    case Direction::UP:
        point.setX(x);
        point.setY(y - 1);
        SetDirection(direction);

        if(map.IsPointAvailable(point))
        {
            y= y - 1;
        }

        break;

    case Direction::DOWN:
        point.setX(x);
        point.setY(y + 1);
        SetDirection(direction);

        if(map.IsPointAvailable(point))
        {
            y= y + 1;
        }

        break;

    case Direction::RIGHT:
        point.setX(x + 1);
        point.setY(y);
        SetDirection(direction);

        if(map.IsPointAvailable(point))
        {
            x = x + 1;
        }

        break;

    default:
        break;
    }

    /*Teleportation when reached left boundary of middle horizontal line*/
    if(x == 0 && y == 318)
    {
        x = 613;
    }

    /*Teleportation when reached right boundary of middle horizontal line*/
    if(x == 614 && y == 318)
    {
        x = 1;
    }

    SetX(x);
    SetY(y);
}
