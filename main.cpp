#include "game_window.h"
#include "connection_dialog_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ConnectionDialogWindow connectionDialogWindow;
    connectionDialogWindow.show();

    app.exec(); //divide program into 2 steps

    GameWindow w(0, connectionDialogWindow.GetIpAddress());
    w.show();

    return app.exec();
}
