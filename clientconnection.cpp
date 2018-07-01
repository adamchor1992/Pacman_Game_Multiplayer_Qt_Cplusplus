#include "clientconnection.h"

ClientConnection::ClientConnection(QStatusBar *_statusbar, int *_game_state, QObject *parent) : QObject(parent)
{
    statusbar = _statusbar;
    game_state = _game_state; //client connection is now writing game_state directly to Game_window object
    clientsocket = new QTcpSocket(this);

    //connect slots
    connect(clientsocket,SIGNAL(connected()), this, SLOT(connected()), Qt::UniqueConnection);
    connect(clientsocket,SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::UniqueConnection);
    connect(clientsocket, SIGNAL(readyRead()), this, SLOT(ShowMessageFromServer()), Qt::UniqueConnection);
}

void ClientConnection::RequestConnection(QHostAddress _address, uint _port)
{
    //clientsocket->connectToHost(serveraddress, serverport);
    clientsocket->connectToHost(_address, _port);

    if(!clientsocket->waitForConnected(500))
    {
        statusbar->showMessage("Connection failed, please restart client");
        qDebug() << "Failed to connect from client side: " << clientsocket->errorString();
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
    case '7':
        clientsocket->putChar('7');
        break;

    default:
        qDebug("Unrecognized or wrong key");
        break;
    }
}

//SLOTS

void ClientConnection::connected()
{
    statusbar->showMessage("Successfully connected from client side", 3000);
    qDebug() << "Successfully connected from client side";
}

void ClientConnection::ShowMessageFromServer()
{
    message_from_server = clientsocket->readAll();
    qDebug() << "Message from server: " << message_from_server;
    statusbar->showMessage(message_from_server);

    if(message_from_server == "Game started")
    {
        statusbar->clearMessage();
        statusbar->showMessage("Game started", 3000);
        disconnect(clientsocket, SIGNAL(readyRead()), this, SLOT(ShowMessageFromServer()));
        //start game
        emit(GameStarted_signal());
        connect(clientsocket, SIGNAL(readyRead()), this, SLOT(ReadCoordinatesFromServer()), Qt::UniqueConnection);
    }
}

void ClientConnection::ReadCoordinatesFromServer()
{
    coordinates = clientsocket->readAll();
}

void ClientConnection::disconnected()
{
    statusbar->showMessage("YOU ARE DISCONNECTED");
    qDebug() << "Disconnected!";
}




