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
                     , [&requester, &requestGenerator](const QByteArray& response)
    {
        LOG_INFO(QString{"Response: %1"}.arg(QString::fromUtf8(response)));

        QVariantMap responseData = Message::decodeResponse(response);
        if (responseData.contains("type") && responseData.contains("data"))
        {
            if (responseData["type"].value<Message::MessageType>() == Message::GET_KEYS)
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

    QObject::connect(&requester, &Request::error, [&requester, &requestGenerator](const QString& errMsg)
    {
        LOG_ERROR(errMsg);
//        QThread::sleep(3);
        requester.send(requestGenerator.createRandomRequest());
    });

    QObject::connect(&requester, &Request::critical, [&app](const QString& errMsg)
    {
        LOG_ERROR(errMsg);
        app.quit();
    });

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&requester]
    {
        requester.closeConnection();
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
