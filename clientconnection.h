#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QTimer>

class ClientConnection : public QObject
{
    Q_OBJECT

private:
    QTcpSocket *clientsocket;
    QHostAddress serveraddress;
    int serverport;

    QByteArray coordinates;
    QByteArray message_from_server;
    QTimer *show_message_from_server_timer;

    int game_state; //0-not started yet, 1-started and running, 2-paused, 3-aborted, to be restarted

public:
    explicit ClientConnection(QObject *parent = nullptr);

    void RequestConnection();
    void SendPressedKeyToServer(char key);
    QByteArray getCoordinates() {return coordinates;}

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten (qint64 bytes);
    void ReadCoordinatesFromServer();
    void ShowMessageFromServer();
};

#endif // CLIENTCONNECTION_H
