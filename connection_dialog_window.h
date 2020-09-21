#pragma once

#include <QMainWindow>
#include <QString>
#include <QTcpSocket>
#include <QRegularExpression>
#include <QDebug>
#include <QHostAddress>
#include <QHostInfo>

namespace Ui
{
class ConnectionDialogWindow;
}

class ConnectionDialogWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectionDialogWindow(QWidget *parent = 0);
    QHostAddress GetIpAddress(){return m_ValidatedIpAddress;}

    ~ConnectionDialogWindow();

private slots:
    void on_lineEdit_IP_textChanged(const QString &_entered_IP);
    void on_pushButton_Connect_clicked();

private:
    Ui::ConnectionDialogWindow* ui;
    QTcpSocket m_TestSocket;

    QString m_ValidatedIpAddressString;

    QHostAddress m_ValidatedIpAddress; //final IP address to be transferred to GameWindow class

    bool m_IsValidIpAddress;
};
