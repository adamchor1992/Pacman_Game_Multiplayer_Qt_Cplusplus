#ifndef AskForIP_Interface_H
#define AskForIP_Interface_H

#include <QMainWindow>
#include <QString>
#include <QTcpSocket>
#include <QRegularExpression>
#include <QDebug>
#include <QHostAddress>
#include <QHostInfo>

namespace Ui
{
class AskForIP_Interface;
}

class AskForIP_Interface : public QMainWindow
{
    Q_OBJECT

public:
    Ui::AskForIP_Interface *ui;
    explicit AskForIP_Interface(QWidget *parent = 0);
    ~AskForIP_Interface();

    QHostAddress getIP(){return ext_IP;}
    uint getPort(){return ext_Port;}

private slots:

    void on_lineEdit_IP_textChanged(const QString &_entered_IP);
    void on_lineEdit_Port_textChanged(const QString &arg1);
    void on_pushButton_Connect_clicked();

private:

    QTcpSocket testsocket;

    QString validated_IP;
    QString validated_Port;

    QHostAddress ext_IP; //final IP address to be transferred to GameWindow class
    uint ext_Port; //final port number to be transferred to GameWindow class

    bool valid_IP;
    bool valid_Port;
};

#endif // AskForIP_Interface_H
