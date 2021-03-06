#pragma once

#include <QMainWindow>
#include <QString>
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
    explicit ConnectionDialogWindow(QWidget* parent = nullptr);
    QHostAddress GetIpAddress(){return m_ValidatedIpAddress;}
    ~ConnectionDialogWindow();

private:
    Ui::ConnectionDialogWindow* ui;
    QString m_ValidatedIpAddressString;
    QHostAddress m_ValidatedIpAddress;
    bool m_IsValidIpAddress;

    void closeEvent(QCloseEvent*);

private slots:
    void on_lineEdit_IP_textChanged(const QString& enteredIpAddress);
    void on_pushButton_Connect_clicked();
};
