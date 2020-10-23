#include "game_window.h"
#include "connection_dialog_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

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
        QHostAddress serverAddress("192.168.1.6");

        GameWindow gameWindow(nullptr, serverAddress);
        gameWindow.show();

        application.exec();
    }

    return 0;
}
