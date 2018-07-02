#ifndef POWERBALL_H
#define POWERBALL_H

#include <QVector>
#include <QPoint>

class PowerBall
{
private:
    QVector<QPoint> powerballpositions;
public:
    /*! Fill powerballpositions vector with powerball positions represented as QPoints  */
    PowerBall();
    /*! Get powerball coordinates in form of QVector<QPoint> */
    QVector<QPoint> getPowerBallPositions() const {return powerballpositions;}
};

#endif // POWERBALL_H
