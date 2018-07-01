#include "Game_window.h"
#include "askforipinterface.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AskForIP_Interface AskForIP_Interface_window;
    AskForIP_Interface_window.show();

    app.exec(); //divide program into 2 steps

    Game_window w(0, AskForIP_Interface_window.getIP(), AskForIP_Interface_window.getPort());
    w.show();

    return app.exec();
}
