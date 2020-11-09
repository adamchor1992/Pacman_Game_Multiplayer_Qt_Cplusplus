#include "game_window.h"
#include "ui_game_window.h"
#include "map.h"
#include "../common/common.h"
#include "../common/powerball_manager.h"
#include "../common/log_manager.h"

GameWindow::GameWindow(QWidget *parent, QHostAddress address) : QMainWindow(parent), ui(new Ui::Game_window)
{
    ui->setupUi(this);

    ui->gameplay_area->setScene(&m_Scene);
    ui->gameplay_area->setRenderHint(QPainter::Antialiasing);
    m_Scene.setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    ui->gameplay_area->setSceneRect(m_Scene.sceneRect());

    StatusBarManager(ui->statusbar);

    m_GameState = GameState::BeforeFirstRun;
    m_WaitingForRestartKey = false;
    m_RestartPending = false;

    m_ServerConnection.ConnectToServer(address, ServerConnection::GAME_PORT);

    GenerateMap();
    PopulateMap();

    GenerateAndPlacePacman();
    GenerateAndPlaceGhosts();

    PrepareGameToStart();

    connect(&m_SceneUpdateTimer, SIGNAL(timeout()), this, SLOT(UpdateScene()), Qt::UniqueConnection);
    connect(&m_ServerConnection, SIGNAL(GameStarted()), this, SLOT(StartGame()), Qt::UniqueConnection);

    m_SceneUpdateTimer.start(10);
}

void GameWindow::GenerateMap()
{
    m_pMapItem = m_Scene.addPixmap(m_Pacmap.GetMapBackgroundPicture());
}

void GameWindow::PopulateMap()
{
    m_PowerballPositions = m_PowerballManager.GetPowerballPositions();
    m_FoodballPositions = m_FoodBallManager.GetFoodballPositions();

    m_FoodballItemsCount = m_FoodballPositions.size();
    m_PowerballItemsCount = m_PowerballPositions.size();

    for(int i=0; i < m_FoodballPositions.size(); i++)
    {
        //populate table of graphical items in following way (Key ; Value) = (QString - "x,y" ; pointer to QGraphicsEllipseItem at point ("x,y"))
        m_FoodballGraphicalItemsTableDict.insert(QString(QString::number(m_FoodballPositions.at(i).x()) + "," + QString::number(m_FoodballPositions.at(i).y())), m_Scene.addEllipse(m_FoodballPositions.at(i).x(),m_FoodballPositions.at(i).y(),7,7,QPen(Qt::NoPen),QBrush(Qt::white)));
    }

    for(int i=0; i < m_PowerballPositions.size(); i++)
    {
        //populate table of graphical items in following way (Key ; Value) = (QString - "x,y" ; pointer to QGraphicsEllipseItem at point ("x,y"))
        m_PowerballGraphicalItemsTableDict.insert(QString(QString::number(m_PowerballPositions.at(i).x()) + "," + QString::number(m_PowerballPositions.at(i).y())), m_Scene.addEllipse(m_PowerballPositions.at(i).x()-5,m_PowerballPositions.at(i).y()-8,15,15,QPen(Qt::NoPen),QBrush(Qt::white)));
    }

    assert(m_PowerballPositions.size() == m_PowerballGraphicalItemsTableDict.size());
    assert(m_FoodballPositions.size() == m_FoodballGraphicalItemsTableDict.size());

    qDebug("Foodball positions size: %d", m_FoodballPositions.size());
}

void GameWindow::GenerateAndPlacePacman()
{
    m_Pacman.SetDirection(Direction::NO_DIRECTION);

    m_Pacman.SetX(PACMAN_START_X);
    m_Pacman.SetY(PACMAN_START_Y);

    m_Scene.addItem(&m_Pacman);
}

void GameWindow::GenerateAndPlaceGhosts()
{
    m_Ghostplayer.SetDirection(Direction::NO_DIRECTION);

    m_Ghostplayer.SetX(GHOST_START_X);
    m_Ghostplayer.SetY(GHOST_START_Y);

    m_Scene.addItem(&m_Ghostplayer);
}

void GameWindow::PrepareGameToStart()
{
    connect(&m_UpdaterTimer, SIGNAL(timeout()), this,SLOT(Updater()), Qt::UniqueConnection);
    connect(&m_UpdateCoordinatesTimer, SIGNAL(timeout()), this,SLOT(UpdateCoordinatesFromServer()), Qt::UniqueConnection);
}

