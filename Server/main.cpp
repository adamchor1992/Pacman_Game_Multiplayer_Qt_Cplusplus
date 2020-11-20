#include <QCoreApplication>
#include "pacman_server.h"

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);

    PacmanServer pacmanserver;

    application.exec();

    return 0;
}
