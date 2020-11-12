#pragma once

#include "../common/common.h"
#include <QByteArray>

class DataPacket
{
public:
    DataPacket() = delete;

    enum PacketType
    {
        GAME_DATA = 1,
        MESSAGE = 2,
        COMMAND = 3
    };

    static QByteArray Pack(int pacmanX,
                           int pacmanY,
                           Direction pacmanDirection,
                           int ghostX,
                           int ghostY,
                           Direction ghostDirection,
                           GameState gameState,
                           GhostScaredState ghostScaredState,
                           QByteArray coordinatesOfObjectToRemove);

    static QByteArray Pack(PacketType packetType,
                           QByteArray& payload);
};
