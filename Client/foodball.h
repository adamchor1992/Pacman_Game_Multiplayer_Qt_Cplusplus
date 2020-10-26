#pragma once

#include <QVector>
#include <QPoint>

class FoodBall
{
public:
    FoodBall();
    QVector<QPoint> GetFoodBallPositions() {return m_FoodballPositions;}

private:
    int m_FoodbalCount;
    QVector<QPoint> m_FoodballPositions;
    QVector<QPoint> m_PacmanMapForReference;
    void CreateFoodballPositionsVector();
};
