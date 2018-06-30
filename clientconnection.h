#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QTimer>
#include <QStatusBar>

class ClientConnection : public QObject
{
    Q_OBJECT

private:
    QTcpSocket *clientsocket;
    QStatusBar *statusbar;
    QHostAddress serveraddress;
    int serverport;

    QByteArray coordinates;
    QByteArray message_from_server;

    int *game_state; //0-not started yet, 1-started and running, 2-paused, 3-aborted, to be restarted, 4-pacman_wins, 5-ghost_wins

public:
    explicit ClientConnection(QStatusBar *_statusbar, int *_game_state, QObject *parent = nullptr);

    void RequestConnection();
    void SendPressedKeyToServer(char key);
    QByteArray getCoordinates() {return coordinates;}

signals:
    void GameStarted_signal();

public slots:
    void connected();
    void disconnected();
    void ReadCoordinatesFromServer();
    void ShowMessageFromServer();
};

#endif // CLIENTCONNECTION_H
