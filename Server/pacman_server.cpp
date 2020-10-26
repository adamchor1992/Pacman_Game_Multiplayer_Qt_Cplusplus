#include "pacman_server.h"

PacmanServer::PacmanServer(QObject *parent) : QObject(parent)
{
    //setting up temporary socket

    m_ServerSocket1 = &m_TempSocket;

    ServerStartListening();

    //wait for new connections
    m_ConnectionObject_NewConnection = connect(&m_Server, SIGNAL(newConnection()), this, SLOT(AcceptConnection()), Qt::UniqueConnection);

    //create game environment
    SetUpAndFillMap();
    SetUpAndPlacePlayers();

    m_GameState = GameState::BeforeFirstRun;
}

void PacmanServer::ServerStartListening()
{
    if(m_Server.listen(QHostAddress::Any, 5000))
    {
        qDebug() << "Server started, awaiting connections";
    }
    else
    {
        qDebug() << "Server could not start: " << m_Server.errorString();
        qDebug() << "Please restart application";
    }
}

void PacmanServer::SetUpAndFillMap()
{
    m_FoodballPositions = m_Foodball.GetFoodballPositions();
    m_PowerballPositions = m_Powerball.GetPowerballPositions();

    m_FoodballItemsCount = m_FoodballPositions.size();
    m_PowerballItemsCount = m_PowerballPositions.size();
}

void PacmanServer::SetUpAndPlacePlayers()
{
    m_Pacman.Reset();
    m_GhostPlayer.Reset();

    m_Player1Score = 0;
    m_Player1Ready = false;
    m_Player2Ready = false;
}

