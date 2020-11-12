#pragma once

#include <QTcpSocket>
#include <QHostAddress>

class ServerConnection : public QObject
{
    Q_OBJECT

public:
    static const int GAME_PORT = 5000;

    explicit ServerConnection();
    void ConnectToServer(QHostAddress address, uint port);
    void SendPressedKeyToServer(char controlKey);
    QByteArray ReadDataFromServer();

private:
    const int CONNECTION_TIMEOUT = 500;

    QTcpSocket m_ServerConnectionSocket;

signals:
    void NewDataFromServerAvailable();

private slots:
    void Connected();
    void Disconnected();
};