void GameWindow::ResetVariablesAndContainers()
{
    m_FoodballPositions.clear();
    m_FoodballPositions.squeeze();
    m_FoodballGraphicalItemsTable.clear();
    m_FoodballGraphicalItemsTable.squeeze();
    m_FoodballGraphicalItemsTableDict.clear();

    m_PowerballPositions.clear();
    m_PowerballPositions.squeeze();
    m_PowerballGraphicalItemsTable.clear();
    m_PowerballGraphicalItemsTable.squeeze();
    m_PowerballGraphicalItemsTableDict.clear();
}

void GameWindow::HideSceneItems()
{
    m_pMapItem->hide();
    m_Pacman.hide();
    m_Ghostplayer.hide();

    m_Scene.removeItem(&m_Pacman);
    m_Scene.removeItem(&m_Ghostplayer);

    for(QMap<QString, QGraphicsEllipseItem*>::iterator iter1 = m_FoodballGraphicalItemsTableDict.begin() ; iter1 != m_FoodballGraphicalItemsTableDict.end() ;iter1++)
    {
        m_FoodballGraphicalItemsTableDict.value(iter1.key())->hide();
    }

    for(QMap<QString, QGraphicsEllipseItem*>::iterator iter2 = m_PowerballGraphicalItemsTableDict.begin() ; iter2 != m_PowerballGraphicalItemsTableDict.end() ;iter2++)
    {
        m_PowerballGraphicalItemsTableDict.value(iter2.key())->hide();
    }
}

void GameWindow::RestartGame()
{
    m_Pacman.SetDirection(Direction::NO_DIRECTION);
    m_Pacman.SetX(PACMAN_START_X);
    m_Pacman.SetY(PACMAN_START_Y);

    m_TextScreenMessage.hide();
    m_Scene.removeItem(&m_TextScreenMessage);

    qDebug() << "Restarting game";

    ResetVariablesAndContainers();
    PopulateMap();
    PrepareGameToStart();

    m_pMapItem->show();
    m_Scene.addItem(&m_Pacman);
    m_Pacman.show();
    m_Scene.addItem(&m_Ghostplayer);
    m_Ghostplayer.show();

    m_Sounds.m_BeginningSound.play();

    StatusBarManager::ShowMessage("Game started", StatusBarManager::MESSAGE_TIMEOUT);

    m_UpdaterTimer.start(6);
    m_UpdateCoordinatesTimer.start(6);

    this->setFocus();
}

//SLOTS
void GameWindow::StartGame()
{
    m_Sounds.m_BeginningSound.play();

    StatusBarManager::ShowMessage("Game started", StatusBarManager::MESSAGE_TIMEOUT);

    m_UpdaterTimer.start(6);
    m_UpdateCoordinatesTimer.start(6);
    this->setFocus();
}

