#pragma once

#include <fstream>

class LogManager
{
public:
    static void ActivateLogging();
    static void LogToFile(std::string &&logMessage);

private:
    inline static std::string LOG_FILE_NAME = "log.txt";

    static std::ofstream m_LogFile;
    static bool m_LoggingActive;
};
