#include "pacmanserver.h"


PacmanServer::PacmanServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    updatertimer = new QTimer(this);
    sendcoordinates_player1_timer = new QTimer(this);
    sendcoordinates_player2_timer = new QTimer(this);

    serversocket1 = new QTcpSocket(this);
    serversocket2 = new QTcpSocket(this);

    updatertimer->start(4);
    sendcoordinates_player1_timer->start(5);
    sendcoordinates_player2_timer->start(5);

    connect(server,SIGNAL(newConnection()), this, SLOT(AcceptConnection()));
    connect(updatertimer, SIGNAL(timeout()), this, SLOT(updater()));

    pac_map = new Map;
    pac_man = new Pacman;
    ghostplayer = new Ghost;
    food_ball = new FoodBall;
    power_ball = new PowerBall;


    powerball_positions = new QVector<QPoint>;
    foodball_positions = new QVector<QPoint>;

    *powerball_positions = power_ball->getPowerBallPositions();
    *foodball_positions = food_ball->getFoodBallPositions();

    foodball_items_count = foodball_positions->size();
    powerball_items_count = powerball_positions->size();


    qDebug("Foodball positions size on server: %d", foodball_positions->size());


    pac_man->setPac_X(320);
    pac_man->setPac_Y(514);
    pac_man->setDirection(1); //pacman moves left after game start

    ghostplayer->setGhost_X(320);
    ghostplayer->setGhost_Y(514);
    ghostplayer->setGhostDirection(1);

    player1points=0;
}

void PacmanServer::StartListening()
{
    if(server->listen(QHostAddress::Any, 23))
    {
        qDebug() << "Server started";
    }
    else
    {
        qDebug() << "Server could not start";
        qDebug() << server->errorString();
    }
}

//SLOT

void PacmanServer::AcceptConnection()
{
    //IF THERE IS NEW CONNECTION

    if(serversocket1->state() == QAbstractSocket::UnconnectedState)
    {
        serversocket1 = server->nextPendingConnection();

        connect(serversocket1,SIGNAL(connected()), this, SLOT(connected1()));
        connect(serversocket1,SIGNAL(disconnected()), this, SLOT(disconnected1()));
        connect(serversocket1,SIGNAL(readyRead()), this, SLOT(ReadDirectionFromClient1()));
        connect(serversocket1,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten1(qint64)));

        connect(sendcoordinates_player1_timer, SIGNAL(timeout()), this, SLOT(SendcoordinatesToClient1()));

        qDebug() << "Connection assigned to socket 1";
        serversocket1->write("You are PLAYER 1 - PACMAN");
    }
    else
    {
        serversocket2 = server->nextPendingConnection();

        connect(serversocket2,SIGNAL(connected()), this, SLOT(connected2()));
        connect(serversocket2,SIGNAL(disconnected()), this, SLOT(disconnected2()));
        connect(serversocket2,SIGNAL(readyRead()), this, SLOT(ReadDirectionFromClient2()));
        connect(serversocket2,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten2(qint64)));

        connect(sendcoordinates_player2_timer, SIGNAL(timeout()), this, SLOT(SendcoordinatesToClient2()));

        qDebug() << "Connection assigned to socket 2";
        serversocket2->write("You are PLAYER 2 - GHOST");
    }
}


void PacmanServer::SendcoordinatesToClient1()
{
    serversocket1->write(info_package_for_clients_packed);
//    int bytes;
//    if(bytes = serversocket1->write(coordinates_players_packed))
//    {
//       qDebug() << "Server sent "<< bytes <<" bytes with those coordinates:      " << coordinates_players_packed;
//    }
}

void PacmanServer::SendcoordinatesToClient2()
{
    serversocket2->write(info_package_for_clients_packed);
//    int bytes;
//    if(bytes = serversocket2->write(coordinates_players_packed))
//    {
//       qDebug() << "Server sent "<< bytes <<" bytes with those coordinates:      " << coordinates_players_packed;
//    }
}

