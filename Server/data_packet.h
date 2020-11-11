#pragma once

#include "../common/common.h"
#include <QByteArray>

class DataPacket
{
public:
    DataPacket(Direction pacmanDirection,
               int pacmanX,
               int pacmanY,
               Direction ghostDirection,
               int ghostX,
               int ghostY,
               GameState gameState,
               GhostScaredState ghostScaredState,
               int score,
               QByteArray message);

    QByteArray Pack();

private:
    Direction m_PacmanDirection;
    int m_PacmanX;
    int m_PacmanY;
    Direction m_GhostDirection;
    int m_GhostX;
    int m_GhostY;
    GameState m_GameState;
    GhostScaredState m_GhostScaredState;
    int m_Score;
    QByteArray m_Message;
};
