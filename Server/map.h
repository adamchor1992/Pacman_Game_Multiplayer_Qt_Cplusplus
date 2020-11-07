#pragma once

#include <QVector>
#include <QPoint>

class Map
{
public:
    Map();
    QVector<QPoint> GetPacmanPaths() const {return m_PacmanPaths;}
    bool IsPointAvailable(QPoint point) const;

private:
    QVector<QPoint> m_PacmanPaths;
    void CreatePathPoints(int startX, int startY, int endX, int endY);
};
