#include "pacman_server.h"
#include "data_packet.h"

PacmanServer::PacmanServer(QObject *parent) : QObject(parent)
{
    //setting up temporary socket

    m_pServerSocket1 = &m_TempSocket;

    ServerStartListening();

    //wait for new connections
    m_ConnectionObject_NewConnection = connect(&m_Server, &QTcpServer::newConnection, this, &PacmanServer::AcceptConnection, Qt::UniqueConnection);

    //create game environment
    SetUpAndFillMap();
    SetUpAndPlacePlayers();

    m_GameState = GameState::BeforeFirstRun;
}

void PacmanServer::ServerStartListening()
{
    if(m_Server.listen(QHostAddress::Any, PORT_NUMBER))
    {
        qDebug() << "Server started, awaiting connections";
    }
    else
    {
        qDebug() << "Server could not start: " << m_Server.errorString();
        qDebug() << "Please restart server";

        exit(0);
    }
}

void PacmanServer::SetUpAndFillMap()
{
    m_FoodballPositions = m_FoodballManager.GetFoodballPositions();
    m_PowerballPositions = m_PowerballManager.GetPowerballPositions();

    m_FoodballItemsCount = m_FoodballPositions.size();
    m_PowerballItemsCount = m_PowerballPositions.size();
}

void PacmanServer::SetUpAndPlacePlayers()
{
    m_Pacman.Reset();
    m_Ghost.Reset();

    m_PacmanScore = 0;
    m_Player1Ready = false;
    m_Player2Ready = false;
}

void PacmanServer::StartGame()
{
    m_ConnectionObject_UpdaterTimer = connect(&m_UpdaterTimer, &QTimer::timeout, this, &PacmanServer::Updater, Qt::UniqueConnection);

    m_ConnectionObject_WaitForPlayerReadySignalTimer = connect(&m_SendCoordinatesPlayer1Timer, &QTimer::timeout, this, &PacmanServer::SendcoordinatesToClient1, Qt::UniqueConnection);
    m_ConnectionObject_SendCoordinatesPlayer2Timer = connect(&m_SendCoordinatesPlayer2Timer, &QTimer::timeout, this, &PacmanServer::SendcoordinatesToClient2, Qt::UniqueConnection);

    m_UpdaterTimer.start(4);
    m_SendCoordinatesPlayer1Timer.start(5);
    m_SendCoordinatesPlayer2Timer.start(5);

    qDebug() << "Game started";

    m_GameState = GameState::Running;
}

void PacmanServer::PauseGame()
{
    m_GameState = GameState::Paused;
}

void PacmanServer::ResumeGame()
{
    m_GameState = GameState::Running;
}

void PacmanServer::StopAllTimers()
{
    m_WaitForPlayerConnectionTimer.stop();
    m_WaitForPlayerReadySignalTimer.stop();
    m_UpdaterTimer.stop();
    m_SendCoordinatesPlayer1Timer.stop();
    m_SendCoordinatesPlayer2Timer.stop();
}

