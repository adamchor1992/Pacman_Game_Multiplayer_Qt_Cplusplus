#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

class ClientConnection : public QObject
{
    Q_OBJECT

private:
    QTcpSocket *clientsocket;
    QHostAddress serveraddress;
    int serverport;

    QByteArray coordinates;

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
};

#endif // CLIENTCONNECTION_H