void GameWindow::CheckForRestartGameSignal()
{
    qDebug() << "Waiting for restart signal from server";
    if(m_GameState == GameState::Running && m_RestartPending) //1 is game is running again (set by server)
    {
        m_RestartPending = false;
        m_WaitingForRestartKey = false;

        RestartGame();
        m_WaitForRestartKeyTimer.stop();

        disconnect(&m_WaitForRestartKeyTimer, SIGNAL(timeout()), this, SLOT(CheckForRestartGameSignal()));
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

void GameWindow::UpdateCoordinatesFromServer()
{
    QByteArray dataReceivedFromServer = m_ServerConnection.GetDataFromServer();

    QRegularExpression coordinatesPattern("{D1(0|1|2|3|4)\\[x1:(\\d+),y1:(\\d+)];D2(0|1|2|3|4)\\[x2:(\\d+),y2:(\\d+)]},{\\[S:(0|1|2|3|4|5)\\],\\[G:(S|W|N)\\],\\[P:(\\d+)\\]},{'(.*)'}}");

    QRegularExpression changeStatePattern("{del:\\[(\\d+,\\d+)\\]}");

    QRegularExpressionMatch match1 = coordinatesPattern.match(dataReceivedFromServer);
    QRegularExpressionMatch match2 = changeStatePattern.match(dataReceivedFromServer);

    if(match1.hasMatch())
    {
        LogManager::LogToFile("MATCH 1" + dataReceivedFromServer.toStdString());

        QString pacmanX = match1.captured(2);
        QString pacmanY = match1.captured(3);
        QString ghostX = match1.captured(5);
        QString ghostY = match1.captured(6);

        m_Pacman.SetX(pacmanX.toInt());
        m_Pacman.SetY(pacmanY.toInt());
        m_Pacman.SetDirection(static_cast<Direction>(match1.captured(1).toInt()));

        m_Ghostplayer.SetX(ghostX.toInt());
        m_Ghostplayer.SetY(ghostY.toInt());
        m_Ghostplayer.SetDirection(static_cast<Direction>(match1.captured(4).toInt()));

        if(match1.captured(7) == "0")
        {
            m_GameState = GameState::BeforeFirstRun;
        }
        else if(match1.captured(7) == "1")
        {
            m_GameState = GameState::Running;
        }
        else if(match1.captured(7) == "2")
        {
            m_GameState = GameState::Paused;
        }
        else if(match1.captured(7) == "3")
        {
            m_GameState =GameState::Aborted;
        }
        else if(match1.captured(7) == "4")
        {
            m_GameState = GameState::PacmanWin;
        }
        else if(match1.captured(7) == "5")
        {
            m_GameState = GameState::GhostWin;
        }

        if(match1.captured(8) == "S")
        {
            m_Ghostplayer.SetScaredStateBlue(true);
            m_Ghostplayer.SetScaredStateWhite(false);
        }
        else if(match1.captured(8) == "W")
        {
            m_Ghostplayer.SetScaredStateWhite(true);
        }
        else if(match1.captured(8) == "N")
        {
            m_Ghostplayer.SetScaredStateBlue(false);
            m_Ghostplayer.SetScaredStateWhite(false);
        }
    }
    else
    {
        LogManager::LogToFile("MALFORMED DATA" + dataReceivedFromServer.toStdString());
    }

    if(match2.hasMatch())
    {
        LogManager::LogToFile("MATCH 2" + dataReceivedFromServer.toStdString());

        QString coordinates_of_object_to_remove = match2.captured(1);
        QMap<QString,QGraphicsEllipseItem*>::iterator iter1 = m_FoodballGraphicalItemsTableDict.find(coordinates_of_object_to_remove);
        QMap<QString,QGraphicsEllipseItem*>::iterator iter2 = m_PowerballGraphicalItemsTableDict.find(coordinates_of_object_to_remove);

        if(iter1 != m_FoodballGraphicalItemsTableDict.end())
        {
            m_FoodballGraphicalItemsTableDict.value(iter1.key())->hide();
            m_FoodballGraphicalItemsTableDict.remove(iter1.key());
            m_Sounds.m_EatSound.play();
        }
        else if(iter2 != m_PowerballGraphicalItemsTableDict.end())
        {
            m_PowerballGraphicalItemsTableDict.value(iter2.key())->hide();
            m_PowerballGraphicalItemsTableDict.remove(iter2.key());
        }
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
        m_RestartPending = true;
        HideSceneItems();

        m_TextScreenMessage.SetTextState("end_pacman");
        m_Scene.addItem(&m_TextScreenMessage);
        m_TextScreenMessage.show();

        qDebug() << "Pacman wins";

        StatusBarManager::ShowMessage("PACMAN WINS");

        m_UpdaterTimer.stop();

        connect(&m_WaitForRestartKeyTimer, SIGNAL(timeout()), this, SLOT(CheckForRestartGameSignal()), Qt::UniqueConnection);
        m_WaitForRestartKeyTimer.start(500);

        m_WaitingForRestartKey = true;
    }

    if((m_GameState == GameState::GhostWin) && (!m_WaitingForRestartKey))
    {
        m_RestartPending = true;
        HideSceneItems();

        m_TextScreenMessage.SetTextState("end_ghost");
        m_Scene.addItem(&m_TextScreenMessage);
        m_TextScreenMessage.show();

        qDebug() << "Ghost wins";

        StatusBarManager::ShowMessage("GHOST WINS");

        m_UpdaterTimer.stop();

        connect(&m_WaitForRestartKeyTimer, SIGNAL(timeout()), this, SLOT(CheckForRestartGameSignal()), Qt::UniqueConnection);
        m_WaitForRestartKeyTimer.start(500);

        m_WaitingForRestartKey = true;
    }

    m_Pacman.AdvanceAnimation();
    m_Ghostplayer.AdvanceAnimation();
}

GameWindow::~GameWindow()
{
    delete ui;
}
