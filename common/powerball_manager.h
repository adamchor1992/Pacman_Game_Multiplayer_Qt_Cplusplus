#pragma once

#include <QVector>
#include <QPoint>

class PowerballManager
{
public:
    static const int DIAMETER = 15;

    PowerballManager();
    QVector<QPoint> GeneratePowerballPositions();
};
