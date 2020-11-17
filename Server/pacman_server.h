#pragma once

#include <QObject>
#include <QtCore>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>

#include "pacman.h"
#include "ghost.h"
#include "../common/map.h"
#include "../common/foodball_manager.h"
#include "../common/powerball_manager.h"

class PacmanServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit PacmanServer();

private:
    const int PORT_NUMBER = 5000;
    const int GAME_TICK_TIMEOUT = 4;
    const int SEND_COORDINATES_TIMEOUT = 1;
    const int PLAYER_READY_TIMEOUT = 2000;
    const int PLAYER_CONNECTION_TIMEOUT = 3000;

    enum Client
    {
        CLIENT1 = 1,
        CLIENT2 = 2
    };

    QTcpSocket* m_pClientConnectionSocket1;
    QTcpSocket* m_pClientConnectionSocket2;
    QTcpSocket m_TempSocket;

    QTimer m_WaitForPlayerConnectionTimer;
    QTimer m_WaitForPlayerReadySignalTimer;
    QTimer m_GameTickTimer;
    QTimer m_SendGameDataToClientsTimer;

    Map m_Map;
    Pacman m_Pacman;
    Ghost m_Ghost;
    FoodballManager m_FoodballManager;
    PowerballManager m_PowerballManager;

    QByteArray m_dataPacketForClient;
    QByteArray m_CoordinatesOfObjectToRemove;
    QByteArray m_GameStatePacked;
    QByteArray m_IsGhostScaredWhitePacked;

    QVector<QPoint> m_RemainingFoodballPositions;
    QVector<QPoint> m_RemainingPowerballPositions;

    bool m_Player1Ready, m_Player2Ready;

    GameState m_GameState;

    void ServerStartListening();
    void SetUpAndFillMap();
    void SetUpAndPlacePlayers();
    void StartGame();
    void TogglePause();
    void PauseGame();
    void ResumeGame();
    void PrepareRestart();
    void CheckCollision();
    void ResetContainersAndVariables();
    void ReadSignalFromClient(QTcpSocket* tcpSocket, MovableCharacter& movableCharacter, bool& playerReady);
    void SetWinner(Character character);
    void SetGameState(GameState gameState) {m_GameState = gameState;}
    void SendGameDataToClient(Client client);
    void SendMessageToClient(Client client, QByteArray&& rawMessage);
    void SendCommandToClient(Client client, QByteArray&& rawMessage);

private slots:
    void AcceptConnection();
    void WaitForPlayerConnection();
    void WaitForPlayerReadySignals();
    void PackGameDataToSendToClients();
    void SendGameDataToClients();
    void GameTick();
    void ReadSignalFromClient1();
    void ReadSignalFromClient2();
    void connected1();
    void disconnected1();
    void connected2();
    void disconnected2();
};
