#pragma once

#include <QVector>
#include <QPoint>

class Foodball
{
public:
    Foodball();
    QVector<QPoint> GetFoodballPositions() const {return foodballPositions;}

private:
    int foodballCount;
    QVector<QPoint> foodballPositions;
    QVector<QPoint> pacmanMapForReference;
    void CreateFoodballPositionsVector();
};
