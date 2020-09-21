#include "ask_for_ip_interface.h"
#include "ui_askforipinterface.h"

AskForIP_Interface::AskForIP_Interface(QWidget *parent) : QMainWindow(parent), ui(new Ui::AskForIP_Interface)
{
    ui->setupUi(this);

    valid_IP = false;
    ui->pushButton_Connect->setEnabled(false);
}

void AskForIP_Interface::on_lineEdit_IP_textChanged(const QString &entered_IP)
{
    QRegularExpression IP_validation_pattern("^(\\d){1,3}\\.(\\d){1,3}\\.(\\d){1,3}\\.(\\d){1,3}$");
    QRegularExpressionMatch matchIP = IP_validation_pattern.match(entered_IP);

    if(matchIP.hasMatch())
    {
        valid_IP = true;
        validated_IP = entered_IP;

        if(valid_IP)
        {
            ui->pushButton_Connect->setEnabled(true);
        }
        else
        {
            ui->pushButton_Connect->setEnabled(false);
        }
    }
    else
    {
        valid_IP = false;
        ui->statusBar->showMessage("Wrong connection data");
        ui->pushButton_Connect->setEnabled(false);
    }
}

void AskForIP_Interface::on_pushButton_Connect_clicked()
{
    ext_IP.setAddress(validated_IP);
    ext_Port = validated_Port.toUInt();

    this->close();
}

AskForIP_Interface::~AskForIP_Interface()
{
    delete ui;
}
