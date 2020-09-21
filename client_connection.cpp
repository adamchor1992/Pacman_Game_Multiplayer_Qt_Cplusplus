#include "client_connection.h"

ClientConnection::ClientConnection(QStatusBar* pStatusBar, int* pGameState, QObject *parent) : QObject(parent)
{
    m_pStatusBar = pStatusBar;
    m_pGameState = pGameState; //client connection is now writing game_state directly to Game_window object
    m_pClientSocket = new QTcpSocket(this);

    //connect slots
    connect(m_pClientSocket,SIGNAL(connected()), this, SLOT(connected()), Qt::UniqueConnection);
    connect(m_pClientSocket,SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::UniqueConnection);
    connect(m_pClientSocket, SIGNAL(readyRead()), this, SLOT(ShowMessageFromServer()), Qt::UniqueConnection);
}

void ClientConnection::RequestConnection(QHostAddress address, uint port)
{
    m_pClientSocket->connectToHost(address, port);

    if(!m_pClientSocket->waitForConnected(500))
    {
        m_pStatusBar->showMessage("Connection failed, please restart client");
        qDebug() << "Failed to connect from client side: " << m_pClientSocket->errorString();
    }
}

void ClientConnection::SendPressedKeyToServer(char key)
{
    switch(key)
    {
    case 'a':
        m_pClientSocket->putChar('1');
        break;

    case 'w':
        m_pClientSocket->putChar('2');
        break;

    case 's':
        m_pClientSocket->putChar('3');
        break;

    case 'd':
        m_pClientSocket->putChar('4');
        break;

    case '5':
        m_pClientSocket->putChar('5');
        break;

    case '6':
        m_pClientSocket->putChar('6');
        break;
    case '7':
        m_pClientSocket->putChar('7');
        break;

    default:
        qDebug("Unrecognized or wrong key");
        break;
    }
}

//SLOTS

void ClientConnection::connected()
{
    m_pStatusBar->showMessage("Successfully connected from client side", 3000);
    qDebug() << "Successfully connected from client side";
}

void ClientConnection::ShowMessageFromServer()
{
    m_MessageFromServer = m_pClientSocket->readAll();
    qDebug() << "Message from server: " << m_MessageFromServer;
    m_pStatusBar->showMessage(m_MessageFromServer);

    if(m_MessageFromServer == "Game started")
    {
        m_pStatusBar->clearMessage();
        m_pStatusBar->showMessage("Game started", 3000);
        disconnect(m_pClientSocket, SIGNAL(readyRead()), this, SLOT(ShowMessageFromServer()));
        //start game
        emit(GameStarted_signal());
        connect(m_pClientSocket, SIGNAL(readyRead()), this, SLOT(ReadCoordinatesFromServer()), Qt::UniqueConnection);
    }
}

void ClientConnection::ReadCoordinatesFromServer()
{
    m_Coordinates = m_pClientSocket->readAll();
}

void ClientConnection::disconnected()
{
    m_pStatusBar->showMessage("YOU ARE DISCONNECTED");
    qDebug() << "Disconnected!";
}




