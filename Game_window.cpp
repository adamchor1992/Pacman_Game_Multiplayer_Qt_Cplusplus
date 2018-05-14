#include "Game_window.h"
#include "ui_Game_window.h"
#include "map.h"
#include "powerball.h"

Game_window::Game_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Game_window)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->gameplay_area->setScene(scene);
    ui->gameplay_area->setRenderHint(QPainter::Antialiasing);
    scene->setSceneRect(0,0,614,740);
    ui->gameplay_area->setSceneRect(scene->sceneRect());

    clientconnection = new ClientConnection(this);
    clientconnection->RequestConnection();

    GenerateMap();
    PopulateMap();

    GenerateAndPlacePacman();
    GenerateAndPlaceGhosts();

    //ShowScoresAndLives();

    StartGame();

//    collision_detection_delay = 0; //delay collision detection after game restart

//    text_start_end = new TextStartEnd;
//    scene->addItem(text_start_end);

//    playing=false;

//    beginning_sound = new QMediaPlayer;
//    eat_sound1 = new QMediaPlayer;
//    eat_sound2 = new QMediaPlayer;
//    eat_ghost_sound = new QMediaPlayer;
//    pacman_death_sound = new QMediaPlayer;

//    beginning_sound->setMedia(QUrl("qrc:/sounds/pacman_beginning.wav"));
//    eat_sound1->setMedia(QUrl("qrc:/sounds/pacman_eat.wav"));
//    eat_sound2->setMedia(QUrl("qrc:/sounds/pacman_eat.wav"));
//    eat_ghost_sound->setMedia(QUrl("qrc:/sounds/pacman_eatghost.wav"));
//    pacman_death_sound->setMedia(QUrl("qrc:/sounds/pacman_death.wav"));

//    this->setFocus(Qt::ActiveWindowFocusReason);
}

void Game_window::GenerateMap()
{
    pac_map=new Map;
    map_item = scene->addPixmap(pac_map->getMap_Background_Picture());
}

void Game_window::PopulateMap()
{
    power_ball=new PowerBall;
    food_ball=new FoodBall;

    powerball_positions = new QVector<QPoint>;
    foodball_positions = new QVector<QPoint>;

    *powerball_positions = power_ball->getPowerBallPositions();
    *foodball_positions = food_ball->getFoodBallPositions();

    foodball_graphical_items_table_dict = new QMap<QString, QGraphicsEllipseItem*>;
    powerball_graphical_items_table_dict = new QMap<QString, QGraphicsEllipseItem*>;

    foodball_items_count = foodball_positions->size();
    powerball_items_count = powerball_positions->size();

    for(int i=0;i<foodball_positions->size();i++)
    {
        //populate table of graphical items in following way (Key ; Value) = (QString - "x,y" ; pointer to QGraphicsEllipseItem at point ("x,y"))
        foodball_graphical_items_table_dict->insert(QString(QString::number(foodball_positions->at(i).x()) + "," + QString::number(foodball_positions->at(i).y())), scene->addEllipse(foodball_positions->at(i).x(),foodball_positions->at(i).y(),7,7,QPen(Qt::NoPen),QBrush(Qt::white)));
    }

    for(int i=0;i<powerball_positions->size();i++)
    {
        //populate table of graphical items in following way (Key ; Value) = (QString - "x,y" ; pointer to QGraphicsEllipseItem at point ("x,y"))
        powerball_graphical_items_table_dict->insert(QString(QString::number(powerball_positions->at(i).x()) + "," + QString::number(powerball_positions->at(i).y())), scene->addEllipse(powerball_positions->at(i).x()-5,powerball_positions->at(i).y()-8,15,15,QPen(Qt::NoPen),QBrush(Qt::white)));
    }

    assert(powerball_positions->size() == powerball_graphical_items_table_dict->size());
    assert(foodball_positions->size() == foodball_graphical_items_table_dict->size());

    qDebug("Foodball positions size: %d", foodball_positions->size());
}

void Game_window::GenerateAndPlacePacman()
{
    pac_man = new Pacman;

    pac_man->setDirection(1); //pacman moves left after game start

    pac_man->setPac_X(320);
    pac_man->setPac_Y(514);

    scene->addItem(pac_man);
}

void Game_window::GenerateAndPlaceGhosts()
{
    ghostplayer = new Ghost;

    ghostplayer->setGhostDirection(1); //pacman moves left after game start

    ghostplayer->setGhost_X(320);
    ghostplayer->setGhost_Y(514);

    scene->addItem(ghostplayer);
}

void Game_window::StartGame()
{
    //beginning_sound->play();

    //text_start_end->hide();

    updatertimer = new QTimer(this);
    updatecoordinates_timer = new QTimer(this);

    //ghostsupdatertimer=new Qupdatertimer(this);
    connect(updatertimer, SIGNAL(timeout()), this,SLOT(updater()));
    connect(updatecoordinates_timer, SIGNAL(timeout()), this,SLOT(UpdateCoordinatesFromServer()));
    //connect(ghostsupdatertimer, SIGNAL(timeout()), this,SLOT(ghostupdater()));
    updatertimer->start(6);
    updatecoordinates_timer->start(6);
    //ghostsupdatertimer->start(6);

    this->setFocus(); //gives the keyboard input focus to this widget
}

