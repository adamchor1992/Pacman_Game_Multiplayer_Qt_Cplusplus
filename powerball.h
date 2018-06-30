#ifndef POWERBALL_H
#define POWERBALL_H

#include <QVector>
#include <QPoint>

class PowerBall
{
private:
    QVector<QPoint> powerballpositions;
public:
    PowerBall();
    QVector<QPoint> getPowerBallPositions() const {return powerballpositions;}
};

#endif // POWERBALL_H
