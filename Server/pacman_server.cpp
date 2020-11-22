#include "pacman_server.h"
#include "../common/data_packet.h"
#include "../common/log_manager.h"

PacmanServer::PacmanServer()
{
    qDebug() << "PacmanServer";

    /*Setting up temporary socket*/
    m_pClientConnectionSocket1 = &m_TempSocket;

    ServerStartListening();

    /*Wait for new connections*/
    connect(this, &QTcpServer::newConnection, this, &PacmanServer::AcceptConnection, Qt::UniqueConnection);

    /*Create game environment*/
    SetUpAndFillMap();
    SetUpAndPlacePlayers();

    SetGameState(GameState::BeforeFirstRun);
}

void PacmanServer::ServerStartListening()
{
    qDebug() << "ServerStartListening";

    if(listen(QHostAddress::Any, PORT_NUMBER))
    {
        qDebug() << "Server started, awaiting connections";
    }
    else
    {
        qDebug() << "Server could not start: " << errorString() << "\nPlease restart server";
        exit(0);
    }
}

void PacmanServer::SetUpAndFillMap()
{
    qDebug() << "SetUpAndFillMap";

    m_RemainingFoodballPositions = m_FoodballManager.GenerateFoodballPositions(m_Map);
    m_RemainingPowerballPositions = m_PowerballManager.GeneratePowerballPositions();
}

void PacmanServer::SetUpAndPlacePlayers()
{
    qDebug() << "SetUpAndPlacePlayers";

    m_Pacman.Reset();
    m_Ghost.Reset();

    m_Player1Ready = false;
    m_Player2Ready = false;
}

void PacmanServer::StartGame()
{
    qDebug() << "StartGame";

    connect(&m_GameTickTimer, &QTimer::timeout, this, &PacmanServer::GameTick, Qt::UniqueConnection);

    connect(&m_SendGameDataToClientsTimer, &QTimer::timeout, this, &PacmanServer::SendGameDataToClients, Qt::UniqueConnection);

    m_GameTickTimer.start(GAME_TICK_TIMEOUT);
    m_SendGameDataToClientsTimer.start(SEND_COORDINATES_TIMEOUT);

    SetGameState(GameState::Running);
}

void PacmanServer::TogglePause()
{
    qDebug() << "TogglePause";

    if(m_GameState == GameState::Paused)
    {
        ResumeGame();
    }
    else if(m_GameState == GameState::Running)
    {
        PauseGame();
    }
    else
    {
        assert(false);
    }
}

void PacmanServer::PauseGame()
{
    qDebug() << "PauseGame";

    SetGameState(GameState::Paused);

    m_GameTickTimer.stop();
}

void PacmanServer::ResumeGame()
{
    qDebug() << "ResumeGame";

    SetGameState(GameState::Running);

    m_GameTickTimer.start();
}

void PacmanServer::ResetContainersAndVariables()
{
    qDebug() << "ResetContainersAndVariables";

    m_Pacman.Reset();
    m_Ghost.Reset();

    m_RemainingFoodballPositions = m_FoodballManager.GenerateFoodballPositions(m_Map);
    m_RemainingPowerballPositions = m_PowerballManager.GeneratePowerballPositions();
}

void PacmanServer::PrepareRestart()
{
    qDebug() << "PrepareRestart";

    ResetContainersAndVariables();

    connect(&m_WaitForPlayerReadySignalTimer, &QTimer::timeout, this, &PacmanServer::WaitForPlayerReadySignals, Qt::UniqueConnection);
    m_WaitForPlayerReadySignalTimer.start(PLAYER_READY_TIMEOUT);

    m_Player1Ready = false;
    m_Player2Ready = false;

    qDebug() << "Game restarted on server, players not ready yet";
}

