#pragma once

#include <QVector>
#include <QPoint>

class FoodballManager
{
public:
    static const int WIDTH = 7;
    static const int HEIGHT = 7;

    FoodballManager();
    QVector<QPoint> GetFoodballPositions() const {return m_FoodballPositions;}

private:
    int m_FoodballCount;
    QVector<QPoint> m_FoodballPositions;
    QVector<QPoint> m_PacmanMapForReference;
    void CreateFoodballPositionsVector();
};
