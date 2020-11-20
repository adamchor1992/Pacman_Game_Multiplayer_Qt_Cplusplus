#pragma once

#include <QVector>
#include <QPoint>
#include "map.h"

class FoodballManager
{
public:
    static const int WIDTH = 7;
    static const int HEIGHT = 7;

    FoodballManager();
    QVector<QPoint> GenerateFoodballPositions(const Map& map);
};
