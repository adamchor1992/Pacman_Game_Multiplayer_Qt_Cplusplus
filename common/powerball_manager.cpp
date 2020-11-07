#include "powerball_manager.h"
#include <QPoint>
#include <QVector>

PowerballManager::PowerballManager()
{
    const int POWERBALL_X_1 = 35;
    const int POWERBALL_X_2 = 579;
    const int POWERBALL_Y_1 = 75;
    const int POWERBALL_Y_2 = 514;

    QPoint powerballPosition1 = QPoint(POWERBALL_X_1, POWERBALL_Y_1);
    QPoint powerballPosition2 = QPoint(POWERBALL_X_2, POWERBALL_Y_1);
    QPoint powerballPosition3 = QPoint(POWERBALL_X_1, POWERBALL_Y_2);
    QPoint powerballPosition4 = QPoint(POWERBALL_X_2, POWERBALL_Y_2);

    m_PowerballPositions.push_back(powerballPosition1);
    m_PowerballPositions.push_back(powerballPosition2);
    m_PowerballPositions.push_back(powerballPosition3);
    m_PowerballPositions.push_back(powerballPosition4);
}

