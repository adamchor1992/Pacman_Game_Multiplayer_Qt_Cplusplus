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
    QTcpServer server;
    QTcpSocket *serversocket1;
    QTcpSocket *serversocket2;
    QTcpSocket tempsocket;

    //connections signals/slots

    QMetaObject::Connection con_newconnection;
    QMetaObject::Connection con_waitforplayerconnection_timer;
    QMetaObject::Connection con_waitforplayerreadysignal_timer;
    QMetaObject::Connection con_updater_timer;
    QMetaObject::Connection con_packdatatosendtoclients;
    QMetaObject::Connection con_sendcoordinates_player1_timer;
    QMetaObject::Connection con_sendcoordinates_player2_timer;
    QMetaObject::Connection con_socket1connected;
    QMetaObject::Connection con_socket1disconnected;
    QMetaObject::Connection con_socket1readyread;
    QMetaObject::Connection con_socket2connected;
    QMetaObject::Connection con_socket2disconnected;
    QMetaObject::Connection con_socket2readyread;

    //timers
    QTimer wait_for_player_connection_timer;
    QTimer wait_for_player_ready_signal_timer;
    QTimer updatertimer;
    QTimer sendcoordinates_player1_timer;
    QTimer sendcoordinates_player2_timer;

    Pacman pac_man;
    Map pac_map;
    Ghost ghostplayer;
    FoodBall food_ball;
    PowerBall power_ball;

    QByteArray info_package_for_clients_packed;
    QByteArray messagetowrite;
    QByteArray directionreceivedfromclient1;
    QByteArray directionreceivedfromclient2;
    QByteArray game_state_packed;
    QByteArray is_ghost_scared_white_packed;

    int player1points;

    QVector<QPoint> powerball_positions;
    QVector<QPoint> foodball_positions;

    int foodball_items_count;
    int powerball_items_count;

    int game_state; //0-not started yet, 1-started and running, 2-paused, 3-to be restarted, 4-pacman wins, 5-ghost wins
    bool player1_ready, player2_ready;

public:
    explicit PacmanServer(QObject *parent = nullptr);

    void ServerStartListening();
    void Player1Move();
    void Player2Move();

    void SetUpAndFillMap();
    
    void SetUpAndPlacePlayers();
    void StartGame();
    void PauseGame();
    void ResumeGame();
    void PrepareRestart();

    void CheckCollision();
    void DisconnectAllSignals();
    void ResetContainersAndVariables();
    void StopAllTimers();

public slots:
    void AcceptConnection();
    void WaitForPlayerConnection();
    void WaitForPlayerReadySignals();

    void PackDataToSendToClients();
    void SendcoordinatesToClient1();
    void SendcoordinatesToClient2();
    void updater();

    void ReadDirectionFromClient1();
    void ReadDirectionFromClient2();
    void connected1();
    void disconnected1();
    void connected2();
    void disconnected2();

signals:
    void PrepareNextCoordinatesPackage();
};

#endif // PACMANSERVER_H
