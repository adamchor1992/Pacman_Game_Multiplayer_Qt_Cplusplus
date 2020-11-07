#pragma once

#include <QString>
#include "../common/common.h"

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
               bool isGhostScaredBlue,
               bool isGhostScaredWhite,
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
    bool m_IsGhostScaredBlue;
    bool m_IsGhostScaredWhite;
    int m_Score;
    QByteArray m_Message;
};
