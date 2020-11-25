#include "server_connection.h"
#include <status_bar_manager.h>
#include "../common/log_manager.h"
#include <QDebug>

ServerConnection::ServerConnection()
{
    connect(&m_ServerConnectionSocket, &QTcpSocket::connected, this, &ServerConnection::Connected, Qt::UniqueConnection);
    connect(&m_ServerConnectionSocket, &QTcpSocket::disconnected, this, &ServerConnection::Disconnected, Qt::UniqueConnection);
    connect(&m_ServerConnectionSocket, &QTcpSocket::readyRead, this, &ServerConnection::ReadDataFromServer, Qt::UniqueConnection);

    LogManager::LogToFile("Connected ServerConnection signals and slots");

    lastCall = std::chrono::steady_clock::now();
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

QByteArray ServerConnection::GetNextPacketFromServer()
{
    if(m_DataFromServer.size() > 0)
    {
        LogManager::LogToFile("Dequeueing data");
        return m_DataFromServer.dequeue();
    }
    else
    {
        LogManager::LogToFile("Returning empty byte array");
        return QByteArray("");
    }
}

void ServerConnection::ReadDataFromServer()
{
    if(m_ServerConnectionSocket.canReadLine())
    {
        m_DataFromServer.enqueue(m_ServerConnectionSocket.readLine());
    }
    else
    {
        LogManager::LogToFile("===============THERE WAS DATA BUT IT WAS NOT WHOLE LINE===============");
    }

    LogManager::LogToFile("Current queue size is: " + std::to_string(m_DataFromServer.size()));
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

void ServerConnection::ReadyReadInvoked()
{
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsedSeconds = std::chrono::duration<double>(end - lastCall);

    LogManager::LogToFile("Time since last readyRead: " + std::to_string(elapsedSeconds.count()));

    lastCall = end;
}
