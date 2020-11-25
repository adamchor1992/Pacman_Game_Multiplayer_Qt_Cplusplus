#pragma once

#include <QTcpSocket>
#include <QHostAddress>
#include <QQueue>
#include <chrono>

class ServerConnection : public QObject
{
    Q_OBJECT

public:
    static const int GAME_PORT = 5000;

    explicit ServerConnection();
    void ConnectToServer(QHostAddress address, uint port);
    void SendPressedKeyToServer(char controlKey);
    QByteArray GetNextPacketFromServer();

private:
    const int CONNECTION_TIMEOUT = 500;

    QTcpSocket m_ServerConnectionSocket;

    std::chrono::steady_clock::time_point lastCall;

    QQueue<QByteArray> m_DataFromServer;

private slots:
    void Connected();
    void Disconnected();
    void ReadDataFromServer();
    void ReadyReadInvoked();
};
