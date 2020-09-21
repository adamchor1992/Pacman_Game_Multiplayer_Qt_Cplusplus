#include "map.h"

Map::Map()
{
    LoadMapImage();

    //HORIZONTAL LINES
    //LINE 1
    CreatePathPoints(35, 35, 274, 35);
    CreatePathPoints(340, 35, 579, 35);

    //LINE 2
    CreatePathPoints(35, 121, 579, 121);

    //LINE 3
    CreatePathPoints(35, 187, 144, 187);
    CreatePathPoints(209, 187, 274, 187);
    CreatePathPoints(340, 187, 406, 187);
    CreatePathPoints(470, 187, 579, 187);

    //LINE 4
    CreatePathPoints(209, 252, 406, 252);

    //LINE 5 - MIDDLE LINE
    CreatePathPoints(0, 318, 209, 318);
    CreatePathPoints(406, 318, 614, 318);

    //LINE 6
    CreatePathPoints(209, 384,406, 384);

    //LINE 7
    CreatePathPoints(35, 449, 274, 449);
    CreatePathPoints(340, 449, 579, 449);

    //LINE 8
    CreatePathPoints(35, 514, 79, 514);
    CreatePathPoints(144, 514, 470, 514);
    CreatePathPoints(536, 514, 579, 514);

    //LINE 9
    CreatePathPoints(35, 580, 144, 580);
    CreatePathPoints(209, 580, 274, 580);
    CreatePathPoints(340, 580, 406, 580);
    CreatePathPoints(470, 580, 579, 580);

    //LINE 10
    CreatePathPoints(35, 645, 579, 645);

    //VERTICAL LINES
    //LINE 1
    CreatePathPoints(35, 35, 35, 187);
    CreatePathPoints(35, 449, 35, 514);
    CreatePathPoints(35, 580, 35, 645);

    //LINE 2
    CreatePathPoints(79, 514, 79, 580);

    //LINE 3
    CreatePathPoints(144, 35, 144, 580);

    //LINE 4
    CreatePathPoints(209, 121, 209, 187);
    CreatePathPoints(209, 252, 209, 449);
    CreatePathPoints(209, 514, 209, 580);

    //LINE 5
    CreatePathPoints(274, 35, 274, 121);
    CreatePathPoints(274, 187, 274, 252);
    CreatePathPoints(274, 449, 274, 514);
    CreatePathPoints(274, 580, 274, 645);

    //LINE 6
    CreatePathPoints(340, 35, 340, 121);
    CreatePathPoints(340, 187, 340, 252);
    CreatePathPoints(340, 449, 340, 514);
    CreatePathPoints(340, 580, 340, 645);

    //LINE 7
    CreatePathPoints(406, 121, 406, 187);
    CreatePathPoints(406, 252, 406, 449);
    CreatePathPoints(406, 514, 406, 580);

    //LINE 8
    CreatePathPoints(470, 35, 470, 580);

    //LINE 9
    CreatePathPoints(536, 514, 536, 580);

    //LINE 10
    CreatePathPoints(579, 35, 579, 187);
    CreatePathPoints(579, 449, 579, 514);
    CreatePathPoints(579, 580, 579, 645);
}

void Map::LoadMapImage()
{
    m_MapBackgroundPicture.load(":/images/pac_map.png");
}

void Map::CreatePathPoints(int startX, int startY, int endX, int endY)
{
    QPoint p;

    if (startX == endX) //vertical line condition
    {
        if (startY < endY) //vertical line from y_begin to y_end
        {
            for (int y = startY; y <= endY; y++)
            {
                p.setX(startX);
                p.setY(y);
                if (!m_PacmanPaths.contains(p))
                {
                    m_PacmanPaths.push_front(p);
                }
            }
        }

        else if(startY == endY) //single point
        {
                p.setX(startX);
                p.setY(startY);
                if (!m_PacmanPaths.contains(p))
                {
                    m_PacmanPaths.push_front(p);
                }
        }
    }

    if (startY == endY) //horizontal line condition
    {
        if (startX < endX) //horizontal line from x_begin to x_end
        {
            for (int x = startX; x <= endX; x++)
            {
                p.setX(x);
                p.setY(startY);
                if (!m_PacmanPaths.contains(p))
                {
                    m_PacmanPaths.push_front(p);
                }
            }
        }
        else //single point
        {
                p.setX(startX);
                p.setY(startY);
                if (!m_PacmanPaths.contains(p))
                {
                    m_PacmanPaths.push_front(p);
                }
        }
    }
}