void PacmanServer::AcceptConnection()
{
    qDebug() << "AcceptConnection";

    /*If there is new connection*/
    if(m_pClientConnectionSocket1->state() == QAbstractSocket::UnconnectedState)
    {
        m_pClientConnectionSocket1 = nextPendingConnection();

        connect(m_pClientConnectionSocket1, &QTcpSocket::connected, this, &PacmanServer::connected1, Qt::UniqueConnection);
        connect(m_pClientConnectionSocket1, &QTcpSocket::disconnected, this, &PacmanServer::disconnected1, Qt::UniqueConnection);
        connect(m_pClientConnectionSocket1, &QTcpSocket::readyRead, this, &PacmanServer::ReadSignalFromClient1, Qt::UniqueConnection);

        /*Set up timer waiting for players signals*/
        connect(&m_WaitForPlayerConnectionTimer, &QTimer::timeout, this, &PacmanServer::WaitForPlayerConnection, Qt::UniqueConnection);
        m_WaitForPlayerConnectionTimer.start(PLAYER_CONNECTION_TIMEOUT);

        qDebug() << "Connection assigned to socket 1 - PACMAN";
        SendMessageToClient(CLIENT1, "You are PLAYER 1 - PACMAN");
    }
    else
    {
        m_pClientConnectionSocket2 = nextPendingConnection();

        connect(m_pClientConnectionSocket2, &QTcpSocket::connected, this, &PacmanServer::connected2, Qt::UniqueConnection);
        connect(m_pClientConnectionSocket2, &QTcpSocket::disconnected, this, &PacmanServer::disconnected2, Qt::UniqueConnection);
        connect(m_pClientConnectionSocket2, &QTcpSocket::readyRead, this, &PacmanServer::ReadSignalFromClient2, Qt::UniqueConnection);

        m_WaitForPlayerConnectionTimer.stop();
        disconnect(&m_WaitForPlayerConnectionTimer, &QTimer::timeout, this, &PacmanServer::WaitForPlayerConnection);

        connect(&m_WaitForPlayerReadySignalTimer, &QTimer::timeout, this, &PacmanServer::WaitForPlayerReadySignals, Qt::UniqueConnection);
        m_WaitForPlayerReadySignalTimer.start(PLAYER_READY_TIMEOUT);

        qDebug() << "Connection assigned to socket 2 - GHOST";
        SendMessageToClient(CLIENT2, "You are PLAYER 2 - GHOST");
    }
}

void PacmanServer::WaitForPlayerConnection()
{
    qDebug() << "WaitForPlayerConnection";

    qDebug() << "Waiting for player 2";
    SendMessageToClient(CLIENT1, "You are connected, waiting for another player");
}

void PacmanServer::WaitForPlayerReadySignals()
{
    qDebug() << "WaitForPlayerReadySignals";

    if(!m_Player1Ready && !m_Player2Ready)
    {
        qDebug("Waiting for players ready signals");
        SendMessageToClient(CLIENT1, "You are not ready, please press space");
        SendMessageToClient(CLIENT2, "You are not ready, please press space");
    }

    if(m_Player1Ready && !m_Player2Ready)
    {
        qDebug("Player 1 - ready Player 2 - NOT READY");
        SendMessageToClient(CLIENT1, "You are ready, awaiting other player connection");
        SendMessageToClient(CLIENT2, "You are not ready, please press space");
    }

    if(!m_Player1Ready && m_Player2Ready)
    {
        qDebug("Player 2 - NOT READY Player 1 - ready");
        SendMessageToClient(CLIENT1, "You are not ready, please press space");
        SendMessageToClient(CLIENT2, "You are ready, awaiting other player connection");
    }

    if(m_Player1Ready && m_Player2Ready)
    {
        qDebug("Player 1 and Player 2 ready");

        StartGame();

        SendCommandToClient(CLIENT1, "Game started");
        SendCommandToClient(CLIENT2, "Game started");

        m_WaitForPlayerReadySignalTimer.stop();
        disconnect(&m_WaitForPlayerReadySignalTimer, &QTimer::timeout, this, &PacmanServer::WaitForPlayerReadySignals);
    }
}

void PacmanServer::SendMessageToClient(Client client, QByteArray&& rawMessage)
{
    qDebug() << "SendMessageToClient";

    QTcpSocket* tcpSocket = nullptr;

    if(client == CLIENT1)
    {
        qDebug() << "Send message to client 1: " << rawMessage;
        tcpSocket = m_pClientConnectionSocket1;
    }
    else if(client == CLIENT2)
    {
        qDebug() << "Send message to client 2: " << rawMessage;
        tcpSocket = m_pClientConnectionSocket2;
    }
    else
    {
        assert(false);
    }

    QByteArray const& packedMessage = DataPacket::Pack(DataPacket::MESSAGE, rawMessage);

    if(tcpSocket->state() == QTcpSocket::ConnectedState)
    {
        tcpSocket->write(packedMessage, packedMessage.size());
    }
    else
    {
        static int delay = 0;

        if(delay % 10000 == 0)
        {
            qDebug() << "WARNING, MESSAGE NOT SENT BECAUSE ONE OF SOCKETS IS NOT CONNECTED";
        }

        ++delay;
    }
}

