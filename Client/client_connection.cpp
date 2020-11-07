#include "client_connection.h"

ClientConnection::ClientConnection(QStatusBar* statusbar, QObject *parent) : QObject(parent)
{
    m_pStatusBar = statusbar;
    m_pClientSocket = new QTcpSocket(this);

    //connect slots
    connect(m_pClientSocket,SIGNAL(connected()), this, SLOT(connected()), Qt::UniqueConnection);
    connect(m_pClientSocket,SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::UniqueConnection);
    connect(m_pClientSocket, SIGNAL(readyRead()), this, SLOT(ShowMessageFromServer()), Qt::UniqueConnection);
}

void ClientConnection::RequestConnection(QHostAddress address, uint port)
{
    m_pClientSocket->connectToHost(address, port);

    if(!m_pClientSocket->waitForConnected(CONNECTION_TIMEOUT))
    {
        m_pStatusBar->showMessage("Connection failed, please restart client");
        qDebug() << "Failed to connect from client side: " << m_pClientSocket->errorString();
    }
}

void ClientConnection::SendPressedKeyToServer(char controlKey)
{
    m_pClientSocket->putChar(controlKey);
}

//SLOTS

void ClientConnection::connected()
{
    m_pStatusBar->showMessage("Successfully connected from client side", MESSAGE_TIMEOUT);
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
        m_pStatusBar->showMessage("Game started", MESSAGE_TIMEOUT);
        disconnect(m_pClientSocket, SIGNAL(readyRead()), this, SLOT(ShowMessageFromServer()));
        //start game
        emit GameStarted();
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




