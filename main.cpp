#include "game_window.h"
#include "connection_dialog_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    ConnectionDialogWindow connectionDialogWindow;
    connectionDialogWindow.show();

    /*Divide program into 2 steps*/
    application.exec();

    GameWindow gameWindow(nullptr, connectionDialogWindow.GetIpAddress());
    gameWindow.show();

    application.exec();

    return 0;
}
