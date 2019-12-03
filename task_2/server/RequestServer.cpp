#include "RequestServer.h"

#include <Logger.h>
#include "ResponderThread.h"

RequestServer::RequestServer(QObject* parent)
    : QTcpServer(parent)
{ }

void RequestServer::incomingConnection(qintptr socketDescriptor)
{
    ResponderThread* thread = new ResponderThread(socketDescriptor, this);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &ResponderThread::error, [](QAbstractSocket::SocketError socketError){
        LOG_ERROR(QString{"Socket error: %1"}.arg(
                        QVariant::fromValue(socketError).toString() ));
    });

    thread->start();
}

