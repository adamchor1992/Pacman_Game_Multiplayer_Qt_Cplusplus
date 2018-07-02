#ifndef FOODBALL_H
#define FOODBALL_H

#include <QVector>
#include <QPoint>

class FoodBall
{
private:
    int foodballcount;
    QVector<QPoint> foodballpositions;
    QVector<QPoint> pacmanmapforreference;
    void CreateFoodballPositionsVector();

public:
    /*! Initialize member variables and fill foodballpositions vector with QPoints representing foodballs coordinates on map */
    FoodBall();
    /*! Get generated foodballpositions vector in form of QVector<QPoint> */
    QVector<QPoint> getFoodBallPositions() {return foodballpositions;}
};

#endif // FOODBALL_H
