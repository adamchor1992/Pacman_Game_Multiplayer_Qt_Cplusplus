#pragma once

#include <QVector>
#include <QPoint>

class PowerBall
{
public:
    PowerBall();
    QVector<QPoint> GetPowerBallPositions() const {return m_PowerballPositions;}

private:
    QVector<QPoint> m_PowerballPositions;
};