void PacmanServer::ResetContainersAndVariables()
{
    m_Pacman.Reset();
    m_Ghost.Reset();

    QByteArray player1Direction = QByteArray::number(0);
    QByteArray player2Direction = QByteArray::number(0);

    QByteArray player1X = QByteArray::number(m_Pacman.GetX());
    QByteArray player1Y_ = QByteArray::number(m_Pacman.GetY());

    QByteArray player2X = QByteArray::number(m_Ghost.GetX());
    QByteArray player2Y = QByteArray::number(m_Ghost.GetY());

    QByteArray player1XCoordinatePacked = "{{D1" + player1Direction + "[x1:"+player1X+",";
    QByteArray player1YCoordinatePacked = "y1:" + player1Y_ + "];";
    QByteArray player2XCoordinatePacked = "D2" + player2Direction + "[x2:"+player2X+",";
    QByteArray player2YCoordinatePacked = "y2:" + player2Y + "]},";

    if(m_GameState == GameState::BeforeFirstRun)
    {
        m_GameStatePacked = "{[S:0],";
    }
    else if(m_GameState == GameState::Running)
    {
        m_GameStatePacked = "{[S:1],";
    }
    else if(m_GameState == GameState::Paused)
    {
        m_GameStatePacked = "{[S:2],";
    }
    else if(m_GameState == GameState::Aborted)
    {
        m_GameStatePacked = "{[S:3],";
    }
    else if(m_GameState == GameState::PacmanWin)
    {
        m_GameStatePacked = "{[S:4],";
        m_UpdaterTimer.stop();
    }
    else if(m_GameState == GameState::GhostWin)
    {
        m_GameStatePacked = "{[S:5],";
        m_UpdaterTimer.stop();
    }

    if(m_Ghost.GetScaredBlue())
    {
        if(!m_Ghost.GetScaredWhite()) //scared blue
        {
            m_IsGhostScaredWhitePacked = "[G:S],";
        }
        else //scared white
        {
            m_IsGhostScaredWhitePacked = "[G:W],";
        }
    }
    else //no scared
    {
        m_IsGhostScaredWhitePacked = "[G:N],";
    }

    QByteArray pointsPacked = "[P:" + QByteArray::number(m_PacmanScore) + "]},";

    m_MessageToWrite = "{del:[100,100]}";

    QByteArray messagePacked = "{'" + m_MessageToWrite + "'}}";

    m_dataPacketForClient = player1XCoordinatePacked + player1YCoordinatePacked + player2XCoordinatePacked + player2YCoordinatePacked +
            m_GameStatePacked + m_IsGhostScaredWhitePacked + pointsPacked + messagePacked;

    m_FoodballPositions.clear();
    m_FoodballPositions.squeeze();
    m_FoodballPositions = m_FoodballManager.GetFoodballPositions();

    m_PowerballPositions.clear();
    m_PowerballPositions.squeeze();
    m_PowerballPositions = m_PowerballManager.GetPowerballPositions();

    m_FoodballItemsCount = m_FoodballPositions.size();
    m_PowerballItemsCount = m_PowerballPositions.size();
}

void PacmanServer::PrepareRestart()
{
    ResetContainersAndVariables();

    m_ConnectionObject_WaitForPlayerReadySignalTimer = connect(&m_WaitForPlayerReadySignalTimer, &QTimer::timeout, this, &PacmanServer::WaitForPlayerReadySignals, Qt::UniqueConnection);
    m_WaitForPlayerReadySignalTimer.start(2000);

    m_Player1Ready = false;
    m_Player2Ready = false;

    qDebug() << "Game restarted on server, players not ready yet";
}

//SLOT

void PacmanServer::AcceptConnection()
{
    //if there is new connection
    if(m_pServerSocket1->state() == QAbstractSocket::UnconnectedState)
    {
        m_pServerSocket1 = m_Server.nextPendingConnection();

        m_ConnectionObject_Socket1Connected = connect(m_pServerSocket1, &QTcpSocket::connected, this, &PacmanServer::connected1, Qt::UniqueConnection);
        m_ConnectionObject_Socket1Disconnected = connect(m_pServerSocket1, &QTcpSocket::disconnected, this, &PacmanServer::disconnected1, Qt::UniqueConnection);
        m_ConnectionObject_Socket1ReadyRead = connect(m_pServerSocket1, &QTcpSocket::readyRead, this, &PacmanServer::ReadDirectionFromClient1, Qt::UniqueConnection);

        //set up timer waiting for players signals
        m_ConnectionObject_WaitForPlayerConnectionTimer = connect(&m_WaitForPlayerConnectionTimer, &QTimer::timeout, this, &PacmanServer::WaitForPlayerConnection, Qt::UniqueConnection);
        m_WaitForPlayerConnectionTimer.start(3000);

        qDebug() << "Connection assigned to socket 1 - PACMAN";
        m_pServerSocket1->write("You are PLAYER 1 - PACMAN");
    }
    else
    {
        m_pServerSocket2 = m_Server.nextPendingConnection();

        m_ConnectionObject_Socket2Connected = connect(m_pServerSocket2, &QTcpSocket::connected, this, &PacmanServer::connected2, Qt::UniqueConnection);
        m_ConnectionObject_Socket2Disconnected = connect(m_pServerSocket2, &QTcpSocket::disconnected, this, &PacmanServer::disconnected2, Qt::UniqueConnection);
        m_ConnectionObject_Socket2ReadyRead = connect(m_pServerSocket2, &QTcpSocket::readyRead, this, &PacmanServer::ReadDirectionFromClient2, Qt::UniqueConnection);

        m_WaitForPlayerConnectionTimer.stop();
        disconnect(&m_WaitForPlayerConnectionTimer, &QTimer::timeout, this, &PacmanServer::WaitForPlayerConnection); //disconnect unneccesary signal

        m_ConnectionObject_WaitForPlayerReadySignalTimer = connect(&m_WaitForPlayerReadySignalTimer, &QTimer::timeout, this, &PacmanServer::WaitForPlayerReadySignals, Qt::UniqueConnection);
        m_WaitForPlayerReadySignalTimer.start(2000);

        qDebug() << "Connection assigned to socket 2 - GHOST";
        m_pServerSocket2->write("You are PLAYER 2 - GHOST");
    }
}

