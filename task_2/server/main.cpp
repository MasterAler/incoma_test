#include <csignal>

#include <QCoreApplication>
#include <QDir>

#include <Logger.h>

#include "RequestServer.h"
#include "Config.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Config::setupLogging(QDir(QCoreApplication::applicationDirPath()).filePath("server_log.txt"));
    LOG_INFO("=== Starting server ===");

    RequestServer responder;

    quint16 port = Config::getPort();
    if (!responder.listen(QHostAddress::Any, port)) {
        LOG_ERROR(QString{"Listen fail (port %1"}.arg(port));
        return EXIT_FAILURE;
    }
    LOG_INFO(QString{"Listen OK (port %1)"}.arg(port));

    std::signal(SIGINT, SIG_DFL);
    return a.exec();
}
