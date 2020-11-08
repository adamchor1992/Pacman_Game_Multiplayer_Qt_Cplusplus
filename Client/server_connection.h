#pragma once

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QTimer>
#include <QStatusBar>

class ServerConnection : public QObject
{
    Q_OBJECT

public:
    static const int GAME_PORT = 5000;

    explicit ServerConnection();
    void ConnectToServer(QHostAddress address, uint port);
    void SendPressedKeyToServer(char controlKey);
    QByteArray GetCoordinates() {return m_Coordinates;}

private:
    const int CONNECTION_TIMEOUT = 500;

    QTcpSocket* m_pClientSocket;

    QByteArray m_Coordinates;
    QByteArray m_MessageFromServer;

signals:
    void GameStarted();

private slots:
    void connected();
    void disconnected();
    void ReadCoordinatesFromServer();
    void ShowMessageFromServer();
};
