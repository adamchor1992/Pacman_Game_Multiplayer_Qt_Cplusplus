#include "server_connection.h"
#include <status_bar_manager.h>
#include "../common/log_manager.h"
#include <QDebug>

ServerConnection::ServerConnection()
{
    connect(&m_ServerConnectionSocket, &QTcpSocket::connected, this, &ServerConnection::Connected, Qt::UniqueConnection);
    connect(&m_ServerConnectionSocket, &QTcpSocket::disconnected, this, &ServerConnection::Disconnected, Qt::UniqueConnection);
    connect(&m_ServerConnectionSocket, &QTcpSocket::readyRead, this, &ServerConnection::NewDataFromServerAvailable, Qt::UniqueConnection);

    LogManager::LogToFile("Connected ServerConnection signals and slots");
}

void ServerConnection::ConnectToServer(QHostAddress address, uint port)
{
    m_ServerConnectionSocket.connectToHost(address, port);

    if(!m_ServerConnectionSocket.waitForConnected(CONNECTION_TIMEOUT))
    {
        StatusBarManager::ShowMessage("Connection failed, please restart client");

        LogManager::LogToFile("Failed to connect to server: " + m_ServerConnectionSocket.errorString().toStdString());

        qDebug() << "Failed to connect from client side: " << m_ServerConnectionSocket.errorString();
    }
}

void ServerConnection::SendPressedKeyToServer(char controlKey)
{
    m_ServerConnectionSocket.putChar(controlKey);
}

QByteArray ServerConnection::ReadDataFromServer()
{
    //LogManager::LogToFile("ReadDataFromServer");

    return m_ServerConnectionSocket.readLine();
}

void ServerConnection::Connected()
{
    StatusBarManager::ShowMessage("Successfully connected from client side", StatusBarManager::MESSAGE_TIMEOUT);
    qDebug() << "Successfully connected from client side";
}

void ServerConnection::Disconnected()
{
    StatusBarManager::ShowMessage("YOU ARE DISCONNECTED");
    qDebug() << "Disconnected!";
}