void PacmanServer::WaitForPlayerConnection()
{
    qDebug() << "Waiting for player 2";
    m_pServerSocket1->write("You are connected, waiting for another player");
}

void PacmanServer::WaitForPlayerReadySignals()
{
    if(!m_Player1Ready && !m_Player2Ready)
    {
        qDebug("Waiting for players ready signals");
        m_pServerSocket1->write("You are not ready, please press space");
        m_pServerSocket2->write("You are not ready, please press space");
    }

    if(m_Player1Ready && !m_Player2Ready)
    {
        qDebug("Player 1 - ready Player 2 - NOT READY");
        m_pServerSocket1->write("You are ready, awaiting other player connection");
        m_pServerSocket2->write("You are not ready, please press space");
    }

    if(!m_Player1Ready && m_Player2Ready)
    {
        qDebug("Player 2 - NOT READY Player 1 - ready");
        m_pServerSocket1->write("You are not ready, please press space");
        m_pServerSocket2->write("You are ready, awaiting other player connection");
    }

    if(m_Player1Ready && m_Player2Ready)
    {
        qDebug("Player 1 and Player 2 ready");

        StartGame();
        m_pServerSocket1->write("Game started");
        m_pServerSocket2->write("Game started");

        m_WaitForPlayerReadySignalTimer.stop();
        disconnect(&m_WaitForPlayerReadySignalTimer, &QTimer::timeout, this, &PacmanServer::WaitForPlayerReadySignals);
    }
}

void PacmanServer::SendcoordinatesToClient1()
{
    m_pServerSocket1->write(m_dataPacketForClient);
}

void PacmanServer::SendcoordinatesToClient2()
{
    m_pServerSocket2->write(m_dataPacketForClient);
    PackDataToSendToClients();
}

void PacmanServer::PackDataToSendToClients()
{
    DataPacket dataPacket(m_Pacman.GetDirection(),
                          m_Pacman.GetX(),
                          m_Pacman.GetY(),
                          m_Ghost.GetDirection(),
                          m_Ghost.GetX(),
                          m_Ghost.GetY(),
                          m_GameState,
                          m_Ghost.GetScaredBlue(),
                          m_Ghost.GetScaredWhite(),
                          m_PacmanScore,
                          m_MessageToWrite);

    m_dataPacketForClient = dataPacket.Pack();
}

void PacmanServer::CheckCollision()
{
    if((m_Pacman.GetX() == m_Ghost.GetX()) && (m_Pacman.GetY() == m_Ghost.GetY()))
    {
        if(m_Ghost.GetScaredBlue() == true)
        {
            //PACMAN WINS
            m_GameState = GameState::PacmanWin;
            qDebug() << "Pacman wins";

            //RESET PACMAN AND GHOST POSITIONS
            SetUpAndPlacePlayers();

            qDebug() << "RESET PACMAN AND GHOST POSITIONS";

            PrepareRestart();
        }
        else
        {
            //GHOST WINS
            m_GameState = GameState::GhostWin;
            qDebug() << "Ghost wins";

            //RESET PACMAN AND GHOST POSITIONS
            SetUpAndPlacePlayers();

            qDebug() << "RESET PACMAN AND GHOST POSITIONS";

            PrepareRestart();
        }
    }
}

void PacmanServer::DisconnectAllSignals()
{
    disconnect(m_ConnectionObject_NewConnection);
    disconnect(m_ConnectionObject_WaitForPlayerConnectionTimer);
    disconnect(m_ConnectionObject_WaitForPlayerReadySignalTimer);
    disconnect(m_ConnectionObject_UpdaterTimer);
    disconnect(m_ConnectionObject_WaitForPlayerReadySignalTimer);
    disconnect(m_ConnectionObject_SendCoordinatesPlayer2Timer);
    disconnect(m_ConnectionObject_Socket1Connected);
    disconnect(m_ConnectionObject_Socket1Disconnected);
    disconnect(m_ConnectionObject_Socket1ReadyRead);
    disconnect(m_ConnectionObject_Socket2Connected);
    disconnect(m_ConnectionObject_Socket2Disconnected);
    disconnect(m_ConnectionObject_Socket2ReadyRead);
}

