#pragma once

#include <QVector>
#include <QPoint>
#include "map.h"

class FoodballManager
{
public:
    static const int DIAMETER = 7;

    FoodballManager();
    QVector<QPoint> GenerateFoodballPositions(const Map& map);
};
