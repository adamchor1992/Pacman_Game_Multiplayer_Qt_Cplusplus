#include "status_bar_manager.h"

QStatusBar* StatusBarManager::m_pStatusBar = nullptr;

StatusBarManager::StatusBarManager(QStatusBar* p_StatusBar)
{
    if(m_pStatusBar == nullptr)
    {
        m_pStatusBar = p_StatusBar;
    }

    /*Second constructor call will do nothing if m_pStatusBar was assigned the first time*/
}

void StatusBarManager::ShowMessage(QString message, int messageTimeout)
{
    if(m_pStatusBar != nullptr)
    {
        m_pStatusBar->showMessage(message, messageTimeout);
    }
    else
    {
        assert(false);
    }
}

void StatusBarManager::ClearMessage()
{
    if(m_pStatusBar != nullptr)
    {
        m_pStatusBar->clearMessage();
    }
    else
    {
        assert(false);
    }
}
