#include "game_window.h"
#include "connection_dialog_window.h"
#include <QApplication>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    std::ofstream logFile("log.txt", std::ios_base::out | std::ios_base::app);

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
                logFile << "File " << inputFileName << " is empty" << std::endl << "Application will close now" << std::endl;
            }
            else
            {
                logFile << "Starting game in TEST mode" << std::endl;

                GameWindow gameWindow(nullptr, QHostAddress(QString::fromStdString(serverAddress)));
                gameWindow.show();

                application.exec();
            }
        }
        else
        {
            logFile << "Could not open " << inputFileName << std::endl << "Application will close now" << std::endl;
            return 0;
        }
    }

    return 0;
}
