#include "pacmanserver.h"

PacmanServer::PacmanServer(QObject *parent) : QObject(parent)
{
    //setting up server and sockets
    server = new QTcpServer(this);
    serversocket1 = new QTcpSocket(this);
    serversocket2 = new QTcpSocket(this);

    StartListening();

    //wait for new connections
    connect(server,SIGNAL(newConnection()), this, SLOT(AcceptConnection()));

    //create game environment
    SetUpAndFillMap();
    SetUpAndPlacePlayers();

    game_state = 0;

    //set up timer waiting for players signals
    wait_for_player_connection_timer = new QTimer(this);
    wait_for_player_ready_signal_timer = new QTimer(this);

    updatertimer = new QTimer(this);
    sendcoordinates_player1_timer = new QTimer(this);
    sendcoordinates_player2_timer = new QTimer(this);
}

void PacmanServer::StartListening()
{
    if(server->listen(QHostAddress::Any, 5000))
    {
        qDebug() << "Server started, awaiting connections";
    }
    else
    {
        qDebug() << "Server could not start: " << server->errorString();
        qDebug() << "Please restart application";
    }
}

void PacmanServer::SetUpAndFillMap()
{
    pac_map = new Map;
    food_ball = new FoodBall;
    power_ball = new PowerBall;

    powerball_positions = new QVector<QPoint>;
    foodball_positions = new QVector<QPoint>;

    *powerball_positions = power_ball->getPowerBallPositions();
    *foodball_positions = food_ball->getFoodBallPositions();

    foodball_items_count = foodball_positions->size();
    powerball_items_count = powerball_positions->size();

    //qDebug("Foodball positions size on server: %d", foodball_positions->size());
}

void PacmanServer::SetUpAndPlacePlayers()
{
    pac_man = new Pacman;
    ghostplayer = new Ghost;

    pac_man->setPac_X(320);
    pac_man->setPac_Y(514);
    pac_man->setDirection(1); //pacman moves left after game start

    ghostplayer->setGhost_X(307);
    ghostplayer->setGhost_Y(252);
    ghostplayer->setGhostDirection(1);

    player1points=0;

    player1_ready = false;
    player2_ready = false;
}

void PacmanServer::StartGame()
{
    connect(updatertimer, SIGNAL(timeout()), this, SLOT(updater()));
    connect(sendcoordinates_player1_timer, SIGNAL(timeout()), this, SLOT(SendcoordinatesToClient1()));
    connect(sendcoordinates_player2_timer, SIGNAL(timeout()), this, SLOT(SendcoordinatesToClient2()));

    updatertimer->start(4);
    sendcoordinates_player1_timer->start(5);
    sendcoordinates_player2_timer->start(5);

    qDebug() << "Game started";

    game_state = 1;
}

void PacmanServer::PauseGame()
{
    game_state = 2;
    //updatertimer->stop();
}

void PacmanServer::ResumeGame()
{
    game_state = 1;
    //updatertimer->start(5);
}

//SLOT

void PacmanServer::AcceptConnection()
{
    //if there is new connection
    if(serversocket1->state() == QAbstractSocket::UnconnectedState)
    {
        serversocket1 = server->nextPendingConnection();

        connect(serversocket1, SIGNAL(connected()), this, SLOT(connected1()));
        connect(serversocket1, SIGNAL(disconnected()), this, SLOT(disconnected1()));
        connect(serversocket1, SIGNAL(readyRead()), this, SLOT(ReadDirectionFromClient1()));
        connect(serversocket1, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten1(qint64)));

        connect(wait_for_player_connection_timer, SIGNAL(timeout()), this, SLOT(WaitForPlayerConnection()));
        wait_for_player_connection_timer->start(3000);

        qDebug() << "Connection assigned to socket 1 - PACMAN";
        serversocket1->write("Connection established. You are PLAYER 1 - PACMAN");
    }
    else
    {
        serversocket2 = server->nextPendingConnection();

        connect(serversocket2, SIGNAL(connected()), this, SLOT(connected2()));
        connect(serversocket2, SIGNAL(disconnected()), this, SLOT(disconnected2()));
        connect(serversocket2, SIGNAL(readyRead()), this, SLOT(ReadDirectionFromClient2()));
        connect(serversocket2, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten2(qint64)));

        wait_for_player_connection_timer->stop();
        disconnect(wait_for_player_connection_timer, SIGNAL(timeout()), this, SLOT(WaitForPlayerConnection())); //disconnect unneccesary signal

        connect(wait_for_player_ready_signal_timer, SIGNAL(timeout()), this, SLOT(WaitForPlayerReadySignals()));
        wait_for_player_ready_signal_timer->start(2000);

        qDebug() << "Connection assigned to socket 2 - GHOST";
        serversocket2->write("You are PLAYER 2 - GHOST");
    }
}

