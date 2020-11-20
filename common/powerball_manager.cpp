#include "powerball_manager.h"
#include <QPoint>
#include <QVector>

PowerballManager::PowerballManager()
{

}

QVector<QPoint> PowerballManager::GeneratePowerballPositions()
{
    const int POWERBALL_X_1 = 35;
    const int POWERBALL_X_2 = 579;
    const int POWERBALL_Y_1 = 75;
    const int POWERBALL_Y_2 = 514;

    QPoint powerball1Position = QPoint(POWERBALL_X_1, POWERBALL_Y_1);
    QPoint powerball2Position = QPoint(POWERBALL_X_2, POWERBALL_Y_1);
    QPoint powerball3Position = QPoint(POWERBALL_X_1, POWERBALL_Y_2);
    QPoint powerball4Position = QPoint(POWERBALL_X_2, POWERBALL_Y_2);

    QVector<QPoint> powerballPositions;

    powerballPositions.push_back(powerball1Position);
    powerballPositions.push_back(powerball2Position);
    powerballPositions.push_back(powerball3Position);
    powerballPositions.push_back(powerball4Position);

    return powerballPositions;
}

