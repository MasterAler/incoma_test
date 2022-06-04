#include <csignal>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDir>

#include <Logger.h>
#include <Config.h>

#include "RequestServer.h"
#include "RequestHandler.h"

#include "SimpleServerContainer.h"
#include "CustomServerContainer.h"

// N.B.: Ключ "-s" выбирает решение в лоб вместо самописного для хранения данных
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;

    QCommandLineOption useSimpleOption("s", QCoreApplication::translate("simple", "Use simple rw-locked container"));
    parser.addOption(useSimpleOption);
    parser.process(app);
    bool useSimple = parser.isSet(useSimpleOption);

    // ------------------------

    Config::setupLogging(QDir(QCoreApplication::applicationDirPath()).filePath("server_log.txt"));
    LOG_INFO("=== Starting server ===");

    RequestServer responder;
    responder.setMaxPendingConnections(50);
    std::shared_ptr<AbstractServerContainer> container;

    if (useSimple)
    {
        LOG_INFO("Using SimpleServerContainer");
        container = std::make_shared<SimpleServerContainer>();
    }
    else
    {
        LOG_INFO("Using CustomServerContainer");
        container = std::make_shared<CustomServerContainer>();
    }

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
