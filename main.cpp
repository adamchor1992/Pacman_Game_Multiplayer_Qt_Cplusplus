#include "game_window.h"
#include "ask_for_ip_interface.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AskForIP_Interface AskForIP_Interface_window;
    AskForIP_Interface_window.show();

    app.exec(); //divide program into 2 steps

    GameWindow w(0, AskForIP_Interface_window.getIP());
    w.show();

    return app.exec();
}
