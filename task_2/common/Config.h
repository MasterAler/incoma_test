#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <FileAppender.h>
#include <ConsoleAppender.h>

namespace Config
{
    static const QString CONFIG_FILENAME{"config.ini"};

    quint16 getPort()
    {
        QSettings settings(CONFIG_FILENAME, QSettings::IniFormat);
        return static_cast<quint16>(settings.value("port", 20033).toUInt());
    }

    QString getServerAddr()
    {
        QSettings settings(CONFIG_FILENAME, QSettings::IniFormat);
        return settings.value("addr", "127.0.0.1").toString();
    }

    // здесь можно поменять уровень логирования,
    // а также то, куда пишется вывод
    void setupLogging(const QString& filename)
    {
        #ifdef QT_DEBUG
            Q_UNUSED(filename)
            ConsoleAppender* appender = new ConsoleAppender();
//            appender->setDetailsLevel(Logger::Trace);
        #else
            FileAppender* appender = new FileAppender(filename);
            appender->setDetailsLevel(Logger::Info);
        #endif
            cuteLogger->registerAppender(appender);
    }
}

#endif