void PacmanServer::SendCommandToClient(Client client, QByteArray&& rawMessage)
{
    qDebug() << "SendCommandToClient";

    QTcpSocket* tcpSocket = nullptr;

    if(client == CLIENT1)
    {
        qDebug() << "Send command to client 1: " << rawMessage;
        tcpSocket = m_pClientConnectionSocket1;
    }
    else if(client == CLIENT2)
    {
        qDebug() << "Send command to client 2: " << rawMessage;
        tcpSocket = m_pClientConnectionSocket2;
    }
    else
    {
        assert(false);
    }

    QByteArray const& packedMessage = DataPacket::Pack(DataPacket::COMMAND, rawMessage);

    if(tcpSocket->state() == QTcpSocket::ConnectedState)
    {
        tcpSocket->write(packedMessage, packedMessage.size());
    }
    else
    {
        static int delay = 0;

        if(delay % 10000 == 0)
        {
            qDebug() << "WARNING, COMMAND NOT SENT TO CLIENT " << client << " BECAUSE THE SOCKET IS NOT CONNECTED";
        }

        ++delay;
    }
}

void PacmanServer::SendGameDataToClients()
{
    QByteArray const& dataPacketForClient = DataPacket::Pack(m_Pacman.GetX(),
                                                             m_Pacman.GetY(),
                                                             m_Pacman.GetDirection(),
                                                             m_Ghost.GetX(),
                                                             m_Ghost.GetY(),
                                                             m_Ghost.GetDirection(),
                                                             m_GameState,
                                                             m_Ghost.GetScaredState(),
                                                             m_CoordinatesOfObjectToRemove);

    SendGameDataToClient(CLIENT1, dataPacketForClient);
    SendGameDataToClient(CLIENT2, dataPacketForClient);

    LogManager::LogToFile("SN: " + std::to_string(DataPacket::GetSequenceNumber()));
}

void PacmanServer::SendGameDataToClient(Client client, QByteArray const& dataPacket)
{
    QTcpSocket* tcpSocket = nullptr;

    if(client == CLIENT1)
    {
        tcpSocket = m_pClientConnectionSocket1;
    }
    else if(client == CLIENT2)
    {
        tcpSocket = m_pClientConnectionSocket2;
    }
    else
    {
        assert(false);
    }

    if(tcpSocket->state() == QTcpSocket::ConnectedState)
    {
        tcpSocket->write(dataPacket, dataPacket.size());
    }
    else
    {
        static int delay = 0;

        if(delay % 10000 == 0)
        {
            qDebug() << "WARNING, GAME DATA NOT SENT TO CLIENT " << client << " BECAUSE THE SOCKET IS NOT CONNECTED";
        }

        ++delay;
    }
}

void PacmanServer::CheckCollision()
{
    const int PACMAN_X = m_Pacman.GetX();
    const int PACMAN_Y = m_Pacman.GetY();
    const int GHOST_X = m_Ghost.GetX();
    const int GHOST_Y = m_Ghost.GetY();

    auto AreCirclesIntersecting = [](int x1, int y1, int x2, int y2, int radius){return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < (4 * radius * radius);};

    if(AreCirclesIntersecting(PACMAN_X, PACMAN_Y, GHOST_X, GHOST_Y, MovableCharacter::RADIUS))
    {
        if(m_Ghost.GetScaredState() != GhostScaredState::NO_SCARED)
        {
            SetWinner(PACMAN);
            qDebug() << "Pacman wins";
        }
        else
        {
            SetWinner(GHOST);
            qDebug() << "Ghost wins";
        }

        /*RESET PACMAN AND GHOST POSITIONS*/
        SetUpAndPlacePlayers();

        qDebug() << "RESET PACMAN AND GHOST POSITIONS";

        PrepareRestart();
    }
}

