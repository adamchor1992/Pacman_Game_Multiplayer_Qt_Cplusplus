#include "game_window.h"
#include "ui_game_window.h"
#include "../common/common.h"
#include "../common/log_manager.h"
#include "../common/data_packet.h"

GameWindow::GameWindow(QWidget *parent, QHostAddress address) :
    QMainWindow(parent),
    ui(new Ui::Game_window),
    m_FoodballPositions(m_FoodBallManager.GetFoodballPositions()),
    m_PowerballPositions(m_PowerballManager.GetPowerballPositions())
{
    ui->setupUi(this);

    setWindowTitle("Pacman");

    ui->gameplay_area->setScene(&m_Scene);
    ui->gameplay_area->setRenderHint(QPainter::Antialiasing);
    m_Scene.setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    ui->gameplay_area->setSceneRect(m_Scene.sceneRect());

    StatusBarManager(ui->statusbar);

    SetGameState(GameState::BeforeFirstRun);
    m_WaitingForRestartKey = false;
    m_RestartPending = false;

    m_ServerConnection.ConnectToServer(address, ServerConnection::GAME_PORT);

    AddSceneItems();

    PrepareGameToStart();

    connect(&m_SceneUpdateTimer, &QTimer::timeout, this, &GameWindow::UpdateScene, Qt::UniqueConnection);

    m_SceneUpdateTimer.start(SCENE_UPDATE_TIMEOUT);
}

void GameWindow::AddSceneItems()
{
    m_pMapItem = m_Scene.addPixmap(QPixmap(":/images/pac_map.png"));

    PopulateMap();

    m_Scene.addItem(&m_ScreenTextDisplay);
    m_Scene.addItem(&m_Pacman);
    m_Scene.addItem(&m_Ghostplayer);
}

void GameWindow::PopulateMap()
{
    const int foodballItemsCount = m_FoodballPositions.size();
    const int powerballItemsCount = m_PowerballPositions.size();

    for(int i = 0; i < foodballItemsCount; i++)
    {
        /*Populate table of graphical items in following way (Key : Value) = (QString - "x,y" : pointer to QGraphicsEllipseItem at point ("x,y"))*/
        m_FoodballGraphicalItemsTableMap.insert(QString(QString::number(m_FoodballPositions.at(i).x()) + "," + QString::number(m_FoodballPositions.at(i).y())),
                                                m_Scene.addEllipse(m_FoodballPositions.at(i).x(), m_FoodballPositions.at(i).y(), FoodballManager::WIDTH, FoodballManager::HEIGHT, QPen(Qt::NoPen), QBrush(Qt::white)));
    }

    for(int i = 0; i < powerballItemsCount; i++)
    {
        /*Populate table of graphical items in following way (Key : Value) = (QString - "x,y" : pointer to QGraphicsEllipseItem at point ("x,y"))*/
        m_PowerballGraphicalItemsTableMap.insert(QString(QString::number(m_PowerballPositions.at(i).x()) + "," + QString::number(m_PowerballPositions.at(i).y())),
                                                 m_Scene.addEllipse(m_PowerballPositions.at(i).x() - 5, m_PowerballPositions.at(i).y() - 8, PowerballManager::WIDTH, PowerballManager::HEIGHT, QPen(Qt::NoPen), QBrush(Qt::white)));
    }

    assert(foodballItemsCount == m_FoodballGraphicalItemsTableMap.size());
    assert(powerballItemsCount == m_PowerballGraphicalItemsTableMap.size());

    qDebug("Foodball positions size: %d", foodballItemsCount);
}

void GameWindow::PrepareGameToStart()
{
    LogManager::LogToFile("PrepareGameToStart");

    connect(&m_UpdaterTimer, &QTimer::timeout, this, &GameWindow::Updater, Qt::UniqueConnection);
    connect(&m_ServerConnection, &ServerConnection::NewDataFromServerAvailable, this, &GameWindow::ProcessNewDataFromServer, Qt::UniqueConnection);

    m_UpdaterTimer.start(UPDATER_TIMEOUT);
}

void GameWindow::HideSceneItems()
{
    LogManager::LogToFile("HideSceneItems");

    m_pMapItem->hide();
    m_Pacman.hide();
    m_Ghostplayer.hide();

    for(QMap<QString, QGraphicsEllipseItem*>::iterator iter1 = m_FoodballGraphicalItemsTableMap.begin() ; iter1 != m_FoodballGraphicalItemsTableMap.end() ;iter1++)
    {
        m_FoodballGraphicalItemsTableMap.value(iter1.key())->hide();
    }

    for(QMap<QString, QGraphicsEllipseItem*>::iterator iter2 = m_PowerballGraphicalItemsTableMap.begin() ; iter2 != m_PowerballGraphicalItemsTableMap.end() ;iter2++)
    {
        m_PowerballGraphicalItemsTableMap.value(iter2.key())->hide();
    }
}

