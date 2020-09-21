#include "game_window.h"
#include "connection_dialog_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ConnectionDialogWindow connectionDialogWindow;
    connectionDialogWindow.show();

    /*Divide program into 2 steps*/
    app.exec();

    GameWindow w(nullptr, connectionDialogWindow.GetIpAddress());
    w.show();

    return app.exec();
}
