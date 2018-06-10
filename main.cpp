#include <QCoreApplication>
#include "pacmanserver.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PacmanServer pacmanserver;

    return a.exec();
}
