#pragma once

#include <QVector>
#include <QPoint>

class PowerballManager
{
public:
    PowerballManager();
    QVector<QPoint> GetPowerballPositions() const {return m_PowerballPositions;}

private:
    QVector<QPoint> m_PowerballPositions;
};