void GameWindow::ShowSceneItems()
{
    LogManager::LogToFile("ShowSceneItems");

    m_pMapItem->show();
    m_Pacman.show();
    m_Ghostplayer.show();

    for(QMap<QString, QGraphicsEllipseItem*>::iterator iter1 = m_FoodballGraphicalItemsTableMap.begin() ; iter1 != m_FoodballGraphicalItemsTableMap.end() ;iter1++)
    {
        m_FoodballGraphicalItemsTableMap.value(iter1.key())->show();
    }

    for(QMap<QString, QGraphicsEllipseItem*>::iterator iter2 = m_PowerballGraphicalItemsTableMap.begin() ; iter2 != m_PowerballGraphicalItemsTableMap.end() ;iter2++)
    {
        m_PowerballGraphicalItemsTableMap.value(iter2.key())->show();
    }
}

void GameWindow::RestartGame()
{
    LogManager::LogToFile("RestartGame");

    m_Pacman.SetDirection(Direction::NO_DIRECTION);
    m_Pacman.SetX(PACMAN_START_X);
    m_Pacman.SetY(PACMAN_START_Y);

    m_ScreenTextDisplay.hide();

    qDebug() << "Restarting game";

    PrepareGameToStart();

    ShowSceneItems();

    m_Sounds.m_BeginningSound.play();

    StatusBarManager::ShowMessage("Game started", StatusBarManager::MESSAGE_TIMEOUT);

    m_UpdaterTimer.start(UPDATER_TIMEOUT);

    this->setFocus();
}

void GameWindow::StartGame()
{
    LogManager::LogToFile("StartGame");

    m_Sounds.m_BeginningSound.play();

    StatusBarManager::ShowMessage("Game started", StatusBarManager::MESSAGE_TIMEOUT);

    m_UpdaterTimer.start(UPDATER_TIMEOUT);
    this->setFocus();
}

void GameWindow::CheckForRestartGameSignal()
{
    LogManager::LogToFile("CheckForRestartGameSignal");

    qDebug() << "Waiting for restart signal from server";
    LogManager::LogToFile("Waiting for restart signal from server");

    if(m_GameState == GameState::Running && m_RestartPending)
    {
        m_RestartPending = false;
        m_WaitingForRestartKey = false;

        RestartGame();
        m_WaitForRestartKeyTimer.stop();

        disconnect(&m_WaitForRestartKeyTimer, &QTimer::timeout, this, &GameWindow::CheckForRestartGameSignal);
        qDebug() << "GAME RESTARTED on client side";
    }
}

void GameWindow::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
    case Qt::Key_A:
    case Qt::Key_Left:
        m_ServerConnection.SendPressedKeyToServer(MOVEMENT_LEFT);
        break;

    case Qt::Key_D:
    case Qt::Key_Right:
        m_ServerConnection.SendPressedKeyToServer(MOVEMENT_RIGHT);
        break;

    case Qt::Key_S:
    case Qt::Key_Down:
        m_ServerConnection.SendPressedKeyToServer(MOVEMENT_DOWN);
        break;

    case Qt::Key_W:
    case Qt::Key_Up:
        m_ServerConnection.SendPressedKeyToServer(MOVEMENT_UP);
        break;

    case Qt::Key_Space:
        if(m_GameState == GameState::BeforeFirstRun || m_GameState == GameState::PacmanWin || m_GameState == GameState::GhostWin)
        {
            m_ServerConnection.SendPressedKeyToServer(SIGNAL_START);
        }
        break;

    default:
        break;
    }
}

void GameWindow::RemoveBall(QString& coordinatesOfObjectToBeRemoved)
{
    QMap<QString, QGraphicsEllipseItem*>::iterator iter1 = m_FoodballGraphicalItemsTableMap.find(coordinatesOfObjectToBeRemoved);
    QMap<QString, QGraphicsEllipseItem*>::iterator iter2 = m_PowerballGraphicalItemsTableMap.find(coordinatesOfObjectToBeRemoved);

    if(iter1 != m_FoodballGraphicalItemsTableMap.end())
    {
        if(m_FoodballGraphicalItemsTableMap.value(iter1.key())->isVisible())
        {
            m_FoodballGraphicalItemsTableMap.value(iter1.key())->hide();
            m_Sounds.m_EatSound.play();
        }
    }
    else if(iter2 != m_PowerballGraphicalItemsTableMap.end())
    {
        if(m_PowerballGraphicalItemsTableMap.value(iter2.key())->isVisible())
        {
            m_PowerballGraphicalItemsTableMap.value(iter2.key())->hide();
            m_Sounds.m_EatSound.play();
        }
    }
}

