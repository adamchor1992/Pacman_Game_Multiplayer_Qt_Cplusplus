#ifndef MAP_H
#define MAP_H

#include <QVector>
#include <QPoint>

class Map
{
private:
    QVector<QPoint> pacman_paths;
    void CreatePathPoints(int x_begin, int y_begin, int x_end, int y_end);
public:
    /*! Create path points where movement is possible on map */
    Map();
    /*! Check if point is present in map paths and return bool */
    bool IsPointAvailable(QPoint point);
    /*! Get pacman possible paths in form of QVector<QPoint> */
    QVector<QPoint> getPacmanPaths() const {return pacman_paths;}
};

#endif // MAP_H
