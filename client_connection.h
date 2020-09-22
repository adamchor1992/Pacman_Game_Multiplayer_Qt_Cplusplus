#pragma once

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QTimer>
#include <QStatusBar>

class ClientConnection : public QObject
{
    Q_OBJECT

public:
    static const int GAME_PORT = 5000;

    explicit ClientConnection(QStatusBar* statusbar, QObject* parent = nullptr);
    void RequestConnection(QHostAddress address, uint port);
    void SendPressedKeyToServer(char key);
    QByteArray GetCoordinates() {return m_Coordinates;}

private:
    QTcpSocket* m_pClientSocket;
    QStatusBar* m_pStatusBar;

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
