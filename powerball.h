#pragma once

#include <QVector>
#include <QPoint>

class Powerball
{
public:
    Powerball();
    QVector<QPoint> GetPowerballPositions() const {return m_PowerballPositions;}

private:
    QVector<QPoint> m_PowerballPositions;
};
