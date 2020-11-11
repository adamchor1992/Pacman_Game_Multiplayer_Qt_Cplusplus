#include "../common/common.h"
#include "data_packet.h"

DataPacket::DataPacket(Direction pacmanDirection,
                       int pacmanX,
                       int pacmanY,
                       Direction ghostDirection,
                       int ghostX,
                       int ghostY,
                       GameState gameState,
                       GhostScaredState ghostScaredState,
                       int score,
                       QByteArray message)
{
    m_PacmanDirection = pacmanDirection;
    m_PacmanX = pacmanX;
    m_PacmanY = pacmanY;

    m_GhostDirection = ghostDirection;
    m_GhostX = ghostX;
    m_GhostY = ghostY;

    m_GameState = gameState;

    m_GhostScaredState = ghostScaredState;

    m_Score = score;

    m_Message = message;
}

QByteArray DataPacket::Pack()
{
    QByteArray fullDataPacket;

    QByteArray pacmanDirectionByteArray = QByteArray::number(static_cast<int>(m_PacmanDirection));
    QByteArray pacmanXByteArray = QByteArray::number(m_PacmanX);
    QByteArray pacmanYByteArray = QByteArray::number(m_PacmanY);

    QByteArray ghostDirectionByteArray = QByteArray::number(static_cast<int>(m_GhostDirection));
    QByteArray ghostXByteArray = QByteArray::number(m_GhostX);
    QByteArray ghostYByteArray = QByteArray::number(m_GhostY);

    QByteArray pacmanCoordinatesPackedByteArray = "{{D1" + pacmanDirectionByteArray + "[x1:" + pacmanXByteArray + "," + "y1:" + pacmanYByteArray + "];";
    QByteArray ghostCoordinatesPackedByteArray = "D2" + ghostDirectionByteArray + "[x2:" + ghostXByteArray + "," + "y2:" + ghostYByteArray + "]},";

    QByteArray gameStatePacked;

    if(m_GameState == GameState::BeforeFirstRun)
    {
        gameStatePacked = "{[S:0],";
    }
    else if(m_GameState == GameState::Running)
    {
        gameStatePacked = "{[S:1],";
    }
    else if(m_GameState == GameState::Paused)
    {
        gameStatePacked = "{[S:2],";
    }
    else if(m_GameState == GameState::Aborted)
    {
        gameStatePacked = "{[S:3],";
    }
    else if(m_GameState == GameState::PacmanWin)
    {
        gameStatePacked = "{[S:4],";
    }
    else if(m_GameState == GameState::GhostWin)
    {
        gameStatePacked = "{[S:5],";
    }

    QByteArray ghostScaredStatePacked;

    if(m_GhostScaredState == GhostScaredState::NO_SCARED)
    {
        ghostScaredStatePacked = "[G:N],";
    }
    else if(m_GhostScaredState == GhostScaredState::SCARED_BLUE)
    {
        ghostScaredStatePacked = "[G:S],";
    }
    else if(m_GhostScaredState == GhostScaredState::SCARED_WHITE)
    {
        ghostScaredStatePacked = "[G:W],";
    }
    else
    {
        assert(false);
    }

    QByteArray pointsPackedByteArray = "[P:" + QByteArray::number(m_Score) + "]},";

    QByteArray messagePackedByteArray = "{'" + m_Message + "'}}";

    fullDataPacket = pacmanCoordinatesPackedByteArray +
            ghostCoordinatesPackedByteArray +
            gameStatePacked +
            ghostScaredStatePacked +
            pointsPackedByteArray +
            messagePackedByteArray;

    return fullDataPacket;
}
