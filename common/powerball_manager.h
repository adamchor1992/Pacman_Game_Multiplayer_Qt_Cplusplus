#pragma once

#include <QVector>
#include <QPoint>

class PowerballManager
{
public:
    static const int WIDTH = 15;
    static const int HEIGHT = 15;

    PowerballManager();
    QVector<QPoint> GeneratePowerballPositions();
};
