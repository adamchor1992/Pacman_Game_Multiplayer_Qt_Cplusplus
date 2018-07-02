#ifndef MAP_H
#define MAP_H

#include <QVector>
#include <QPixmap>

class Map
{
private:
    QPixmap map_background_picture;
    QVector<QPoint> pacman_paths;
    void LoadMapImage();
    void CreatePathPoints(int x_begin, int y_begin, int x_end, int y_end);
    QRectF boundingRect() const;

public:
    /*! Create path points where movement is possible on map */
    Map();
    /*! Get pacman possible paths in form of QVector<QPoint> */
    QVector<QPoint> getPacmanPaths() {return pacman_paths;}
    /*! Get pixel map of game map in form of QPixmap */
    QPixmap getMap_Background_Picture() {return map_background_picture;}
};

#endif // MAP_H
