#pragma once

#include <QVector>
#include <QPoint>

class FoodballManager
{
public:
    FoodballManager();
    QVector<QPoint> GetFoodballPositions() {return m_FoodballPositions;}

private:
    int m_FoodballCount;
    QVector<QPoint> m_FoodballPositions;
    QVector<QPoint> m_PacmanMapForReference;
    void CreateFoodballPositionsVector();
};
