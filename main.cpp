#include <QCoreApplication>
#include "pacmanserver.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    PacmanServer pacmanserver;

    pacmanserver.StartListening();

    //pacmanserver.ReadMessageFromClient1();

    return a.exec();
}
