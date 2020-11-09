#pragma once

#include <fstream>

class LogManager
{
public:
    LogManager();

    static void LogToFile(std::string logMessage);

private:
    static std::ofstream logFile;
};
