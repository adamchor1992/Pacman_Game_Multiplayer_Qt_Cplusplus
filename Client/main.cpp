#include "game_window.h"
#include "connection_dialog_window.h"
#include <QApplication>
#include <fstream>
#include "../common/log_manager.h"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    LogManager::ActivateLogging(argv[1]);

    /*If no command line arguments were given*/
    if(argc == 1)
    {
        ConnectionDialogWindow connectionDialogWindow;
        connectionDialogWindow.show();

        /*Divide program into 2 steps*/
        application.exec();

        GameWindow gameWindow(nullptr, connectionDialogWindow.GetIpAddress());
        gameWindow.show();

        application.exec();
    }
    else
    {
        std::string inputFileName = "server_ip_address.txt";

        std::ifstream ipAddrInputFile(inputFileName, std::ios_base::in);

        if(ipAddrInputFile.is_open())
        {
            std::string serverAddress;

            std::getline(ipAddrInputFile, serverAddress);

            if(serverAddress.empty())
            {
                LogManager::LogToFile("File " + inputFileName + " is empt\ny" + "Application will close now\n");
            }
            else
            {
                LogManager::LogToFile("Starting game in TEST mode\n");

                GameWindow gameWindow(nullptr, QHostAddress(QString::fromStdString(serverAddress)));
                gameWindow.show();

                application.exec();
            }
        }
        else
        {
            LogManager::LogToFile("Could not open " + inputFileName + "\n" + "Application will close now\n");
            return 0;
        }
    }

    return 0;
}
