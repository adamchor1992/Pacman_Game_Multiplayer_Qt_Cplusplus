#pragma once

#include <QVector>
#include <QPoint>

class FoodBall
{
private:
    int m_FoodbalCount;
    QVector<QPoint> m_FoodballPositions;
    QVector<QPoint> m_PacmanMapForReference;
    void CreateFoodballPositionsVector();

public:
    FoodBall();
    QVector<QPoint> GetFoodBallPositions() {return m_FoodballPositions;}
};
