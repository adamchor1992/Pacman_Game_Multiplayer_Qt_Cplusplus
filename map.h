#ifndef MAP_H
#define MAP_H

#include <QVector>
#include <QPoint>

class Map
{
private:
    QVector<QPoint> pacman_paths;

public:
    Map();
    void CreatePathPoints(int x_begin, int y_begin, int x_end, int y_end);
    bool IsPointAvailable(QPoint point);
    QVector<QPoint> getPacmanPaths() const {return pacman_paths;}
};

#endif // MAP_H
