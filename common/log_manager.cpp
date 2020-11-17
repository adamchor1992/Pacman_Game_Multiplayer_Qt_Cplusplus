#include "log_manager.h"
#include <QDebug>
#include <chrono>

std::ofstream LogManager::m_LogFile(LOG_FILE_NAME, std::ios_base::out);
bool LogManager::m_LoggingActive = false;

void LogManager::ActivateLogging()
{
    if(m_LogFile.is_open())
    {
        auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        m_LogFile << "\n\n<<<<<" << std::ctime(&currentTime) << ">>>>>\n\n";
    }
    else
    {
        qDebug() << "Cannot open log file";
        assert(false);
    }

    m_LoggingActive = true;
}

void LogManager::LogToFile(std::string&& logMessage)
{
    if(m_LoggingActive == true)
    {
        m_LogFile << logMessage << std::endl;
        m_LogFile.flush();
    }
}

