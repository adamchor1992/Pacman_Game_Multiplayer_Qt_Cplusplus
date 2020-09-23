#pragma once

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

public:
    explicit PacmanServer(QObject* parent = nullptr);

private:
    QTcpServer m_Server;
    QTcpSocket* m_ServerSocket1;
    QTcpSocket* m_ServerSocket2;
    QTcpSocket m_TempSocket;

    QMetaObject::Connection m_ConnectionObject_NewConnection;
    QMetaObject::Connection m_ConnectionObject_WaitForPlayerConnectionTimer;
    QMetaObject::Connection m_ConnectionObject_WaitForPlayerReadySignalTimer;
    QMetaObject::Connection m_ConnectionObject_UpdaterTimer;
    QMetaObject::Connection m_ConnectionObject_PackDataToSendToClients;
    QMetaObject::Connection m_ConnectionObject_SendCoordinatesPlayer1Timer;
    QMetaObject::Connection m_ConnectionObject_SendCoordinatesPlayer2Timer;
    QMetaObject::Connection m_ConnectionObject_Socket1Connected;
    QMetaObject::Connection m_ConnectionObject_Socket1Disconnected;
    QMetaObject::Connection m_ConnectionObject_Socket1ReadyRead;
    QMetaObject::Connection m_ConnectionObject_Socket2Connected;
    QMetaObject::Connection m_ConnectionObject_Socket2Disconnected;
    QMetaObject::Connection m_ConnectionObject_Socket2ReadyRead;

    QTimer m_WaitForPlayerConnectionTimer;
    QTimer m_WaitForPlayerReadySignalTimer;
    QTimer m_UpdaterTimer;
    QTimer m_SendCoordinatesPlayer1Timer;
    QTimer m_SendCoordinatesPlayer2Timer;

    Pacman m_Pacman;
    Map m_Map;
    Ghost m_GhostPlayer;
    Foodball m_Foodball;
    Powerball m_Powerball;

    QByteArray m_InfoPackageForClientsPacked;
    QByteArray m_MessageToWrite;
    QByteArray m_DirectionReceivedFromClient1;
    QByteArray m_DirectionReceivedFromClient2;
    QByteArray m_GameStatePacked;
    QByteArray m_IsGhostScaredWhitePacked;

    int m_Player1Score;

    QVector<QPoint> m_PowerballPositions;
    QVector<QPoint> m_FoodballPositions;

    int m_FoodballItemsCount;
    int m_PowerballItemsCount;

    int m_GameState; //0-not started yet, 1-started and running, 2-paused, 3-to be restarted, 4-pacman wins, 5-ghost wins
    bool m_Player1Ready, m_Player2Ready;

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

private slots:
    void AcceptConnection();
    void WaitForPlayerConnection();
    void WaitForPlayerReadySignals();

    void PackDataToSendToClients();
    void SendcoordinatesToClient1();
    void SendcoordinatesToClient2();
    void Updater();
    void ReadDirectionFromClient1();
    void ReadDirectionFromClient2();
    void connected1();
    void disconnected1();
    void connected2();
    void disconnected2();

signals:
    void PrepareNextCoordinatesPackage();
};