void Game_window::keyPressEvent(QKeyEvent *event) //supports pacman movement using WSAD and directional keys
{
    //int pacman_nextdirection=pac_man->getNextDirection();
    //int ghost_nextdirection=ghost1->getNextGhostDirection();

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

        //    case Qt::Key_Space:
        //        if(!playing)
        //        {
        //            StartGame();
        //            playing=true;
        //        }

    default:
        break;
    }
}

void Game_window::UpdateCoordinatesFromServer()
{
    QByteArray data_received = clientconnection->getCoordinates();
    QString player1_x_part;
    QString player1_y_part;
    QString player2_x_part;
    QString player2_y_part;

    QRegularExpression coordinates_pattern("{D1(1|2|3|4)\\[x1:(\\d+),y1:(\\d+)];D2(1|2|3|4)\\[x2:(\\d+),y2:(\\d+)]},{\\[G:(S|W|N)\\],\\[P:(\\d+)\\]},{'(.+)'}}");

    QRegularExpression change_state_pattern("{del:\\[(\\d+,\\d+)\\]}");

    QRegularExpressionMatch match = coordinates_pattern.match(data_received);
    QRegularExpressionMatch match2 = change_state_pattern.match(data_received);

    if(match.hasMatch())
    {
        player1_x_part = match.captured(2);
        player1_y_part = match.captured(3);
        player2_x_part = match.captured(5);
        player2_y_part = match.captured(6);

        pac_man->setPac_X(player1_x_part.toInt());
        pac_man->setPac_Y(player1_y_part.toInt());
        pac_man->setDirection(match.captured(1).toInt());

        ghostplayer->setGhost_X(player2_x_part.toInt());
        ghostplayer->setGhost_Y(player2_y_part.toInt());
        ghostplayer->setGhostDirection(match.captured(4).toInt());

        if(match.captured(7) == "S")
        {
            ghostplayer->setIsScared(true);
            ghostplayer->setScaredWhite(false);
        }
        else if(match.captured(7) == "W")
        {
            ghostplayer->setIsScared(false);
            ghostplayer->setScaredWhite(true);
        }
        else if(match.captured(7) == "N")
        {
            ghostplayer->setIsScared(false);
            ghostplayer->setScaredWhite(false);
        }

        //qDebug() << match.captured(7); //GHOSTscared
        //qDebug() << match.captured(8); //POINTS
        //qDebug() << match.captured(9); //MESSAGE

        //qDebug() <<"Pacman (x,y) on client (" << pac_man->getPac_X() << "," << pac_man->getPac_Y() << ")";
    }
    else
    {
        qDebug() << data_received;
        qDebug() << "PACKAGE LOST: Wrong coordinate package";
    }

    if(match2.hasMatch())
    {
        QString coordinates_of_object_to_remove = match2.captured(1);
        QMap<QString,QGraphicsEllipseItem*>::iterator iter1 = foodball_graphical_items_table_dict->find(coordinates_of_object_to_remove);
        QMap<QString,QGraphicsEllipseItem*>::iterator iter2 = powerball_graphical_items_table_dict->find(coordinates_of_object_to_remove);

        if(iter1 != foodball_graphical_items_table_dict->end())
        {
            foodball_graphical_items_table_dict->value(iter1.key())->hide();
            foodball_graphical_items_table_dict->remove(iter1.key());
            qDebug() << "Client has " << foodball_graphical_items_table_dict->size() << "foodballs left";
        }
        else if(iter2 != powerball_graphical_items_table_dict->end())
        {
            powerball_graphical_items_table_dict->value(iter2.key())->hide();
            powerball_graphical_items_table_dict->remove(iter2.key());
            qDebug() << "Client has " << powerball_graphical_items_table_dict->size() << "powerballs left";
        }
    }
}

void Game_window::updater()
{
//    if(collision_detection_delay >= 500)
//        CheckCollision();
//    else
//        collision_detection_delay++;



//    if(foodball_items_count==0)
//    {
//        updatertimer->stop();
//        ghostsupdatertimer->stop();
//        EndGame(1);
//    }

//qDebug("%d", scarestate);

//    if(scared)
//    {
//        scarestate+=1;

//        if(scarestate==1)
//        {
//            ghostsupdatertimer->setInterval(15);
//        }

//        if(scarestate==750)
//        {
//            ghost1->setScaredWhite(true);
//            ghost2->setScaredWhite(true);
//            ghost3->setScaredWhite(true);
//            ghost4->setScaredWhite(true);
//        }

//        if(scarestate==1000)
//        {
//            scared=false;
//            ghost1->setIsScared(false);
//            ghost2->setIsScared(false);
//            ghost3->setIsScared(false);
//            ghost4->setIsScared(false);

//            ghost1->setScaredWhite(false);
//            ghost2->setScaredWhite(false);
//            ghost3->setScaredWhite(false);
//            ghost4->setScaredWhite(false);

//            scarestate = 0;

//            ghostsupdatertimer->setInterval(4);
//        }
//    }
//    else
//    {
//        ghostsupdatertimer->setInterval(4);
//    }

    pac_man->advance();
    ghostplayer->advance();

    //qDebug("Items left: %d", foodball_items_count);

    scene->update(scene->sceneRect());
    //qDebug("Pacman coordinates: (%d,%d)", pac_x, pac_y);
}

Game_window::~Game_window()
{
    delete ui;
}
