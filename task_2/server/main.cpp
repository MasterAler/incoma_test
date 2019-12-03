#include <csignal>

#include <QCoreApplication>
#include <QDir>

#include <Logger.h>
#include <Config.h>

#include "RequestServer.h"
#include "RequestHandler.h"
#include "SimpleServerContainer.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Config::setupLogging(QDir(QCoreApplication::applicationDirPath()).filePath("server_log.txt"));
    LOG_INFO("=== Starting server ===");

    RequestServer responder;
    responder.setMaxPendingConnections(50);
    std::shared_ptr<AbstractServerContainer> container = std::make_shared<SimpleServerContainer>();
    responder.setHandler(std::make_shared<RequestHandler>(container));

    quint16 port = Config::getPort();
    if (!responder.listen(QHostAddress::Any, port)) {
        LOG_ERROR(QString{"Listen fail (port %1"}.arg(port));
        return EXIT_FAILURE;
    }
    LOG_INFO(QString{"Listen OK (port %1)"}.arg(port));

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [container]
    {
        LOG_INFO(QString{"\n\n Container contains %1 records"}.arg(container->getKeys().length()));
        LOG_INFO(" === Server stopped ===");
    });


    std::signal(SIGINT, SIG_DFL);
    return app.exec();
}
