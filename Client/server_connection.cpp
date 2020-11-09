#include "server_connection.h"
#include <status_bar_manager.h>

ServerConnection::ServerConnection() : QObject()
{
    m_pClientSocket = new QTcpSocket(this);

    //connect slots
    connect(m_pClientSocket,SIGNAL(connected()), this, SLOT(connected()), Qt::UniqueConnection);
    connect(m_pClientSocket,SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::UniqueConnection);
    connect(m_pClientSocket, SIGNAL(readyRead()), this, SLOT(ShowMessageFromServer()), Qt::UniqueConnection);
}

void ServerConnection::ConnectToServer(QHostAddress address, uint port)
{
    m_pClientSocket->connectToHost(address, port);

    if(!m_pClientSocket->waitForConnected(CONNECTION_TIMEOUT))
    {
        StatusBarManager::ShowMessage("Connection failed, please restart client");

        qDebug() << "Failed to connect from client side: " << m_pClientSocket->errorString();
    }
}

void ServerConnection::SendPressedKeyToServer(char controlKey)
{
    m_pClientSocket->putChar(controlKey);
}

//SLOTS

void ServerConnection::connected()
{
    StatusBarManager::ShowMessage("Successfully connected from client side", StatusBarManager::MESSAGE_TIMEOUT);
    qDebug() << "Successfully connected from client side";
}

void ServerConnection::ShowMessageFromServer()
{
    m_MessageFromServer = m_pClientSocket->readAll();

    StatusBarManager::ShowMessage(m_MessageFromServer);

    if(m_MessageFromServer == "Game started")
    {
        StatusBarManager::ClearMessage();
        StatusBarManager::ShowMessage("Game started", StatusBarManager::MESSAGE_TIMEOUT);
        disconnect(m_pClientSocket, SIGNAL(readyRead()), this, SLOT(ShowMessageFromServer()));
        //start game
        emit GameStarted();
        connect(m_pClientSocket, SIGNAL(readyRead()), this, SLOT(ReadDataFromServer()), Qt::UniqueConnection);
    }
}

void ServerConnection::ReadDataFromServer()
{
    m_Coordinates = m_pClientSocket->readAll();
}

void ServerConnection::disconnected()
{
    StatusBarManager::ShowMessage("YOU ARE DISCONNECTED");
    qDebug() << "Disconnected!";
}
