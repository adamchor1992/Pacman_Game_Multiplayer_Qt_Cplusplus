#include "game_window.h"
#include "ui_game_window.h"
#include "map.h"
#include "powerball.h"

GameWindow::GameWindow(QWidget *parent, QHostAddress address) : QMainWindow(parent), ui(new Ui::Game_window)
{
    ui->setupUi(this);

    ui->gameplay_area->setScene(&m_Scene);
    ui->gameplay_area->setRenderHint(QPainter::Antialiasing);
    m_Scene.setSceneRect(0,0,614,740);
    ui->gameplay_area->setSceneRect(m_Scene.sceneRect());

    m_GameState = 0;
    m_WaitingForRestartKey = false;
    m_RestartPending = false;

    //passing status bar and game state so clientconnection object can write messages directly to status bar
    m_pClientConnection = new ClientConnection(ui->statusbar, &m_GameState, this);

    m_pClientConnection->RequestConnection(address, GAME_PORT);

    GenerateMap();
    PopulateMap();

    GenerateAndPlacePacman();
    GenerateAndPlaceGhosts();

    PrepareGameToStart();

    connect(&m_SceneUpdateTimer, SIGNAL(timeout()), this, SLOT(UpdateScene()), Qt::UniqueConnection);
    connect(m_pClientConnection, SIGNAL(GameStarted_signal()), this, SLOT(StartGame()), Qt::UniqueConnection);

    m_SceneUpdateTimer.start(10);
}

void GameWindow::GenerateMap()
{
    m_pMapItem = m_Scene.addPixmap(m_Pacmap.GetMapBackgroundPicture());
}

void GameWindow::PopulateMap()
{
    m_PowerballPositions = m_Powerball.GetPowerBallPositions();
    m_FoodballPositions = m_FoodBall.GetFoodBallPositions();

    m_FoodballItemsCount = m_FoodballPositions.size();
    m_PowerballItemsCount = m_PowerballPositions.size();

    for(int i=0; i <m_FoodballPositions.size(); i++)
    {
        //populate table of graphical items in following way (Key ; Value) = (QString - "x,y" ; pointer to QGraphicsEllipseItem at point ("x,y"))
        m_FoodballGraphicalItemsTableDict.insert(QString(QString::number(m_FoodballPositions.at(i).x()) + "," + QString::number(m_FoodballPositions.at(i).y())), m_Scene.addEllipse(m_FoodballPositions.at(i).x(),m_FoodballPositions.at(i).y(),7,7,QPen(Qt::NoPen),QBrush(Qt::white)));
    }

    for(int i=0; i <m_PowerballPositions.size(); i++)
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
    m_Pacman.SetDirection(0); //pacman does not move after game start

    m_Pacman.SetX(320);
    m_Pacman.SetY(514);

    m_Scene.addItem(&m_Pacman);
}

void GameWindow::GenerateAndPlaceGhosts()
{
    m_Ghostplayer.SetDirection(0); //pacman does not move after game start

    m_Ghostplayer.SetX(307);
    m_Ghostplayer.SetY(252);

    m_Scene.addItem(&m_Ghostplayer);
}

void GameWindow::PrepareGameToStart()
{
    connect(&m_UpdaterTimer, SIGNAL(timeout()), this,SLOT(Updater()), Qt::UniqueConnection);
    connect(&m_IpdateCoordinatesTimer, SIGNAL(timeout()), this,SLOT(UpdateCoordinatesFromServer()), Qt::UniqueConnection);
}

void GameWindow::ResetVariablesandContainers()
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
    m_Pacman.SetDirection(0); //pacman does not move after game start
    m_Pacman.SetX(320);
    m_Pacman.SetY(514);

    m_TextScreenMessage.hide();
    m_Scene.removeItem(&m_TextScreenMessage);

    qDebug() << "Restarting game";

    ResetVariablesandContainers();
    PopulateMap();
    PrepareGameToStart();

    m_pMapItem->show();
    m_Scene.addItem(&m_Pacman);
    m_Pacman.show();
    m_Scene.addItem(&m_Ghostplayer);
    m_Ghostplayer.show();

    m_Sounds.m_BeginningSound.play();

    ui->statusbar->showMessage("Game started", 3000);
    m_UpdaterTimer.start(6);
    m_IpdateCoordinatesTimer.start(6);

    this->setFocus(); //gives the keyboard input focus to this widget
}

//SLOTS
void GameWindow::StartGame()
{
    m_Sounds.m_BeginningSound.play();

    ui->statusbar->showMessage("Game started", 3000);
    m_UpdaterTimer.start(6);
    m_IpdateCoordinatesTimer.start(6);
    this->setFocus(); //gives the keyboard input focus to this widget
}

