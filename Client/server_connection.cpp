#include "server_connection.h"
#include <status_bar_manager.h>
#include "../common/log_manager.h"
#include <QNetworkDatagram>
#include <QDebug>

ServerConnection::ServerConnection()
{
    connect(&m_ServerConnectionTcpSocket, &QTcpSocket::connected, this, &ServerConnection::Connected, Qt::UniqueConnection);
    connect(&m_ServerConnectionTcpSocket, &QTcpSocket::disconnected, this, &ServerConnection::Disconnected, Qt::UniqueConnection);
    connect(&m_ServerConnectionTcpSocket, &QTcpSocket::readyRead, this, &ServerConnection::ReadTcpDataFromServer, Qt::UniqueConnection);

    connect(&m_ServerConnectionUdpSocket, &QUdpSocket::readyRead, this, &ServerConnection::ReadUdpDataFromServer, Qt::UniqueConnection);

    LogManager::LogToFile("Connected ServerConnection signals and slots");

    lastCall = std::chrono::steady_clock::now();
}

void ServerConnection::ConnectToServer(QHostAddress address, uint port)
{
    m_ServerConnectionTcpSocket.connectToHost(address, port);
    m_ServerConnectionUdpSocket.bind(address, port);

    if(!m_ServerConnectionTcpSocket.waitForConnected(CONNECTION_TIMEOUT))
    {
        StatusBarManager::ShowMessage("Connection failed, please restart client");

        LogManager::LogToFile("Failed to connect to server: " + m_ServerConnectionTcpSocket.errorString().toStdString());

        qDebug() << "Failed to connect from client side: " << m_ServerConnectionTcpSocket.errorString();
    }
}

void ServerConnection::SendPressedKeyToServer(char controlKey)
{
    m_ServerConnectionTcpSocket.putChar(controlKey);
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

void ServerConnection::ReadTcpDataFromServer()
{
    if(m_ServerConnectionTcpSocket.canReadLine())
    {
        m_DataFromServer.enqueue(m_ServerConnectionTcpSocket.readLine());
    }
    else
    {
        LogManager::LogToFile("===============THERE WAS DATA ON TCP SOCKET BUT IT WAS NOT WHOLE LINE===============");
    }

    LogManager::LogToFile("TCP READ: Current queue size is: " + std::to_string(m_DataFromServer.size()));
}

void ServerConnection::ReadUdpDataFromServer()
{
    if(m_ServerConnectionUdpSocket.hasPendingDatagrams())
    {
        int pendingDatagramSize = m_ServerConnectionUdpSocket.pendingDatagramSize();

        LogManager::LogToFile("===============THERE IS PENDING DATAGRAM OF SIZE: " + std::to_string(pendingDatagramSize));

        QByteArray receivedDatagramData = m_ServerConnectionUdpSocket.receiveDatagram(pendingDatagramSize).data();

        LogManager::LogToFile("Received datagram data is: " + receivedDatagramData.toStdString());

        m_DataFromServer.enqueue(receivedDatagramData);
    }
    else
    {
        LogManager::LogToFile("===============NO PENDING DATAGRAMS ON SOCKET===============");
    }

    LogManager::LogToFile("UDP READ: Current queue size is: " + std::to_string(m_DataFromServer.size()));
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