void PacmanServer::Player1Move()
{
    QPoint p;

    int pac_x = pac_man->getPac_X();
    int pac_y = pac_man->getPac_Y();
    int direction = pac_man->getDirection();
    int nextdirection = pac_man->getNextDirection();

    if(nextdirection!=direction)
    {
        switch(nextdirection)
        {
        case 1: //left
            p.setX(pac_x-1);
            p.setY(pac_y);

            if(pac_map->IsPointAvailable(p))
            {
                direction=nextdirection;
                nextdirection=0;
            }
            break;

        case 2: //up
            p.setX(pac_x);
            p.setY(pac_y-1);
            if(pac_map->IsPointAvailable(p))
            {
                direction=nextdirection;
                nextdirection=0;
            }
            break;

        case 3: //down
            p.setX(pac_x);
            p.setY(pac_y+1);
            if(pac_map->IsPointAvailable(p))
            {
                direction=nextdirection;
                nextdirection=0;
            }
            break;

        case 4: //right
            p.setX(pac_x+1);
            p.setY(pac_y);
            if(pac_map->IsPointAvailable(p))
            {
                direction=nextdirection;
                nextdirection=0;
            }
            break;
        }
    }

    switch(direction)
    {
    case 1: //left
        p.setX(pac_x-1);
        p.setY(pac_y);
        pac_man->setDirection(direction);

        if(pac_map->IsPointAvailable(p))
        {
            pac_x = pac_x - 1;
        }

        break;

    case 2: //up
        p.setX(pac_x);
        p.setY(pac_y-1);
        pac_man->setDirection(direction);

        if(pac_map->IsPointAvailable(p))
        {
            pac_y= pac_y - 1;
        }

        break;

    case 3: //down
        p.setX(pac_x);
        p.setY(pac_y+1);
        pac_man->setDirection(direction);

        if(pac_map->IsPointAvailable(p))
        {
            pac_y= pac_y + 1;
        }

        break;

    case 4: //right
        p.setX(pac_x+1);
        p.setY(pac_y);
        pac_man->setDirection(direction);

        if(pac_map->IsPointAvailable(p))
        {
            pac_x = pac_x + 1;
        }

        break;
    }

    if(pac_x==0 && pac_y==318) //teleportation when reached left boundary of middle horizontal line
    {
        pac_x=613;
    }

    if(pac_x==614 && pac_y==318) //teleportation when reached right boundary of middle horizontal line
    {
        pac_x=1;
    }

    pac_man->setPac_X(pac_x);
    pac_man->setPac_Y(pac_y);
}

void PacmanServer::Player2Move()
{
        QPoint p;

        int ghost_x = ghostplayer->getGhost_X();
        int ghost_y = ghostplayer->getGhost_Y();
        int ghost_direction = ghostplayer->getGhostDirection();
        int nextghostdirection = ghostplayer->getNextGhostDirection();

        if(nextghostdirection!=ghost_direction)
        {
            switch(nextghostdirection)
            {
            case 1: //left
                p.setX(ghost_x-1);
                p.setY(ghost_y);

                if(pac_map->IsPointAvailable(p))
                {
                    ghost_direction=nextghostdirection;
                    nextghostdirection=0;
                }
                break;

            case 2: //up
                p.setX(ghost_x);
                p.setY(ghost_y-1);
                if(pac_map->IsPointAvailable(p))
                {
                    ghost_direction=nextghostdirection;
                    nextghostdirection=0;
                }
                break;

            case 3: //down
                p.setX(ghost_x);
                p.setY(ghost_y+1);
                if(pac_map->IsPointAvailable(p))
                {
                    ghost_direction=nextghostdirection;
                    nextghostdirection=0;
                }
                break;

            case 4: //right
                p.setX(ghost_x+1);
                p.setY(ghost_y);
                if(pac_map->IsPointAvailable(p))
                {
                    ghost_direction=nextghostdirection;
                    nextghostdirection=0;
                }
                break;
            }
        }

        switch(ghost_direction)
        {
        case 1: //left
            p.setX(ghost_x-1);
            p.setY(ghost_y);
            ghostplayer->setGhostDirection(ghost_direction);

            if(pac_map->IsPointAvailable(p))
            {
                ghost_x = ghost_x - 1;
            }

            break;

        case 2: //up
            p.setX(ghost_x);
            p.setY(ghost_y-1);
            ghostplayer->setGhostDirection(ghost_direction);

            if(pac_map->IsPointAvailable(p))
            {
                ghost_y= ghost_y - 1;
            }

            break;

        case 3: //down
            p.setX(ghost_x);
            p.setY(ghost_y+1);
            ghostplayer->setGhostDirection(ghost_direction);

            if(pac_map->IsPointAvailable(p))
            {
                ghost_y= ghost_y + 1;
            }

            break;

        case 4: //right
            p.setX(ghost_x+1);
            p.setY(ghost_y);
            ghostplayer->setGhostDirection(ghost_direction);

            if(pac_map->IsPointAvailable(p))
            {
                ghost_x = ghost_x + 1;
            }

            break;
        }

        if(ghost_x==0 && ghost_y==318) //teleportation when reached left boundary of middle horizontal line
        {
            ghost_x=613;
        }

        if(ghost_x==614 && ghost_y==318) //teleportation when reached right boundary of middle horizontal line
        {
            ghost_x=1;
        }

        ghostplayer->setGhost_X(ghost_x);
        ghostplayer->setGhost_Y(ghost_y);
}


