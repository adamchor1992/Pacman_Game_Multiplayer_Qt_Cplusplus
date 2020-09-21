#pragma once

#include <QVector>
#include <QPixmap>

class Map
{
private:
    QPixmap m_MapBackgroundPicture;
    QVector<QPoint> m_PacmanPaths;
    void LoadMapImage();
    void CreatePathPoints(int x_begin, int y_begin, int x_end, int y_end);

    //TO DO
    QRectF boundingRect() const;

public:
    Map();
    QVector<QPoint> GetPacmanPaths() {return m_PacmanPaths;}
    QPixmap GetMapBackgroundPicture() {return m_MapBackgroundPicture;}
};
