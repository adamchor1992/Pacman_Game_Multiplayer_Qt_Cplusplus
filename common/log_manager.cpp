#include "log_manager.h"
#include <cassert>
#include <QDebug>
#include <chrono>

std::ofstream LogManager::logFile("log.txt", std::ios_base::out);

LogManager::LogManager()
{
    if(logFile.is_open())
    {
        auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        logFile << "\n\n<<<<<" << std::ctime(&currentTime) << ">>>>>\n\n";
    }
    else
    {
        qDebug() << "Cannot open log file";
        assert(false);
    }
}

void LogManager::LogToFile(std::string logMessage)
{
    logFile << logMessage << std::endl;
    logFile.flush();
}

