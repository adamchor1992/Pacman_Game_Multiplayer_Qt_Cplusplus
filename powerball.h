#pragma once

#include <QVector>
#include <QPoint>

class PowerBall
{
private:
    QVector<QPoint> m_PowerballPositions;
public:
    PowerBall();
    QVector<QPoint> GetPowerBallPositions() {return m_PowerballPositions;}
};
