#pragma once

#include <QVector>
#include <QPoint>

class Map
{
public:
    Map();
    bool IsPointAvailable(QPoint point);
    QVector<QPoint> getPacmanPaths() const {return pacmanPaths;}

private:
    QVector<QPoint> pacmanPaths;
    void CreatePathPoints(int startX, int startY, int endX, int endY);
};
