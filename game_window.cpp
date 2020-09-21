#include "game_window.h"
#include "ui_Game_window.h"
#include "map.h"
#include "powerball.h"

GameWindow::GameWindow(QWidget *parent, QHostAddress _address) : QMainWindow(parent), ui(new Ui::Game_window)
{
    ui->setupUi(this);

    ui->gameplay_area->setScene(&scene);
    ui->gameplay_area->setRenderHint(QPainter::Antialiasing);
    scene.setSceneRect(0,0,614,740);
    ui->gameplay_area->setSceneRect(scene.sceneRect());

    game_state = 0;
    waitingforrestartkey = false;
    restartpending = false;

    //passing status bar and game state so clientconnection object can write messages directly to status bar
    clientconnection = new ClientConnection(ui->statusbar, &game_state, this);

    clientconnection->RequestConnection(_address, GAME_PORT);

    GenerateMap();
    PopulateMap();

    GenerateAndPlacePacman();
    GenerateAndPlaceGhosts();

    PrepareGameToStart();

    connect(&sceneupdate_timer, SIGNAL(timeout()), this, SLOT(UpdateScene()), Qt::UniqueConnection);
    connect(clientconnection, SIGNAL(GameStarted_signal()), this, SLOT(StartGame()), Qt::UniqueConnection);

    sceneupdate_timer.start(10);
}

void GameWindow::GenerateMap()
{
    map_item = scene.addPixmap(pac_map.getMap_Background_Picture());
}

void GameWindow::PopulateMap()
{
    powerball_positions = power_ball.getPowerBallPositions();
    foodball_positions = food_ball.getFoodBallPositions();

    foodball_items_count = foodball_positions.size();
    powerball_items_count = powerball_positions.size();

    for(int i=0;i<foodball_positions.size();i++)
    {
        //populate table of graphical items in following way (Key ; Value) = (QString - "x,y" ; pointer to QGraphicsEllipseItem at point ("x,y"))
        foodball_graphical_items_table_dict.insert(QString(QString::number(foodball_positions.at(i).x()) + "," + QString::number(foodball_positions.at(i).y())), scene.addEllipse(foodball_positions.at(i).x(),foodball_positions.at(i).y(),7,7,QPen(Qt::NoPen),QBrush(Qt::white)));
    }

    for(int i=0;i<powerball_positions.size();i++)
    {
        //populate table of graphical items in following way (Key ; Value) = (QString - "x,y" ; pointer to QGraphicsEllipseItem at point ("x,y"))
        powerball_graphical_items_table_dict.insert(QString(QString::number(powerball_positions.at(i).x()) + "," + QString::number(powerball_positions.at(i).y())), scene.addEllipse(powerball_positions.at(i).x()-5,powerball_positions.at(i).y()-8,15,15,QPen(Qt::NoPen),QBrush(Qt::white)));
    }

    assert(powerball_positions.size() == powerball_graphical_items_table_dict.size());
    assert(foodball_positions.size() == foodball_graphical_items_table_dict.size());

    qDebug("Foodball positions size: %d", foodball_positions.size());
}

void GameWindow::GenerateAndPlacePacman()
{
    pac_man.setDirection(0); //pacman does not move after game start

    pac_man.setPac_X(320);
    pac_man.setPac_Y(514);

    scene.addItem(&pac_man);
}

void GameWindow::GenerateAndPlaceGhosts()
{
    ghostplayer.setGhostDirection(0); //pacman does not move after game start

    ghostplayer.setGhost_X(307);
    ghostplayer.setGhost_Y(252);

    scene.addItem(&ghostplayer);
}

void GameWindow::PrepareGameToStart()
{
    connect(&updatertimer, SIGNAL(timeout()), this,SLOT(updater()), Qt::UniqueConnection);
    connect(&updatecoordinates_timer, SIGNAL(timeout()), this,SLOT(UpdateCoordinatesFromServer()), Qt::UniqueConnection);
}

void GameWindow::ResetVariablesandContainers()
{
    foodball_positions.clear();
    foodball_positions.squeeze();
    foodball_graphical_items_table.clear();
    foodball_graphical_items_table.squeeze();
    foodball_graphical_items_table_dict.clear();

    powerball_positions.clear();
    powerball_positions.squeeze();
    powerball_graphical_items_table.clear();
    powerball_graphical_items_table.squeeze();
    powerball_graphical_items_table_dict.clear();
}

void GameWindow::HideSceneItems()
{
    map_item->hide();
    pac_man.hide();
    ghostplayer.hide();

    scene.removeItem(&pac_man);
    scene.removeItem(&ghostplayer);

    for(QMap<QString, QGraphicsEllipseItem*>::iterator iter1 = foodball_graphical_items_table_dict.begin() ; iter1 != foodball_graphical_items_table_dict.end() ;iter1++)
    {
        foodball_graphical_items_table_dict.value(iter1.key())->hide();
    }

    for(QMap<QString, QGraphicsEllipseItem*>::iterator iter2 = powerball_graphical_items_table_dict.begin() ; iter2 != powerball_graphical_items_table_dict.end() ;iter2++)
    {
        powerball_graphical_items_table_dict.value(iter2.key())->hide();
    }
}

void GameWindow::RestartGame()
{
    pac_man.setDirection(0); //pacman does not move after game start
    pac_man.setPac_X(320);
    pac_man.setPac_Y(514);

    textscreenmessage.hide();
    scene.removeItem(&textscreenmessage);

    qDebug() << "Restarting game";

    ResetVariablesandContainers();
    PopulateMap();
    PrepareGameToStart();

    map_item->show();
    scene.addItem(&pac_man);
    pac_man.show();
    scene.addItem(&ghostplayer);
    ghostplayer.show();

    sounds.beginning_sound.play();

    ui->statusbar->showMessage("Game started", 3000);
    updatertimer.start(6);
    updatecoordinates_timer.start(6);

    this->setFocus(); //gives the keyboard input focus to this widget
}

