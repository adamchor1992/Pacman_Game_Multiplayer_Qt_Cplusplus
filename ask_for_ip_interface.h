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

/*! Class responsible for displaying user interface asking for IP address and port number to connect to */

class AskForIP_Interface : public QMainWindow
{
    Q_OBJECT

public:
    /*! Initialize member variables and disable Connect push button*/
    explicit AskForIP_Interface(QWidget *parent = 0);
    /*! Get IP address in form of QHostAddress */
    QHostAddress getIP(){return ext_IP;}
    /*! Get port number in form of unsigned integer */
    uint getPort(){return ext_Port;}

    ~AskForIP_Interface();

private slots:
    void on_lineEdit_IP_textChanged(const QString &_entered_IP);
    void on_pushButton_Connect_clicked();

private:
    Ui::AskForIP_Interface *ui;
    QTcpSocket testsocket;

    QString validated_IP;
    QString validated_Port;

    QHostAddress ext_IP; //final IP address to be transferred to GameWindow class
    uint ext_Port; //final port number to be transferred to GameWindow class

    bool valid_IP;
};

#endif // AskForIP_Interface_H
