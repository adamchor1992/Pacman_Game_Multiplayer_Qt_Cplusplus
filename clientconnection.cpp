#include "clientconnection.h"

ClientConnection::ClientConnection(QObject *parent) : QObject(parent)
{
    clientsocket = new QTcpSocket(this);
    //serveraddress = "192.168.43.96";
    serveraddress = "192.168.43.167";
    //serveraddress = "192.168.0.192";
    serverport = 23;
        
    connect(clientsocket,SIGNAL(connected()), this, SLOT(connected()));
    connect(clientsocket,SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(clientsocket,SIGNAL(readyRead()), this, SLOT(ReadCoordinatesFromServer()));
    connect(clientsocket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));
}

void ClientConnection::RequestConnection()
{
    clientsocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    clientsocket->connectToHost(serveraddress, serverport);

    if(!clientsocket->waitForConnected(500))
    {
        qDebug() << "Failed to connect from client side";
    }
}

void ClientConnection::SendPressedKeyToServer(char key)
{
    switch(key)
    {
    case 'a':
        clientsocket->putChar('1');
        break;

    case 'w':
        clientsocket->putChar('2');
        break;

    case 's':
        clientsocket->putChar('3');
        break;

    case 'd':
        clientsocket->putChar('4');
        break;

    default:
        qDebug("Unrecognized or wrong key");
        break;
    }
}

//SLOTS

void ClientConnection::connected()
{
    qDebug() << "Successfully connected from client side";
}

void ClientConnection::ReadCoordinatesFromServer()
{
    coordinates = clientsocket->readAll();
}

void ClientConnection::bytesWritten (qint64 bytes)
{
    qDebug() << "Bytes written: " << bytes;
}

void ClientConnection::disconnected()
{
    qDebug() << "Disconnected!";
}




