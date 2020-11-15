#pragma once

#include "../common/common.h"
#include <QString>
#include <QByteArray>

class DataPacket
{
public:
    DataPacket() = delete;

    inline static const QString TYPE = "T";
    inline static const QString PACMAN_X = "PX";
    inline static const QString PACMAN_Y = "PY";
    inline static const QString PACMAN_DIRECTION = "PD";
    inline static const QString GHOST_X = "GX";
    inline static const QString GHOST_Y = "GY";
    inline static const QString GHOST_DIRECTION = "GD";
    inline static const QString GAME_STATE = "GS";
    inline static const QString GHOST_SCARED_STATE = "GSS";
    inline static const QString OBJECT_TO_REMOVE = "O";
    inline static const QString PAYLOAD = "P";

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
