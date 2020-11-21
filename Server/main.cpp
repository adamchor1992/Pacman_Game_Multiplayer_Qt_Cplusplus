#include <QCoreApplication>
#include "pacman_server.h"
#include "../common/log_manager.h"

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);

    LogManager::ActivateLogging(argv[1]);

    PacmanServer pacmanserver;

    application.exec();

    return 0;
}
