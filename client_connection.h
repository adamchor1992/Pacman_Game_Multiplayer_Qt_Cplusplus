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

    QByteArray coordinates;
    QByteArray message_from_server;

    int *game_state; //0-not started yet, 1-started and running, 2-paused, 3-aborted, to be restarted, 4-pacman_wins, 5-ghost_wins

public:
    /*! Initialize member variables, set up client socket and connect its signals and slots */
    explicit ClientConnection(QStatusBar *_statusbar, int *_game_state, QObject *parent = nullptr);
    /*! Try to connect with given address and port number */
    void RequestConnection(QHostAddress _address, uint _port);
    /*! Translate pressed key to certain value and send it to server */
    void SendPressedKeyToServer(char key);
    /*! Get coordinates and data received from server in form of QByteArray */
    QByteArray getCoordinates() {return coordinates;}

signals:
    void GameStarted_signal();

private slots:
    void connected();
    void disconnected();
    void ReadCoordinatesFromServer();
    void ShowMessageFromServer();
};

#endif // CLIENTCONNECTION_H
