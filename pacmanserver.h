#ifndef PACMANSERVER_H
#define PACMANSERVER_H

#include <QObject>
#include <QtCore>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>

#include "map.h"
#include "pacman.h"
#include "ghost.h"
#include "foodball.h"
#include "powerball.h"

class PacmanServer : public QObject
{
    Q_OBJECT

private:
    QTcpServer *server;
    QTcpSocket *serversocket1;
    QTcpSocket *serversocket2;

    QTimer *updatertimer;
    QTimer *sendcoordinates_player1_timer;
    QTimer *sendcoordinates_player2_timer;

    Pacman *pac_man;
    Map *pac_map;
    Ghost *ghostplayer;
    FoodBall *food_ball;
    PowerBall *power_ball;

    QByteArray info_package_for_clients_packed;
    QByteArray messagetowrite;

    QByteArray directionreceivedfromclient1;
    QByteArray directionreceivedfromclient2;

    QByteArray is_ghost_scared_white_packed;

    int player1points;

    QVector<QPoint> *powerball_positions;
    QVector<QPoint> *foodball_positions;

    int foodball_items_count;
    int powerball_items_count;

public:
    explicit PacmanServer(QObject *parent = nullptr);

    void StartListening();
    void Player1Move();
    void Player2Move();

signals:

public slots:
    void AcceptConnection();

    void SendcoordinatesToClient1();
    void SendcoordinatesToClient2();
    void updater();

    int ReadDirectionFromClient1();
    int ReadDirectionFromClient2();
    void connected1();
    void disconnected1();
    void bytesWritten1(qint64 bytes);
    void connected2();
    void disconnected2();
    void bytesWritten2(qint64 bytes);

};

#endif // PACMANSERVER_H