void PacmanServer::StartGame()
{
    m_ConnectionObject_UpdaterTimer = connect(&m_UpdaterTimer, SIGNAL(timeout()), this, SLOT(Updater()), Qt::UniqueConnection);

    m_ConnectionObject_PackDataToSendToClients = connect(this, SIGNAL(PrepareNextCoordinatesPackage()), this, SLOT(PackDataToSendToClients()), Qt::UniqueConnection);
    m_ConnectionObject_WaitForPlayerReadySignalTimer = connect(&m_SendCoordinatesPlayer1Timer, SIGNAL(timeout()), this, SLOT(SendcoordinatesToClient1()), Qt::UniqueConnection);
    m_ConnectionObject_SendCoordinatesPlayer2Timer = connect(&m_SendCoordinatesPlayer2Timer, SIGNAL(timeout()), this, SLOT(SendcoordinatesToClient2()), Qt::UniqueConnection);

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
    m_GhostPlayer.Reset();

    QByteArray player1Direction = QByteArray::number(0);
    QByteArray player2Direction = QByteArray::number(0);

    QByteArray player1X = QByteArray::number(m_Pacman.GetX());
    QByteArray player1Y_ = QByteArray::number(m_Pacman.GetY());

    QByteArray player2X = QByteArray::number(m_GhostPlayer.GetX());
    QByteArray player2Y = QByteArray::number(m_GhostPlayer.GetY());

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

    if(m_GhostPlayer.GetScaredBlue())
    {
        if(!m_GhostPlayer.GetScaredWhite()) //scared blue
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

    QByteArray pointsPacked = "[P:" + QByteArray::number(m_Player1Score) + "]},";

    m_MessageToWrite = "{del:[100,100]}";

    QByteArray messagePacked = "{'" + m_MessageToWrite + "'}}";

    m_InfoPackageForClientsPacked = player1XCoordinatePacked + player1YCoordinatePacked + player2XCoordinatePacked + player2YCoordinatePacked +
            m_GameStatePacked + m_IsGhostScaredWhitePacked + pointsPacked + messagePacked;

    m_FoodballPositions.clear();
    m_FoodballPositions.squeeze();
    m_FoodballPositions = m_Foodball.GetFoodballPositions();

    m_PowerballPositions.clear();
    m_PowerballPositions.squeeze();
    m_PowerballPositions = m_Powerball.GetPowerballPositions();

    m_FoodballItemsCount = m_FoodballPositions.size();
    m_PowerballItemsCount = m_PowerballPositions.size();
}

void PacmanServer::PrepareRestart()
{
    ResetContainersAndVariables();

    m_ConnectionObject_WaitForPlayerReadySignalTimer = connect(&m_WaitForPlayerReadySignalTimer, SIGNAL(timeout()), this, SLOT(WaitForPlayerReadySignals()), Qt::UniqueConnection);
    m_WaitForPlayerReadySignalTimer.start(2000);

    m_Player1Ready = false;
    m_Player2Ready = false;

    qDebug() << "Game restarted on server, players not ready yet";
}

//SLOT

void PacmanServer::AcceptConnection()
{
    //if there is new connection
    if(m_ServerSocket1->state() == QAbstractSocket::UnconnectedState)
    {
        m_ServerSocket1 = m_Server.nextPendingConnection();

        m_ConnectionObject_Socket1Connected = connect(m_ServerSocket1, SIGNAL(connected()), this, SLOT(connected1()), Qt::UniqueConnection);
        m_ConnectionObject_Socket1Disconnected = connect(m_ServerSocket1, SIGNAL(disconnected()), this, SLOT(disconnected1()), Qt::UniqueConnection);
        m_ConnectionObject_Socket1ReadyRead = connect(m_ServerSocket1, SIGNAL(readyRead()), this, SLOT(ReadDirectionFromClient1()), Qt::UniqueConnection);

        //set up timer waiting for players signals
        m_ConnectionObject_WaitForPlayerConnectionTimer = connect(&m_WaitForPlayerConnectionTimer, SIGNAL(timeout()), this, SLOT(WaitForPlayerConnection()), Qt::UniqueConnection);
        m_WaitForPlayerConnectionTimer.start(3000);

        qDebug() << "Connection assigned to socket 1 - PACMAN";
        m_ServerSocket1->write("You are PLAYER 1 - PACMAN");
    }
    else
    {
        m_ServerSocket2 = m_Server.nextPendingConnection();

        m_ConnectionObject_Socket2Connected = connect(m_ServerSocket2, SIGNAL(connected()), this, SLOT(connected2()), Qt::UniqueConnection);
        m_ConnectionObject_Socket2Disconnected = connect(m_ServerSocket2, SIGNAL(disconnected()), this, SLOT(disconnected2()), Qt::UniqueConnection);
        m_ConnectionObject_Socket2ReadyRead = connect(m_ServerSocket2, SIGNAL(readyRead()), this, SLOT(ReadDirectionFromClient2()), Qt::UniqueConnection);

        m_WaitForPlayerConnectionTimer.stop();
        disconnect(&m_WaitForPlayerConnectionTimer, SIGNAL(timeout()), this, SLOT(WaitForPlayerConnection())); //disconnect unneccesary signal

        m_ConnectionObject_WaitForPlayerReadySignalTimer = connect(&m_WaitForPlayerReadySignalTimer, SIGNAL(timeout()), this, SLOT(WaitForPlayerReadySignals()), Qt::UniqueConnection);
        m_WaitForPlayerReadySignalTimer.start(2000);

        qDebug() << "Connection assigned to socket 2 - GHOST";
        m_ServerSocket2->write("You are PLAYER 2 - GHOST");
    }
}

void PacmanServer::WaitForPlayerConnection()
{
    qDebug() << "Waiting for player 2";
    m_ServerSocket1->write("You are connected, waiting for another player");
}

void PacmanServer::WaitForPlayerReadySignals()
{
    if(!m_Player1Ready && !m_Player2Ready)
    {
        qDebug("Waiting for players ready signals");
        m_ServerSocket1->write("You are not ready, please press space");
        m_ServerSocket2->write("You are not ready, please press space");
    }

    if(m_Player1Ready && !m_Player2Ready)
    {
        qDebug("Player 1 - ready Player 2 - NOT READY");
        m_ServerSocket1->write("You are ready, awaiting other player connection");
        m_ServerSocket2->write("You are not ready, please press space");
    }

    if(!m_Player1Ready && m_Player2Ready)
    {
        qDebug("Player 2 - NOT READY Player 1 - ready");
        m_ServerSocket1->write("You are not ready, please press space");
        m_ServerSocket2->write("You are ready, awaiting other player connection");
    }

    if(m_Player1Ready && m_Player2Ready)
    {
        qDebug("Player 1 and Player 2 ready");

        StartGame();
        m_ServerSocket1->write("Game started");
        m_ServerSocket2->write("Game started");

        m_WaitForPlayerReadySignalTimer.stop();
        disconnect(&m_WaitForPlayerReadySignalTimer, SIGNAL(timeout()), this, SLOT(WaitForPlayerReadySignals()));
    }
}

void PacmanServer::SendcoordinatesToClient1()
{
    m_ServerSocket1->write(m_InfoPackageForClientsPacked);
}

void PacmanServer::SendcoordinatesToClient2()
{
    m_ServerSocket2->write(m_InfoPackageForClientsPacked);
    emit(PrepareNextCoordinatesPackage());
}

void PacmanServer::PackDataToSendToClients()
{
    QByteArray player1Direction = QByteArray::number(static_cast<int>(m_Pacman.GetDirection()));
    QByteArray player2Direction = QByteArray::number(static_cast<int>(m_GhostPlayer.GetDirection()));

    QByteArray player1X = QByteArray::number(m_Pacman.GetX());
    QByteArray player1Y = QByteArray::number(m_Pacman.GetY());

    QByteArray player2X = QByteArray::number(m_GhostPlayer.GetX());
    QByteArray player2Y = QByteArray::number(m_GhostPlayer.GetY());

    QByteArray player1XCoordinatePacked = "{{D1" + player1Direction + "[x1:"+player1X+",";
    QByteArray player1YCoordinatePacked = "y1:" + player1Y + "];";
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

    if(m_GhostPlayer.GetScaredBlue())
    {
        if(!m_GhostPlayer.GetScaredWhite())
        {
            m_IsGhostScaredWhitePacked = "[G:S],";
        }
        else
        {
            m_IsGhostScaredWhitePacked = "[G:W],";
        }
    }

    else //no scared
    {
        m_IsGhostScaredWhitePacked = "[G:N],";
    }

    QByteArray pointsPacked = "[P:" + QByteArray::number(m_Player1Score) + "]},";

    QByteArray messagePacked = "{'" + m_MessageToWrite + "'}}";

    m_InfoPackageForClientsPacked = player1XCoordinatePacked + player1YCoordinatePacked + player2XCoordinatePacked + player2YCoordinatePacked +
            m_GameStatePacked + m_IsGhostScaredWhitePacked + pointsPacked + messagePacked;
}

void PacmanServer::Player1Move()
{
    QPoint p;

    int player1X = m_Pacman.GetX();
    int player1Y = m_Pacman.GetY();
    Direction player1Direction = m_Pacman.GetDirection();
    Direction player1NextDirection = m_Pacman.GetNextDirection();

    if(player1NextDirection != player1Direction)
    {
        switch(player1NextDirection)
        {
        case Direction::LEFT: //left
            p.setX(player1X - 1);
            p.setY(player1Y);

            if(m_Map.IsPointAvailable(p))
            {
                player1Direction = player1NextDirection;
            }
            break;

        case Direction::UP:
            p.setX(player1X);
            p.setY(player1Y - 1);
            if(m_Map.IsPointAvailable(p))
            {
                player1Direction = player1NextDirection;
            }
            break;

        case Direction::DOWN:
            p.setX(player1X);
            p.setY(player1Y + 1);
            if(m_Map.IsPointAvailable(p))
            {
                player1Direction = player1NextDirection;
            }
            break;

        case Direction::RIGHT:
            p.setX(player1X + 1);
            p.setY(player1Y);
            if(m_Map.IsPointAvailable(p))
            {
                player1Direction = player1NextDirection;
            }
            break;

        default:
            break;
        }
    }

    switch(player1Direction)
    {
    case Direction::LEFT:
        p.setX(player1X - 1);
        p.setY(player1Y);
        m_Pacman.SetDirection(player1Direction);

        if(m_Map.IsPointAvailable(p))
        {
            player1X = player1X - 1;
        }

        break;

    case Direction::UP:
        p.setX(player1X);
        p.setY(player1Y - 1);
        m_Pacman.SetDirection(player1Direction);

        if(m_Map.IsPointAvailable(p))
        {
            player1Y= player1Y - 1;
        }

        break;

    case Direction::DOWN:
        p.setX(player1X);
        p.setY(player1Y + 1);
        m_Pacman.SetDirection(player1Direction);

        if(m_Map.IsPointAvailable(p))
        {
            player1Y= player1Y + 1;
        }

        break;

    case Direction::RIGHT:
        p.setX(player1X + 1);
        p.setY(player1Y);
        m_Pacman.SetDirection(player1Direction);

        if(m_Map.IsPointAvailable(p))
        {
            player1X = player1X + 1;
        }

        break;

    default:
        break;
    }

    if(player1X == 0 && player1Y == 318) //teleportation when reached left boundary of middle horizontal line
    {
        player1X = 613;
    }

    if(player1X == 614 && player1Y == 318) //teleportation when reached right boundary of middle horizontal line
    {
        player1X = 1;
    }

    m_Pacman.SetX(player1X);
    m_Pacman.SetY(player1Y);
}

void PacmanServer::Player2Move()
{
    QPoint p;

    int player2X = m_GhostPlayer.GetX();
    int player2Y = m_GhostPlayer.GetY();
    Direction player2Direction = m_GhostPlayer.GetDirection();
    Direction player2NextDirection = m_GhostPlayer.GetNextDirection();

    if(player2NextDirection!=player2Direction)
    {
        switch(player2NextDirection)
        {
        case Direction::LEFT:
            p.setX(player2X - 1);
            p.setY(player2Y);

            if(m_Map.IsPointAvailable(p))
            {
                player2Direction = player2NextDirection;
            }

            break;

        case Direction::UP:
            p.setX(player2X);
            p.setY(player2Y - 1);
            if(m_Map.IsPointAvailable(p))
            {
                player2Direction = player2NextDirection;
            }

            break;

        case Direction::DOWN:
            p.setX(player2X);
            p.setY(player2Y + 1);
            if(m_Map.IsPointAvailable(p))
            {
                player2Direction = player2NextDirection;
            }

            break;

        case Direction::RIGHT:
            p.setX(player2X + 1);
            p.setY(player2Y);
            if(m_Map.IsPointAvailable(p))
            {
                player2Direction = player2NextDirection;
            }

            break;

        default:
            break;
        }
    }

    switch(player2Direction)
    {
    case Direction::LEFT:
        p.setX(player2X - 1);
        p.setY(player2Y);
        m_GhostPlayer.SetDirection(player2Direction);

        if(m_Map.IsPointAvailable(p))
        {
            player2X = player2X - 1;
        }

        break;

    case Direction::UP:
        p.setX(player2X);
        p.setY(player2Y - 1);
        m_GhostPlayer.SetDirection(player2Direction);

        if(m_Map.IsPointAvailable(p))
        {
            player2Y= player2Y - 1;
        }

        break;

    case Direction::DOWN:
        p.setX(player2X);
        p.setY(player2Y + 1);
        m_GhostPlayer.SetDirection(player2Direction);

        if(m_Map.IsPointAvailable(p))
        {
            player2Y = player2Y + 1;
        }

        break;

    case Direction::RIGHT:
        p.setX(player2X + 1);
        p.setY(player2Y);
        m_GhostPlayer.SetDirection(player2Direction);

        if(m_Map.IsPointAvailable(p))
        {
            player2X = player2X + 1;
        }

        break;

    default:
        break;
    }

    if(player2X == 0 && player2Y == 318) //teleportation when reached left boundary of middle horizontal line
    {
        player2X = 613;
    }

    if(player2X == 614 && player2Y == 318) //teleportation when reached right boundary of middle horizontal line
    {
        player2X = 1;
    }

    m_GhostPlayer.SetX(player2X);
    m_GhostPlayer.SetY(player2Y);
}

void PacmanServer::CheckCollision()
{
    if((m_Pacman.GetX() == m_GhostPlayer.GetX()) && (m_Pacman.GetY() == m_GhostPlayer.GetY()))
    {
        if(m_GhostPlayer.GetScaredBlue() == true)
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
    int player1X = m_Pacman.GetX();
    int player1Y = m_Pacman.GetY();

    m_FoodballItemsCount = m_FoodballPositions.size();
    m_PowerballItemsCount = m_PowerballPositions.size();

    //eating foodballs
    for(int i = 0; i < m_FoodballItemsCount; i++)
    {
        if(player1X == m_FoodballPositions.at(i).x() && player1Y == m_FoodballPositions.at(i).y())
        {
            m_MessageToWrite = "{del:[" + QByteArray::number(m_FoodballPositions.at(i).x()) + "," + QByteArray::number(m_FoodballPositions.at(i).y()) + "]}";

            --m_FoodballItemsCount;
            m_FoodballPositions.remove(i);
        }
    }

    //eating powerballs
    for(int i=0; i < m_PowerballItemsCount; i++)
    {
        if(player1X == m_PowerballPositions.at(i).x() && player1Y == m_PowerballPositions.at(i).y())
        {
            m_MessageToWrite = "{del:[" + QByteArray::number(m_PowerballPositions.at(i).x()) + "," + QByteArray::number(m_PowerballPositions.at(i).y()) + "]}";

            --m_PowerballItemsCount;
            m_PowerballPositions.remove(i);

            m_GhostPlayer.SetScaredStateBlue(true);
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

    if(m_GhostPlayer.GetScaredBlue())
    {
        m_GhostPlayer.IncrementScaredStateTimer();

        if(m_GhostPlayer.GetScaredStateTimer() == 750)
        {
            m_GhostPlayer.SetScaredStateWhite(true);
        }

        if(m_GhostPlayer.GetScaredStateTimer() == 1000)
        {
            m_GhostPlayer.SetScaredStateBlue(false);
            m_GhostPlayer.SetScaredStateWhite(false);

            m_GhostPlayer.SetScaredStateTimer(0);
        }
    }

    Player1Move();
    Player2Move();

    CheckCollision();
}

//SLOTS
//socket1
void PacmanServer::ReadDirectionFromClient1()
{
    QByteArray dataReceivedFromClient1 = m_ServerSocket1->readAll();
    dataReceivedFromClient1.resize(1);

    int keyInputReceivedFromClient1 = dataReceivedFromClient1.toInt();

    if((m_GameState == GameState::BeforeFirstRun || m_GameState == GameState::PacmanWin || m_GameState == GameState::GhostWin) && keyInputReceivedFromClient1 == 5) // 5 is ready signal
    {
        m_Player1Ready = true;
        return;
    }

    if((m_GameState == GameState::PacmanWin || m_GameState == GameState::GhostWin) && keyInputReceivedFromClient1 == 7) // 5 is ready signal
    {
        m_Player1Ready = true;
        return;
    }

    if(m_GameState == GameState::Running && keyInputReceivedFromClient1 == 6) // 6 is pause signal
    {
        PauseGame();
        return;
    }

    if(m_GameState == GameState::Paused && keyInputReceivedFromClient1 == 6) // 6 is pause signal
    {
        ResumeGame();
        return;
    }

    m_Pacman.SetNextDirection(static_cast<Direction>(keyInputReceivedFromClient1));
}

//socket2
void PacmanServer::ReadDirectionFromClient2()
{
    QByteArray dataReceivedFromClient2 = m_ServerSocket2->readAll();
    dataReceivedFromClient2.resize(1);

    int keyInputReceivedFromClient2 = dataReceivedFromClient2.toInt();

    if((m_GameState == GameState::BeforeFirstRun || m_GameState == GameState::PacmanWin || m_GameState == GameState::GhostWin) && keyInputReceivedFromClient2 == 5) // 5 is ready signal
    {
        m_Player2Ready = true;
        return;
    }

    if((m_GameState == GameState::PacmanWin || m_GameState == GameState::GhostWin) && keyInputReceivedFromClient2 == 7) // 5 is ready signal
    {
        m_Player2Ready = true;
        return;
    }

    if(m_GameState == GameState::Running && keyInputReceivedFromClient2 == 6) // 6 is pause signal
    {
        PauseGame();
        return;
    }

    if(m_GameState == GameState::Paused && keyInputReceivedFromClient2 == 6) // 6 is pause signal
    {
        ResumeGame();
        return;
    }

    m_GhostPlayer.SetNextDirection(static_cast<Direction>(keyInputReceivedFromClient2));
}

void PacmanServer::connected1()
{
    qDebug() << "Client 1 Connected!";
}

void PacmanServer::disconnected1()
{
    qDebug() << "Client 1 Disconnected!";
    exit(1);
}

void PacmanServer::connected2()
{
    qDebug() << "Client 2 Connected!";
}

void PacmanServer::disconnected2()
{
    qDebug() << "Client 2 Disconnected!";
    exit(2);
}