void PacmanServer::Updater()
{
    int pacmanX = m_Pacman.GetX();
    int pacmanY = m_Pacman.GetY();

    m_FoodballItemsCount = m_FoodballPositions.size();
    m_PowerballItemsCount = m_PowerballPositions.size();

    /*Eating foodballs*/
    for(int i = 0; i < m_FoodballItemsCount; i++)
    {
        if(pacmanX == m_FoodballPositions.at(i).x() && pacmanY == m_FoodballPositions.at(i).y())
        {
            m_MessageToWrite = "{del:[" + QByteArray::number(m_FoodballPositions.at(i).x()) + "," + QByteArray::number(m_FoodballPositions.at(i).y()) + "]}";

            --m_FoodballItemsCount;
            m_FoodballPositions.remove(i);
        }
    }

    /*Eating powerballs*/
    for(int i=0; i < m_PowerballItemsCount; i++)
    {
        if(pacmanX == m_PowerballPositions.at(i).x() && pacmanY == m_PowerballPositions.at(i).y())
        {
            m_MessageToWrite = "{del:[" + QByteArray::number(m_PowerballPositions.at(i).x()) + "," + QByteArray::number(m_PowerballPositions.at(i).y()) + "]}";

            --m_PowerballItemsCount;
            m_PowerballPositions.remove(i);

            m_Ghost.SetScaredStateBlue(true);
        }
    }

    if(m_FoodballItemsCount == 0)
    {
        //PACMAN WINS
        m_GameState = GameState::PacmanWin;
        qDebug() << "Pacman wins";

        //RESET PACMAN AND GHOST POSITIONS
        SetUpAndPlacePlayers();

        qDebug() << "RESET PACMAN AND GHOST POSITIONS";

        PrepareRestart();
    }

    if(m_Ghost.GetScaredBlue())
    {
        m_Ghost.IncrementScaredStateTimer();

        if(m_Ghost.GetScaredStateTimer() == Ghost::SCARED_WHITE_THRESHOLD)
        {
            m_Ghost.SetScaredStateWhite(true);
        }

        if(m_Ghost.GetScaredStateTimer() == Ghost::SCARED_TIMEOUT)
        {
            m_Ghost.SetScaredStateBlue(false);
            m_Ghost.SetScaredStateWhite(false);

            m_Ghost.SetScaredStateTimer(0);
        }
    }

    m_Pacman.Move(m_Map);
    m_Ghost.Move(m_Map);

    CheckCollision();
}

//SLOTS
/*Socket 1 wrapper*/
void PacmanServer::ReadDirectionFromClient1()
{
    ReadDirection(m_pServerSocket1, m_Pacman, m_Player1Ready);
}

/*Socket 2 wrapper*/
void PacmanServer::ReadDirectionFromClient2()
{
    ReadDirection(m_pServerSocket2, m_Ghost, m_Player2Ready);
}

void PacmanServer::ReadDirection(QTcpSocket* tcpSocket, MovableCharacter& movableCharacter, bool& playerReady)
{
    QByteArray dataReceivedFromClient = tcpSocket->readAll();
    dataReceivedFromClient.resize(1);

    char keyInputReceivedFromClient = dataReceivedFromClient.at(0);

    if((m_GameState == GameState::BeforeFirstRun || m_GameState == GameState::PacmanWin || m_GameState == GameState::GhostWin) && keyInputReceivedFromClient == SIGNAL_START)
    {
        playerReady = true;
        return;
    }

    if(m_GameState == GameState::Running && keyInputReceivedFromClient == SIGNAL_PAUSE)
    {
        PauseGame();
        return;
    }

    if(m_GameState == GameState::Paused && keyInputReceivedFromClient == SIGNAL_PAUSE)
    {
        ResumeGame();
        return;
    }

    movableCharacter.SetNextDirection(static_cast<Direction>(keyInputReceivedFromClient - '0'));
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
