#pragma once

#include <QStatusBar>

class StatusBarManager
{
public:
    static const int MESSAGE_TIMEOUT = 3000;

    StatusBarManager(QStatusBar* pStatusBar);

    static void ShowMessage(QString message, int messageTimeout = 0);
    static void ClearMessage();

private:
    static QStatusBar* m_pStatusBar;
};
