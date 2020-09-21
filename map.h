#pragma once

#include <QVector>
#include <QPixmap>

class Map
{
public:
    Map();
    QVector<QPoint> GetPacmanPaths() {return m_PacmanPaths;}
    QPixmap GetMapBackgroundPicture() {return m_MapBackgroundPicture;}

private:
    QPixmap m_MapBackgroundPicture;
    QVector<QPoint> m_PacmanPaths;
    void LoadMapImage();
    void CreatePathPoints(int startX, int startY, int endX, int endY);
};
