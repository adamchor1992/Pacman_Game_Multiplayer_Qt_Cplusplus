#include "../common/common.h"
#include "data_packet.h"
#include <QJsonObject>
#include <QJsonDocument>

int DataPacket::m_SequenceNumber = 1;
QByteArray DataPacket::m_Data;

QByteArray& DataPacket::Pack(int pacmanX,
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
        {TYPE, static_cast<int>(PacketType::GAME_DATA)},
        {PACMAN_X, pacmanX},
        {PACMAN_Y, pacmanY},
        {PACMAN_DIRECTION, static_cast<int>(pacmanDirection)},
        {GHOST_X, ghostX},
        {GHOST_Y, ghostY},
        {GHOST_DIRECTION, static_cast<int>(ghostDirection)},
        {GAME_STATE, static_cast<int>(gameState)},
        {GHOST_SCARED_STATE, static_cast<int>(ghostScaredState)},
        {OBJECT_TO_REMOVE, coordinatesOfObjectToRemove.toStdString().c_str()},
        {SEQUENCE_NUMBER, m_SequenceNumber}
    };

    QJsonDocument dataPacketJsonDocument(dataPacketJsonObject);

    m_Data = dataPacketJsonDocument.toJson(QJsonDocument::JsonFormat::Compact) + "\n";

    ++m_SequenceNumber;

    return m_Data;
}

QByteArray& DataPacket::Pack(PacketType packetType, QByteArray& payload)
{
    if((packetType != PacketType::COMMAND) && (packetType != PacketType::MESSAGE))
    {
        assert(false);
    }

    QJsonObject data
    {
        {TYPE, static_cast<int>(packetType)},
        {PAYLOAD, QJsonValue(payload.toStdString().c_str())},
        {SEQUENCE_NUMBER, m_SequenceNumber}
    };

    QJsonDocument dataDocument(data);

    m_Data = dataDocument.toJson(QJsonDocument::JsonFormat::Compact) + "\n";

    ++m_SequenceNumber;

    return m_Data;
}
