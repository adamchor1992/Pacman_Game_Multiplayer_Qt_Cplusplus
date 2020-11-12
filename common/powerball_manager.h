#pragma once

#include <QVector>
#include <QPoint>

class PowerballManager
{
public:
public:
    static const int WIDTH = 15;
    static const int HEIGHT = 15;

    PowerballManager();
    QVector<QPoint> GetPowerballPositions() const {return m_PowerballPositions;}

private:
    QVector<QPoint> m_PowerballPositions;
};
