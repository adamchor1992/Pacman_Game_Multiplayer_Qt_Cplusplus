#pragma once

#include <QVector>
#include <QPoint>

class Foodball
{
public:
    Foodball();
    QVector<QPoint> GetFoodballPositions() {return m_FoodballPositions;}

private:
    int m_FoodballCount;
    QVector<QPoint> m_FoodballPositions;
    QVector<QPoint> m_PacmanMapForReference;
    void CreateFoodballPositionsVector();
};