void PacmanServer::updater()
{
    int pac_x = pac_man->getPac_X();
    int pac_y = pac_man->getPac_Y();

    if(serversocket1->state() == QAbstractSocket::ConnectedState)
    {      
        QByteArray player1_direction = QByteArray::number(pac_man->getDirection());
        QByteArray player2_direction = QByteArray::number(ghostplayer->getGhostDirection());

        QByteArray player1_x_coordinate = QByteArray::number(pac_man->getPac_X());
        QByteArray player1_y_coordinate = QByteArray::number(pac_man->getPac_Y());

        QByteArray player2_x_coordinate = QByteArray::number(ghostplayer->getGhost_X());
        QByteArray player2_y_coordinate = QByteArray::number(ghostplayer->getGhost_Y());

        QByteArray player1_x_coordinate_packed = "{{D1" + player1_direction + "[x1:"+player1_x_coordinate+",";
        QByteArray player1_y_coordinate_packed = "y1:" + player1_y_coordinate + "];";
        QByteArray player2_x_coordinate_packed = "D2" + player2_direction + "[x2:"+player2_x_coordinate+",";
        QByteArray player2_y_coordinate_packed = "y2:" + player2_y_coordinate + "]},";

        if(ghostplayer->getIsScared()) //scared blue
        {
            is_ghost_scared_white_packed = "{[G:S],";
        }
        else if(ghostplayer->getScaredWhite()) //scared white
        {
            is_ghost_scared_white_packed = "{[G:W],";
        }
        else //no scared
        {
            is_ghost_scared_white_packed = "{[G:N],";
        }

        QByteArray points_packed = "[P:" + QByteArray::number(player1points) + "]},";

        //QByteArray message = "hello";
        QByteArray message_packed = "{'" + messagetowrite + "'}}";

        info_package_for_clients_packed = player1_x_coordinate_packed + player1_y_coordinate_packed + player2_x_coordinate_packed + player2_y_coordinate_packed +
                is_ghost_scared_white_packed + points_packed + message_packed;

    //qDebug() << "info_package_for_clients_packed" << info_package_for_clients_packed;

        //("\\{del:[(\\d+),(\\d+)]}");

        Player1Move();
        Player2Move();

        //qDebug() <<"Pacman (x,y) on server (" << pac_man->getPac_X() << "," << pac_man->getPac_Y() << ")";
    }



    foodball_items_count = foodball_positions->size();
    powerball_items_count = powerball_positions->size();




    for(int i=0;i<foodball_items_count;i++)
    {
        if(pac_x==foodball_positions->at(i).x() && pac_y==foodball_positions->at(i).y())
        {
            qDebug("WYSLANO MESSAGE BO NADEPNIETO NA KULKE");

            messagetowrite = "{del:[" + QByteArray::number(foodball_positions->at(i).x()) + "," + QByteArray::number(foodball_positions->at(i).y()) + "]}";

            foodball_items_count--;
            foodball_positions->remove(i);

            //serversocket2->write(messagetowrite);
 //           "{del:\\[(\\d+,\\d+)\\]}"

//            if(eat_sound1->state()==QMediaPlayer::StoppedState)
//            {
//                eat_sound1->play();
//            }

//            if(eat_sound1->state()==QMediaPlayer::PlayingState)
//            {
//                eat_sound2->play();
//            }

//            score++;
//            score_display->setPlainText("Score: " + QString::number(score));

            qDebug() << "Na serwerze pozostalo " << foodball_positions->size() << "foodballi";
        }
    }

    for(int i=0;i<powerball_items_count;i++)
    {
        if(pac_x==powerball_positions->at(i).x() && pac_y==powerball_positions->at(i).y())
        {
            qDebug("WYSLANO MESSAGE BO NADEPNIETO NA KULKE");

            messagetowrite = "{del:[" + QByteArray::number(powerball_positions->at(i).x()) + "," + QByteArray::number(powerball_positions->at(i).y()) + "]}";

            powerball_items_count--;
            powerball_positions->remove(i);

            //serversocket2->write(messagetowrite);

            //            score += 100;
//            score_display->setPlainText("Score: " + QString::number(score));

//            scarestate = 0;

//            ghost1->setIsScared(true);
//            ghost2->setIsScared(true);
//            ghost3->setIsScared(true);
//            ghost4->setIsScared(true);

//            scared=true;

//            score_display->setPlainText("Score: " + QString::number(score));

            qDebug() << "Na serwerze pozostalo " << powerball_positions->size() << "powerballi";
        }
    }
}

//SLOTS

int PacmanServer::ReadDirectionFromClient1()
{
    directionreceivedfromclient1 = serversocket1->readAll();
    directionreceivedfromclient1.resize(1);
    qDebug() << "Socket1" << directionreceivedfromclient1;
    pac_man->setNextDirection(directionreceivedfromclient1.toInt());
}

void PacmanServer::connected1()
{
    qDebug() << "Client 1 Connected!";
}

void PacmanServer::disconnected1()
{
    qDebug() << "Client 1 Disconnected!";
}

void PacmanServer::bytesWritten1 (qint64 bytes)
{
    //qDebug() << "Server wrote: " << bytes;
}

int PacmanServer::ReadDirectionFromClient2()
{
    directionreceivedfromclient2 = serversocket2->readAll();
    directionreceivedfromclient2.resize(1);
    qDebug() << "Socket2" << directionreceivedfromclient2;
    ghostplayer->setNextGhostDirection(directionreceivedfromclient2.toInt());
}

void PacmanServer::connected2()
{
    qDebug() << "Client 2 Connected!";
}

void PacmanServer::disconnected2()
{
    qDebug() << "Client 2 Disconnected!";
}

void PacmanServer::bytesWritten2(qint64 bytes)
{
    //qDebug() << "Server wrote: " << bytes;
}
