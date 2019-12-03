#include <csignal>

#include <QCoreApplication>
#include <QDir>
#include <QTimer>
#include <QThread>
#include <QDebug>

#include <Logger.h>
#include <Config.h>
#include <Message.h>

#include "Request.h"
#include "ClientRandomRequestGenerator.h"


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Config::setupLogging(QDir(QCoreApplication::applicationDirPath()).filePath("client_log.txt"));
    LOG_INFO("=== Starting client ===");

    Request requester;
    ClientRandomRequestGenerator requestGenerator;
    requester.setConnectionParams(Config::getServerAddr(), Config::getPort());

    QObject::connect(&requester
                     , &Request::responseReceived
                     , [&](const QByteArray& response)
    {
//#ifdef QT_DEBUG
        static int requestCount = 0;
        if (requestCount > 1000)
            app.quit();
        ++requestCount;
//#endif
        LOG_INFO(QString{"Response [%1]: %2"}.arg(requestCount).arg(QString::fromUtf8(response)));

        QVariantMap responseData = Message::decodeMessage(response);
        if (responseData.contains(Message::TYPE_KEY) && responseData.contains(Message::DATA_KEY))
        {
            if (responseData[Message::TYPE_KEY].value<Message::MessageType>() == Message::GET_KEYS)
            {
                QStringList keys;
                QVariantList dataList = responseData["data"].toList();
                for (auto item : dataList)
                    keys << item.toString();

                requestGenerator.setKnownKeys(keys);
            }
        }

        requester.send(requestGenerator.createRandomRequest());
    });

    QObject::connect(&requester, &Request::error, [&](const QString& errMsg)
    {
        LOG_ERROR(errMsg);
        if (!requestGenerator.keyListIsEmpty())
            requester.send(requestGenerator.createRandomRequest());
        else
        {
            LOG_WARNING("Everything went too wrong, let's quit!");
            app.quit();
        }
    });

    QObject::connect(&requester, &Request::critical, &app, &QCoreApplication::quit);

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&requester]
    {
        requester.closeConnection();
        LOG_INFO(QString{"\n\n Client average request timing = %1 ms"}.arg(requester.getAverageTiming()));
        LOG_INFO("=== Client finished ===");
    });

    // hacky-hacky =)
    QTimer::singleShot(0, &app, [&]
    {
        requester.send(ClientRandomRequestGenerator::createKeyListRequest());
    });

    std::signal(SIGINT, SIG_DFL);
    return app.exec();
}