//SLOTS
void GameWindow::StartGame()
{
    sounds.beginning_sound.play();

    ui->statusbar->showMessage("Game started", 3000);
    updatertimer.start(6);
    updatecoordinates_timer.start(6);
    this->setFocus(); //gives the keyboard input focus to this widget
}

void GameWindow::CheckForRestartGameSignal()
{
    qDebug() << "Waiting for restart signal from server";
    if(game_state == 1 && restartpending) //1 is game is running again (set by server)
    {
        restartpending = false;
        waitingforrestartkey = false;

        RestartGame();
        wait_for_restart_key_timer.stop();

        disconnect(&wait_for_restart_key_timer, SIGNAL(timeout()), this, SLOT(CheckForRestartGameSignal()));
        qDebug() << "GAME RESTARTED on client side";
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event) //supports pacman movement using WSAD and directional keys
{
    switch(event->key())
    {
    case Qt::Key_A:
    case Qt::Key_Left:
        clientconnection->SendPressedKeyToServer('a');
        break;

    case Qt::Key_D:
    case Qt::Key_Right:
        clientconnection->SendPressedKeyToServer('d');
        break;

    case Qt::Key_S:
    case Qt::Key_Down:
        clientconnection->SendPressedKeyToServer('s');
        break;

    case Qt::Key_W:
    case Qt::Key_Up:
        clientconnection->SendPressedKeyToServer('w');
        break;

    case Qt::Key_Space:
        if(game_state == 4 || game_state == 5)
        {
            clientconnection->SendPressedKeyToServer('7');
        }
        else
        {
            clientconnection->SendPressedKeyToServer('5');
        }
        break;

    default:
        break;
    }
}

void GameWindow::UpdateCoordinatesFromServer()
{
    QByteArray data_received = clientconnection->getCoordinates();
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

        pac_man.setPac_X(player1_x_part.toInt());
        pac_man.setPac_Y(player1_y_part.toInt());
        pac_man.setDirection(match.captured(1).toInt());

        ghostplayer.setGhost_X(player2_x_part.toInt());
        ghostplayer.setGhost_Y(player2_y_part.toInt());
        ghostplayer.setGhostDirection(match.captured(4).toInt());

        if(match.captured(7) == "0")
        {
            game_state = 0;
        }
        else if(match.captured(7) == "1")
        {
            game_state = 1;
        }
        else if(match.captured(7) == "2")
        {
            game_state = 2;
        }
        else if(match.captured(7) == "3")
        {
            game_state = 3;
        }
        else if(match.captured(7) == "4")
        {
            game_state = 4;
        }
        else if(match.captured(7) == "5")
        {
            game_state = 5;
        }

        if(match.captured(8) == "S")
        {
            ghostplayer.setIsScared(true);
            ghostplayer.setScaredWhite(false);
        }
        else if(match.captured(8) == "W")
        {
            ghostplayer.setScaredWhite(true);
        }
        else if(match.captured(8) == "N")
        {
            ghostplayer.setIsScared(false);
            ghostplayer.setScaredWhite(false);
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
        QMap<QString,QGraphicsEllipseItem*>::iterator iter1 = foodball_graphical_items_table_dict.find(coordinates_of_object_to_remove);
        QMap<QString,QGraphicsEllipseItem*>::iterator iter2 = powerball_graphical_items_table_dict.find(coordinates_of_object_to_remove);

        if(iter1 != foodball_graphical_items_table_dict.end())
        {
            foodball_graphical_items_table_dict.value(iter1.key())->hide();
            foodball_graphical_items_table_dict.remove(iter1.key());
            sounds.eat_sound.play();
        }
        else if(iter2 != powerball_graphical_items_table_dict.end())
        {
            powerball_graphical_items_table_dict.value(iter2.key())->hide();
            powerball_graphical_items_table_dict.remove(iter2.key());
        }
    }
}

void GameWindow::UpdateScene()
{
    scene.update(scene.sceneRect());
}

void GameWindow::updater()
{
    //PACMAN WINS
    if((game_state == 4) && (!waitingforrestartkey))
    {
        restartpending = true;
        HideSceneItems();

        textscreenmessage.setTextState("end_pacman");
        scene.addItem(&textscreenmessage);
        textscreenmessage.show();

        qDebug() << "Pacman wins";
        ui->statusbar->showMessage("PACMAN WINS");
        updatertimer.stop();

        connect(&wait_for_restart_key_timer, SIGNAL(timeout()), this, SLOT(CheckForRestartGameSignal()), Qt::UniqueConnection);
        wait_for_restart_key_timer.start(500);

        waitingforrestartkey = true;
    }

    //GHOST WINS
    if((game_state == 5) && (!waitingforrestartkey))
    {
        restartpending = true;
        HideSceneItems();

        textscreenmessage.setTextState("end_ghost");
        scene.addItem(&textscreenmessage);
        textscreenmessage.show();

        qDebug() << "Ghost wins";
        ui->statusbar->showMessage("GHOST WINS");
        updatertimer.stop();

        connect(&wait_for_restart_key_timer, SIGNAL(timeout()), this, SLOT(CheckForRestartGameSignal()), Qt::UniqueConnection);
        wait_for_restart_key_timer.start(500);

        waitingforrestartkey = true;
    }

    pac_man.advance();
    ghostplayer.advance();
}

GameWindow::~GameWindow()
{
    delete ui;
    delete clientconnection;
}