void PacmanServer::WaitForPlayerConnection()
{
    qDebug() << "Waiting for player 2";
    serversocket1->write("You are connected, waiting for another player");
}

void PacmanServer::WaitForPlayerReadySignals()
{
    if(!player1_ready && !player2_ready)
    {
        qDebug("Waiting for players ready signals");
        serversocket1->write("You are not ready, please press space");
        serversocket2->write("You are not ready, please press space");
    }

    if(player1_ready && !player2_ready)
    {
        qDebug("Player 1 - ready Player 2 - NOT READY");
        serversocket1->write("You are ready, awaiting other player connection");
        serversocket2->write("You are not ready, please press space");
    }

    if(!player1_ready && player2_ready)
    {
        qDebug("Player 2 - NOT READY Player 1 - ready");
        serversocket1->write("You are not ready, please press space");
        serversocket2->write("You are ready, awaiting other player connection");
    }

    if(player1_ready && player2_ready)
    {
        qDebug("Player 1 and Player 2 ready");

        StartGame();
        serversocket1->write("Game started");
        serversocket2->write("Game started");

        wait_for_player_ready_signal_timer->stop();
        disconnect(wait_for_player_ready_signal_timer, SIGNAL(timeout()), this, SLOT(WaitForPlayerReadySignals()));

        //delete wait_for_player_signal_timer;
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

void PacmanServer::PackDataToSendToClients()
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

        if(game_state == 0) //game not started
        {
            game_state_packed = "{[S:0],";
        }
        else if(game_state == 1) //game running
        {
            game_state_packed = "{[S:1],";
        }
        else if(game_state == 2) //game paused
        {
            game_state_packed = "{[S:2],";
        }
        else if(game_state == 3) //game aborted, to be restarted
        {
            game_state_packed = "{[S:3],";
        }
        else if(game_state == 4) //pacman wins
        {
            game_state_packed = "{[S:4],";
            updatertimer->stop();
        }
        else if(game_state == 5) //ghost wins
        {
            game_state_packed = "{[S:5],";
            updatertimer->stop();
        }

        if(ghostplayer->getIsScared()) //scared
        {
            if(!ghostplayer->getScaredWhite()) //scared blue
            {
                is_ghost_scared_white_packed = "[G:S],";
            }
            else //scared white
            {
                is_ghost_scared_white_packed = "[G:W],";
            }
        }

        else //no scared
        {
            is_ghost_scared_white_packed = "[G:N],";
        }

        QByteArray points_packed = "[P:" + QByteArray::number(player1points) + "]},";

        //QByteArray message = "hello";
        QByteArray message_packed = "{'" + messagetowrite + "'}}";

        info_package_for_clients_packed = player1_x_coordinate_packed + player1_y_coordinate_packed + player2_x_coordinate_packed + player2_y_coordinate_packed +
                game_state_packed + is_ghost_scared_white_packed + points_packed + message_packed;

    //qDebug() << "info_package_for_clients_packed" << info_package_for_clients_packed;

        //qDebug() << "Package przed przeslaniem na serwerze: " << info_package_for_clients_packed;

        //qDebug() <<"Pacman (x,y) on server (" << pac_man->getPac_X() << "," << pac_man->getPac_Y() << ")";
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

void PacmanServer::CheckCollision()
{
    if((pac_man->getPac_X() == ghostplayer->getGhost_X()) && (pac_man->getPac_Y() == ghostplayer->getGhost_Y()))
    {
        if(ghostplayer->getIsScared() == true)
        {
            //sounds->eat_ghost_sound->play();
            //score+=200;
            //score_display->setPlainText("Score: " + QString::number(score));

            game_state = 4;
            qDebug() << "Pacman wins";
//            ghostplayer->setGhost_X(307);
//            ghostplayer->setGhost_Y(252);
//            ghostplayer->setIsScared(false);
        }
        else
        {
            //GHOST WINS
            game_state = 5;
            qDebug() << "Ghost wins";
            //            timer->stop();
            //            ghoststimer->stop();
            //            EndGame(0);
        }
    }
}

void PacmanServer::updater()
{
    //qDebug() << "Game state on server: " << game_state << "Game state packed on server: " << game_state_packed;

    int pac_x = pac_man->getPac_X();
    int pac_y = pac_man->getPac_Y();

    foodball_items_count = foodball_positions->size();
    powerball_items_count = powerball_positions->size();

    for(int i=0;i<foodball_items_count;i++)
    {
        if(pac_x==foodball_positions->at(i).x() && pac_y==foodball_positions->at(i).y())
        {
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
            messagetowrite = "{del:[" + QByteArray::number(powerball_positions->at(i).x()) + "," + QByteArray::number(powerball_positions->at(i).y()) + "]}";

            powerball_items_count--;
            powerball_positions->remove(i);

            //serversocket2->write(messagetowrite);

            //            score += 100;
//            score_display->setPlainText("Score: " + QString::number(score));

//            scarestate = 0;

            ghostplayer->setIsScared(true);

//            scared=true;

//            score_display->setPlainText("Score: " + QString::number(score));

            qDebug() << "Na serwerze pozostalo " << powerball_positions->size() << "powerballi";
        }
    }

    if(foodball_items_count==0)
    {
        game_state = 4;
        //PACMAN WINS
        qDebug() << "Pacman wins";
        //        timer->stop();
        //        ghoststimer->stop();
        //        EndGame(1);
    }

    if(ghostplayer->getIsScared())
    {
        ghostplayer->incrementScarestate();

        if(ghostplayer->getScarestate() == 1)
        {
            //ghoststimer->setInterval(50);
        }

        if(ghostplayer->getScarestate() == 750)
        {
            ghostplayer->setScaredWhite(true);
        }

        if(ghostplayer->getScarestate() == 1000)
        {
            ghostplayer->setIsScared(false);
            ghostplayer->setScaredWhite(false);

            ghostplayer->setScarestate(0);
            //ghoststimer->setInterval(4);
        }
    }

    qDebug() << "Game state on server: " << game_state;

    Player1Move();
    Player2Move();

    CheckCollision();

    if(serversocket1->state() == QAbstractSocket::ConnectedState && serversocket2->state() == QAbstractSocket::ConnectedState)
    {
        PackDataToSendToClients();
    }
}

//SLOTS
//socket1
void PacmanServer::ReadDirectionFromClient1()
{
    directionreceivedfromclient1 = serversocket1->readAll();
    directionreceivedfromclient1.resize(1);

    //qDebug() << "Socket1" << directionreceivedfromclient1;

    if(game_state==0 && directionreceivedfromclient1.toInt() == 5) // 5 is ready signal
    {
        player1_ready = true;
        return;
    }

    if(game_state==1 && directionreceivedfromclient1.toInt() == 6) // 6 is pause signal
    {
        PauseGame();
        return;
    }

    if(game_state==2 && directionreceivedfromclient1.toInt() == 6) // 6 is pause signal
    {
        ResumeGame();
        return;
    }

    pac_man->setNextDirection(directionreceivedfromclient1.toInt());
}

void PacmanServer::connected1()
{
    qDebug() << "Client 1 Connected!";
}

void PacmanServer::disconnected1()
{
    disconnect(serversocket1, SIGNAL(connected()), this, SLOT(connected1()));
    disconnect(serversocket1, SIGNAL(disconnected()), this, SLOT(disconnected1()));
    disconnect(serversocket1, SIGNAL(readyRead()), this, SLOT(ReadDirectionFromClient1()));
    disconnect(serversocket1, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten1(qint64)));

    disconnect(wait_for_player_connection_timer, SIGNAL(timeout()), this, SLOT(WaitForPlayerConnection()));
    wait_for_player_connection_timer->stop();
    qDebug() << "Client 1 Disconnected!";
}

void PacmanServer::bytesWritten1 (qint64 bytes)
{
    //qDebug() << "Server wrote: " << bytes;
}

//socket2
void PacmanServer::ReadDirectionFromClient2()
{
    directionreceivedfromclient2 = serversocket2->readAll();
    directionreceivedfromclient2.resize(1);
    qDebug() << "Socket2" << directionreceivedfromclient2;

    if(game_state==0 && directionreceivedfromclient2.toInt() == 5) // 5 is ready signal
    {
        player2_ready = true;
        return;
    }

    if(game_state==1 && directionreceivedfromclient2.toInt() == 6) // 6 is pause signal
    {
        PauseGame();
        return;
    }

    if(game_state==2 && directionreceivedfromclient2.toInt() == 6) // 6 is pause signal
    {
        ResumeGame();
        return;
    }

    ghostplayer->setNextGhostDirection(directionreceivedfromclient2.toInt());
}

void PacmanServer::connected2()
{
    qDebug() << "Client 2 Connected!";
}

void PacmanServer::disconnected2()
{
//    disconnect(serversocket2, SIGNAL(connected()), this, SLOT(connected2()));
//    disconnect(serversocket2, SIGNAL(disconnected()), this, SLOT(disconnected2()));
//    disconnect(serversocket2, SIGNAL(readyRead()), this, SLOT(ReadDirectionFromClient2()));
//    disconnect(serversocket2, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten2(qint64)));

//    disconnect(wait_for_player_connection_timer, SIGNAL(timeout()), this, SLOT(WaitForPlayerConnection()));
//    wait_for_player_connection_timer->stop();
    qDebug() << "Client 2 Disconnected!";
}

void PacmanServer::bytesWritten2(qint64 bytes)
{
    //qDebug() << "Server wrote: " << bytes;
}
