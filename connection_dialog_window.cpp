#include "connection_dialog_window.h"
#include "ui_connection_dialog_window.h"

ConnectionDialogWindow::ConnectionDialogWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ConnectionDialogWindow)
{
    ui->setupUi(this);

    m_IsValidIpAddress = false;
    ui->pushButton_Connect->setEnabled(false);
}

void ConnectionDialogWindow::on_lineEdit_IP_textChanged(const QString &enteredIpAddress)
{
    QRegularExpression IP_validation_pattern("^(\\d){1,3}\\.(\\d){1,3}\\.(\\d){1,3}\\.(\\d){1,3}$");
    QRegularExpressionMatch matchIP = IP_validation_pattern.match(enteredIpAddress);

    if(matchIP.hasMatch())
    {
        m_IsValidIpAddress = true;
        m_ValidatedIpAddressString = enteredIpAddress;

        if(m_IsValidIpAddress)
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
        m_IsValidIpAddress = false;
        ui->statusBar->showMessage("Wrong connection data");
        ui->pushButton_Connect->setEnabled(false);
    }
}

void ConnectionDialogWindow::on_pushButton_Connect_clicked()
{
    m_ValidatedIpAddress.setAddress(m_ValidatedIpAddressString);

    this->close();
}

ConnectionDialogWindow::~ConnectionDialogWindow()
{
    delete ui;
}