void GameWindow::CheckForRestartGameSignal()
{
    qDebug() << "Waiting for restart signal from server";
    if(m_GameState == 1 && m_RestartPending) //1 is game is running again (set by server)
    {
        m_RestartPending = false;
        m_WaitingForRestartKey = false;

        RestartGame();
        m_WaitForRestartKeyTimer.stop();

        disconnect(&m_WaitForRestartKeyTimer, SIGNAL(timeout()), this, SLOT(CheckForRestartGameSignal()));
        qDebug() << "GAME RESTARTED on client side";
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event) //supports pacman movement using WSAD and directional keys
{
    switch(event->key())
    {
    case Qt::Key_A:
    case Qt::Key_Left:
        m_pClientConnection->SendPressedKeyToServer('a');
        break;

    case Qt::Key_D:
    case Qt::Key_Right:
        m_pClientConnection->SendPressedKeyToServer('d');
        break;

    case Qt::Key_S:
    case Qt::Key_Down:
        m_pClientConnection->SendPressedKeyToServer('s');
        break;

    case Qt::Key_W:
    case Qt::Key_Up:
        m_pClientConnection->SendPressedKeyToServer('w');
        break;

    case Qt::Key_Space:
        if(m_GameState == 4 || m_GameState == 5)
        {
            m_pClientConnection->SendPressedKeyToServer('7');
        }
        else
        {
            m_pClientConnection->SendPressedKeyToServer('5');
        }
        break;

    default:
        break;
    }
}

void GameWindow::UpdateCoordinatesFromServer()
{
    QByteArray data_received = m_pClientConnection->GetCoordinates();
    QString player1_x_part;
    QString player1_y_part;
    QString player2_x_part;
    QString player2_y_part;

    QRegularExpression coordinates_pattern("{D1(0|1|2|3|4)\\[x1:(\\d+),y1:(\\d+)];D2(0|1|2|3|4)\\[x2:(\\d+),y2:(\\d+)]},{\\[S:(0|1|2|3|4|5)\\],\\[G:(S|W|N)\\],\\[P:(\\d+)\\]},{'(.+)'}}");

    QRegularExpression change_state_pattern("{del:\\[(\\d+,\\d+)\\]}");

    QRegularExpressionMatch match = coordinates_pattern.match(data_received);
    QRegularExpressionMatch match2 = change_state_pattern.match(data_received);

    if(match.hasMatch())
    {
        player1_x_part = match.captured(2);
        player1_y_part = match.captured(3);
        player2_x_part = match.captured(5);
        player2_y_part = match.captured(6);

        m_Pacman.SetX(player1_x_part.toInt());
        m_Pacman.SetY(player1_y_part.toInt());
        m_Pacman.SetDirection(match.captured(1).toInt());

        m_Ghostplayer.SetX(player2_x_part.toInt());
        m_Ghostplayer.SetY(player2_y_part.toInt());
        m_Ghostplayer.SetDirection(match.captured(4).toInt());

        if(match.captured(7) == "0")
        {
            m_GameState = 0;
        }
        else if(match.captured(7) == "1")
        {
            m_GameState = 1;
        }
        else if(match.captured(7) == "2")
        {
            m_GameState = 2;
        }
        else if(match.captured(7) == "3")
        {
            m_GameState = 3;
        }
        else if(match.captured(7) == "4")
        {
            m_GameState = 4;
        }
        else if(match.captured(7) == "5")
        {
            m_GameState = 5;
        }

        if(match.captured(8) == "S")
        {
            m_Ghostplayer.SetScaredStateBlue(true);
            m_Ghostplayer.SetScaredStateWhite(false);
        }
        else if(match.captured(8) == "W")
        {
            m_Ghostplayer.SetScaredStateWhite(true);
        }
        else if(match.captured(8) == "N")
        {
            m_Ghostplayer.SetScaredStateBlue(false);
            m_Ghostplayer.SetScaredStateWhite(false);
        }
    }
    else
    {
        qDebug() << data_received;
        qDebug() << "PACKAGE LOST: Wrong coordinate package";
    }

    if(match2.hasMatch())
    {
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
    //PACMAN WINS
    if((m_GameState == 4) && (!m_WaitingForRestartKey))
    {
        m_RestartPending = true;
        HideSceneItems();

        m_TextScreenMessage.SetTextState("end_pacman");
        m_Scene.addItem(&m_TextScreenMessage);
        m_TextScreenMessage.show();

        qDebug() << "Pacman wins";
        ui->statusbar->showMessage("PACMAN WINS");
        m_UpdaterTimer.stop();

        connect(&m_WaitForRestartKeyTimer, SIGNAL(timeout()), this, SLOT(CheckForRestartGameSignal()), Qt::UniqueConnection);
        m_WaitForRestartKeyTimer.start(500);

        m_WaitingForRestartKey = true;
    }

    //GHOST WINS
    if((m_GameState == 5) && (!m_WaitingForRestartKey))
    {
        m_RestartPending = true;
        HideSceneItems();

        m_TextScreenMessage.SetTextState("end_ghost");
        m_Scene.addItem(&m_TextScreenMessage);
        m_TextScreenMessage.show();

        qDebug() << "Ghost wins";
        ui->statusbar->showMessage("GHOST WINS");
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
    delete m_pClientConnection;
}
