#include <QCoreApplication>
#include "pacman_server.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PacmanServer pacmanserver;

    return a.exec();
}
