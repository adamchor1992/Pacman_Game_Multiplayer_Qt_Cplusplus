#include "clientconnection.h"

ClientConnection::ClientConnection(QObject *parent) : QObject(parent)
{
    clientsocket = new QTcpSocket(this);
    //serveraddress = "192.168.43.152";
    serveraddress = "192.168.43.235"; // telefon
    //serveraddress = "192.168.43.96";
    //serveraddress = "10.2.122.95"; // wi-free
    serverport = 5000;

    show_message_from_server_timer = new QTimer(this);

    //connect slots
    connect(clientsocket,SIGNAL(connected()), this, SLOT(connected()));
    connect(clientsocket,SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(clientsocket, SIGNAL(readyRead()), this, SLOT(ShowMessageFromServer()));
    connect(clientsocket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));

    //connect(clientsocket,SIGNAL(readyRead()), this, SLOT(ReadCoordinatesFromServer()));
}

void ClientConnection::RequestConnection()
{
    //clientsocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    clientsocket->connectToHost(serveraddress, serverport);

    if(!clientsocket->waitForConnected(500))
    {
        qDebug() << "Failed to connect from client side: " << clientsocket->errorString();
        qDebug() << "Application will terminate";
        //exit(1);
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

    case '5':
        clientsocket->putChar('5');
        break;

    case '6':
        clientsocket->putChar('6');
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

void ClientConnection::ShowMessageFromServer()
{
    message_from_server = clientsocket->readAll();
    qDebug() << message_from_server;
    if(message_from_server == "Game started")
    {
        disconnect(clientsocket, SIGNAL(readyRead()), this, SLOT(ShowMessageFromServer()));
        connect(clientsocket,SIGNAL(readyRead()), this, SLOT(ReadCoordinatesFromServer()));
    }
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