void GameWindow::ProcessGameDataPacket(QJsonObject& jsonObject)
{
    LogManager::LogToFile("ProcessGameDataPacket");

    int pacmanX = jsonObject.value("PX").toInt(-1);
    int pacmanY = jsonObject.value("PY").toInt(-1);
    Direction pacmanDirection = static_cast<Direction>(jsonObject.value("PD").toInt(-1));
    int ghostX = jsonObject.value("GX").toInt(-1);
    int ghostY = jsonObject.value("GY").toInt(-1);
    Direction ghostDirection = static_cast<Direction>(jsonObject.value("GD").toInt(-1));
    GameState gameState = static_cast<GameState>(jsonObject.value("GameState").toInt(-1));
    GhostScaredState ghostScaredState = static_cast<GhostScaredState>(jsonObject.value("GhostScaredState").toInt(-1));

    m_Pacman.SetX(pacmanX);
    m_Pacman.SetY(pacmanY);
    m_Pacman.SetDirection(pacmanDirection);
    m_Ghostplayer.SetX(ghostX);
    m_Ghostplayer.SetY(ghostY);
    m_Ghostplayer.SetDirection(ghostDirection);
    SetGameState(gameState);
    m_Ghostplayer.SetScaredState(ghostScaredState);

    QString coordinatesOfObjectToBeRemoved = jsonObject.value("Object to remove").toString();

    RemoveBall(coordinatesOfObjectToBeRemoved);
}

void GameWindow::ProcessCommandPacket(QJsonObject& jsonObject)
{
    LogManager::LogToFile("ProcessCommandPacket");

    if(jsonObject.value("Payload") == "Game started")
    {
        StatusBarManager::ClearMessage();
        StatusBarManager::ShowMessage("Game started", StatusBarManager::MESSAGE_TIMEOUT);
    }
}

void GameWindow::ProcessMessagePacket(QJsonObject& jsonObject)
{
    LogManager::LogToFile("ProcessMessagePacket");
    StatusBarManager::ShowMessage(jsonObject.value("Payload").toString());
}

void GameWindow::ProcessNewDataFromServer()
{
    LogManager::LogToFile("ProcessNewDataFromServer");

    QByteArray dataReceivedFromServer = m_ServerConnection.ReadDataFromServer();

    LogManager::LogToFile("Data received from server<START>: " + std::to_string(dataReceivedFromServer.size()) + " bytes>>>>>");
    LogManager::LogToFile(dataReceivedFromServer.toStdString());
    LogManager::LogToFile("Data received from server<END>");

    QJsonDocument jsonDocument = QJsonDocument::fromJson(dataReceivedFromServer);
    QJsonObject jsonObject = jsonDocument.object();

    LogManager::LogToFile("JSON Type: " + std::to_string(jsonObject.value("Type").toInt(-1)));

    if(jsonObject.value("Type").toInt(-1) == static_cast<int>(DataPacket::GAME_DATA))
    {
        ProcessGameDataPacket(jsonObject);
    }
    else if(jsonObject.value("Type").toInt(-1) == static_cast<int>(DataPacket::COMMAND))
    {
        ProcessCommandPacket(jsonObject);
    }
    else if(jsonObject.value("Type").toInt(-1) == static_cast<int>(DataPacket::MESSAGE))
    {
        ProcessMessagePacket(jsonObject);
    }
    else
    {
        LogManager::LogToFile("-----UNKNOWN PACKET START-----");
        LogManager::LogToFile(dataReceivedFromServer.toStdString());
        LogManager::LogToFile("-----UNKNOWN PACKET END-----");
    }
}

void GameWindow::UpdateScene()
{
    m_Scene.update(m_Scene.sceneRect());
}

void GameWindow::Updater()
{
    if((m_GameState == GameState::PacmanWin) && (!m_WaitingForRestartKey))
    {
        LogManager::LogToFile("PacmanWin && !m_WaitingForRestartKey");

        m_RestartPending = true;
        HideSceneItems();

        m_ScreenTextDisplay.SetTextDisplayState(ScreenTextDisplay::TextDisplayState::END_PACMAN);

        qDebug() << "Pacman wins";

        StatusBarManager::ShowMessage("PACMAN WINS");

        m_UpdaterTimer.stop();

        connect(&m_WaitForRestartKeyTimer, &QTimer::timeout, this, &GameWindow::CheckForRestartGameSignal, Qt::UniqueConnection);
        m_WaitForRestartKeyTimer.start(500);

        m_WaitingForRestartKey = true;
    }

    if((m_GameState == GameState::GhostWin) && (!m_WaitingForRestartKey))
    {
        LogManager::LogToFile("GhostWin && !m_WaitingForRestartKey");

        m_RestartPending = true;
        HideSceneItems();

        m_ScreenTextDisplay.SetTextDisplayState(ScreenTextDisplay::TextDisplayState::END_GHOST);

        qDebug() << "Ghost wins";

        StatusBarManager::ShowMessage("GHOST WINS");

        m_UpdaterTimer.stop();

        connect(&m_WaitForRestartKeyTimer, &QTimer::timeout, this, &GameWindow::CheckForRestartGameSignal, Qt::UniqueConnection);
        m_WaitForRestartKeyTimer.start(500);

        m_WaitingForRestartKey = true;
    }

    m_Pacman.AdvanceAnimation();
    m_Ghostplayer.AdvanceAnimation();
}

void GameWindow::SetGameState(GameState gameState)
{
    LogManager::LogToFile("Setting game state to: " + std::to_string(static_cast<int>(gameState)));
    m_GameState = gameState;
}

GameWindow::~GameWindow()
{
    delete ui;
}