void PacmanServer::GameTick()
{
    int pacmanX = m_Pacman.GetX();
    int pacmanY = m_Pacman.GetY();

    int remainingFoodballCount = m_RemainingFoodballPositions.size();
    int remainingPowerballCount = m_RemainingPowerballPositions.size();

    /*Eating foodballs*/
    for(int i = 0; i < remainingFoodballCount; i++)
    {
        if(pacmanX == m_RemainingFoodballPositions.at(i).x() && pacmanY == m_RemainingFoodballPositions.at(i).y())
        {
            m_CoordinatesOfObjectToRemove = QByteArray::number(m_RemainingFoodballPositions.at(i).x()) + "," + QByteArray::number(m_RemainingFoodballPositions.at(i).y());
            m_RemainingFoodballPositions.remove(i);

            qDebug() << "Remaining foodball positions: " << m_RemainingFoodballPositions.size();

            break;
        }
    }

    /*Eating powerballs*/
    for(int i = 0; i < remainingPowerballCount; i++)
    {
        if(pacmanX == m_RemainingPowerballPositions.at(i).x() && pacmanY == m_RemainingPowerballPositions.at(i).y())
        {
            m_CoordinatesOfObjectToRemove = QByteArray::number(m_RemainingPowerballPositions.at(i).x()) + "," + QByteArray::number(m_RemainingPowerballPositions.at(i).y());
            m_RemainingPowerballPositions.remove(i);
            m_Ghost.SetScaredState(GhostScaredState::SCARED_BLUE);
            break;
        }
    }

    if(m_RemainingFoodballPositions.size() == 0)
    {
        SetWinner(PACMAN);
        qDebug() << "Pacman wins";

        /*RESET PACMAN AND GHOST POSITIONS*/
        SetUpAndPlacePlayers();

        qDebug() << "RESET PACMAN AND GHOST POSITIONS";

        PrepareRestart();
    }

    if(m_Ghost.GetScaredState() == GhostScaredState::SCARED_BLUE || m_Ghost.GetScaredState() == GhostScaredState::SCARED_WHITE)
    {
        m_Ghost.IncrementScaredStateTimer();

        if(m_Ghost.GetScaredStateTimer() == Ghost::SCARED_WHITE_THRESHOLD)
        {
            m_Ghost.SetScaredState(GhostScaredState::SCARED_WHITE);
        }

        if(m_Ghost.GetScaredStateTimer() == Ghost::SCARED_TIMEOUT)
        {
            m_Ghost.SetScaredState(GhostScaredState::NO_SCARED);

            m_Ghost.SetScaredStateTimer(0);
        }
    }

    m_Pacman.Move(m_Map);
    m_Ghost.Move(m_Map);

    CheckCollision();
}

void PacmanServer::ReadSignalFromClient(QTcpSocket* tcpSocket, MovableCharacter& movableCharacter, bool& playerReady)
{
    char characterReceivedFromClient = 0;

    tcpSocket->getChar(&characterReceivedFromClient);

    if((m_GameState == GameState::BeforeFirstRun || m_GameState == GameState::PacmanWin || m_GameState == GameState::GhostWin) && characterReceivedFromClient == SIGNAL_START)
    {
        playerReady = true;
        return;
    }

    if((m_GameState == GameState::Running || m_GameState == GameState::Paused) && characterReceivedFromClient == SIGNAL_PAUSE)
    {
        TogglePause();
        return;
    }

    movableCharacter.SetNextDirection(static_cast<Direction>(characterReceivedFromClient - '0'));
}

void PacmanServer::SetWinner(Character character)
{
    qDebug() << "SetWinner";

    if(character == PACMAN)
    {
        SetGameState(GameState::PacmanWin);
    }
    else if(character == GHOST)
    {
        SetGameState(GameState::GhostWin);
    }
    else
    {
        assert(false);
    }
}

/*Socket 1 wrapper*/
void PacmanServer::ReadSignalFromClient1()
{
    qDebug() << "ReadSignalFromClient1";

    ReadSignalFromClient(m_pClientConnectionSocket1, m_Pacman, m_Player1Ready);
}

/*Socket 2 wrapper*/
void PacmanServer::ReadSignalFromClient2()
{
    qDebug() << "ReadSignalFromClient2";

    ReadSignalFromClient(m_pClientConnectionSocket2, m_Ghost, m_Player2Ready);
}

void PacmanServer::connected1()
{
    qDebug() << "Client 1 Connected!";
}

void PacmanServer::disconnected1()
{
    qDebug() << "Client 1 Disconnected! Please restart application";
}

void PacmanServer::connected2()
{
    qDebug() << "Client 2 Connected!";
}

void PacmanServer::disconnected2()
{
    qDebug() << "Client 2 Disconnected! Please restart application";
}
