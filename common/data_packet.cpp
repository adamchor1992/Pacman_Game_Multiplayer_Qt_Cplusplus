#include "../common/common.h"
#include "data_packet.h"
#include <QJsonObject>
#include <QJsonDocument>

QByteArray DataPacket::Pack(int pacmanX,
                            int pacmanY,
                            Direction pacmanDirection,
                            int ghostX,
                            int ghostY,
                            Direction ghostDirection,
                            GameState gameState,
                            GhostScaredState ghostScaredState,
                            QByteArray coordinatesOfObjectToRemove)
{
    QJsonObject dataPacketJsonObject
    {
        {"Type", static_cast<int>(PacketType::GAME_DATA)},
        {"PX", pacmanX},
        {"PY", pacmanY},
        {"PD", static_cast<int>(pacmanDirection)},
        {"GX", ghostX},
        {"GY", ghostY},
        {"GD", static_cast<int>(ghostDirection)},
        {"GameState", static_cast<int>(gameState)},
        {"GhostScaredState", static_cast<int>(ghostScaredState)},
        {"Object to remove", coordinatesOfObjectToRemove.toStdString().c_str()}
    };

    QJsonDocument dataPacketJsonDocument(dataPacketJsonObject);

    return (dataPacketJsonDocument.toJson(QJsonDocument::JsonFormat::Compact) + "\n");
}

QByteArray DataPacket::Pack(PacketType packetType, QByteArray& payload)
{
    if((packetType != PacketType::COMMAND) && (packetType != PacketType::MESSAGE))
    {
        assert(false);
    }

    QJsonObject data
    {
        {"Type", static_cast<int>(packetType)},
        {"Payload", QJsonValue(payload.toStdString().c_str())}
    };

    QJsonDocument dataDocument(data);

    return (dataDocument.toJson(QJsonDocument::JsonFormat::Compact) + "\n");
}
